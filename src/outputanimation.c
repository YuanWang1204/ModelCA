#include "output.h"
#include "outputanimation.h"
#include "grid.h"
#include "cell.h"
#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include "caengine.h"
#include <string.h>

#include "cellint.h"
#include "speciesoncell.h"

typedef struct _Color {
  double r;
  double g;
  double b;
} Color;

const Color backgroundcolor = {0.2, 0.2, 0.2};

const Color colors[] = {
  {0,0,0},
  {0,1,0},
  {0.5,0.5,0.5}
};

const OptionDef odoutput[] = {
  {"imgfileprefix", required, 1},
  {NULL, 0, 0}
};

char * imgfileprefix;
int frame = 0;

int initparamsoutput(Option * opts, int optsc) {
  // imgfileprefix=getoption("imgfileprefix", opts, optsc).args[0];
  imgfileprefix=strdup(getoption("imgfileprefix", opts, optsc).args[0]);

  return EXIT_SUCCESS;
}

void outputstart(Grid g) {}

void outputfinish(Grid g, double t) {
  free(imgfileprefix);
  imgfileprefix = NULL;
}

void output(Grid g, double t) {
  cairo_surface_t *surface;
  cairo_t *cr;

  double size_factor = 5;
  
  double hspace = 5;

  double headerheight = 20;

  double soc;

  /*********/
  /* Cairo */
  /*********/

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 
                                       xmax * size_factor * nspecies + hspace * (nspecies - 1), 
                                       ymax * size_factor + headerheight);
  cr = cairo_create (surface);

  char filename [OUTPUT_GRID_PNG_FILENAME_MAX_LENGTH]="";
  sprintf(filename, "%s%0*d.png",imgfileprefix, FRAME_NUMBER_DIGITS, frame);
  frame++;

  // Paint the background white
  cairo_set_source_rgb(cr, backgroundcolor.r, backgroundcolor.g, backgroundcolor.b);
  cairo_paint(cr);

  // Draw the trees black
  for (int s = 0; s < nspecies; s++) {
    for (int x = 0; x < xmax; x++) {
      for (int y = 0; y < ymax; y++) {
        cairo_rectangle(cr, 
          s * (xmax * size_factor + hspace) + x * size_factor,
          y * size_factor,
          1 * size_factor,
          1 * size_factor);
          soc = 1 - speciesoncell(g[xy2i(x,y)], s);
          cairo_set_source_rgb(cr, soc, soc, soc);
          cairo_fill(cr);
      }
    }
  }

  cairo_destroy(cr);
  fprintf(stderr, "writing png to %s\n", filename);
  cairo_surface_write_to_png(surface, filename);
  cairo_surface_destroy(surface);
}