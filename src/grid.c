#include "grid.h"
#include "cell.h"
#include <math.h>
#include <stdlib.h>
#include "neighbourhood.h"
#include "parseoptions.h"

#include <stdio.h>

const OptionDef odgrid [] = {
  {"xmax", required, 1},
  {"ymax", required, 1},
  {NULL,0,0}
};

int xmax;
int ymax;

int initparamsgrid(Option * opts, int optsc) {
  xmax=atoi(getoption("xmax", opts, optsc).args[0]);
  ymax=atoi(getoption("ymax", opts, optsc).args[0]);
  return EXIT_SUCCESS;
}

CellState * cell(Grid g, int x, int y) {
  return g[xy2i(x,y)];  
}
 
Grid mallocgrid() {
  Grid g = malloc(sizeof(CellState *) * xmax * ymax);
  for (int x=0; x < xmax; x++) {
    for (int y=0; y < ymax; y++) {
      g[xy2i(x,y)] = malloccellstate();
    }
  }
  return g;
}

void freegrid(Grid * g) {
  for (int x=0; x < xmax; x++) {
    for (int y=0; y < ymax; y++) {
      freecellstate((*g)[xy2i(x,y)]);
    }
  }
  free(*g);
  (*g) = NULL;
}

void copygrid(const Grid g1, Grid g2) {
  int x,y,i;
  for (x = 0; x < xmax; x++) {
    for (y = 0; y < ymax; y++) {
      i = xy2i(x,y);
      copycellstate(g1[i], g2[i]);
    }
  }
}
