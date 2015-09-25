#include "neighbourhoodshape.h"
#include "neighbourhoodcircular.h"
#include <math.h>
#include <stdlib.h>

const OptionDef odneighbourhoodshape [] = {
  {"neighbourradius", required, 1},
  {NULL, 0, 0}
};

double neighbourradius;
int neighbourhoodcontainersize;

int initparamsneighbourhoodshape(Option * opts, int optsc) {
  neighbourradius = atof(getoption("neighbourradius", opts, optsc).args[0]);
  return EXIT_SUCCESS;
}

void initneighbourhoodshape() {
  neighbourhoodcontainersize = (int) pow((neighbourradius * 2 + 1),2);
}

int neighbourscoordinates(int xc, int yc, int retX[], int retY[]){
  int intr, xHalfRange;
  int i, x, y;

  intr = (int) neighbourradius;

  i = 0;
  for(y = yc - intr; y <= yc + intr; y++) {
    xHalfRange = (int) sqrt(pow(neighbourradius,2) - pow(abs(y-yc),2));
    for(x = xc - xHalfRange; x <= xc + xHalfRange; x++) {
      retX[i] = x;
      retY[i] = y;
      i++;
    }
  }
  return i;
}
