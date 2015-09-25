#include "grid.h"
#include "initgrid.h"
#include "cell.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "parseoptions.h"

#include "speciesoncell.h"
#include "initcell.h"

const OptionDef odinitgrid [] = {
  {NULL,0,0}
};

int initparamsinitgrid(Option * opts, int optsc) {
  return EXIT_SUCCESS;
}

void freeparamsinitgrid() {
}

void initgrid(Grid g) {
  int x,y;

  for (x = 0; x < xmax; x++) {
    for (y = 0; y < ymax; y++) {
      initcell(g[xy2i(x,y)], x, y);
    }
  }
}

