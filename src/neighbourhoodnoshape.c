#include "neighbourhoodshape.h"
#include "parseoptions.h"
#include <math.h>
#include <stdlib.h>

const OptionDef odneighbourhoodshape [] = {
  {NULL, 0, 0}
};

int initparamsneighbourhoodshape(Option * opts, int optsc) {
  neighbourhoodcontainersize = 0;

  return EXIT_SUCCESS;
}

void initneighbourhoodshape() {}

int neighbourscoordinates(int xc, int yc, int retX[], int retY[]){
  retX = NULL;
  retY = NULL;
  return 0;
}
