#include "neighbourhood.h"
#include "speciesoncell.h"
#include <math.h>
#include <stdlib.h>

#include "speciesoncell.h"
#include "cellint.h"

const OptionDef odneighbourhood [] = {
  {NULL, 0, 0}
};

int initparamsneighbourhood(Option * opts, int optsc){
  return EXIT_SUCCESS;
}

double * ngbhood = NULL; //nspecies

void mallocneighbourhood() {
  ngbhood = malloc(sizeof(double) * nspecies);
}

void freeneighbourhood() {
  free(ngbhood);
  ngbhood = NULL;
}

// void initneighbourhood(Grid g) {
//   int s, x, y, i;

//   for (s = 0; s < nspecies; s++) {
//     ngbhood[s] = 0;
//   }

//   for (x = 0; x < xmax; x++) {
//     for (y = 0; y < ymax; y++) {
//       i = xy2i(x,y);
//       for (s = 0; s < nspecies; s++) {
//         //addtoneighbourhood(ngbhood, g[i]);
//         ngbhood[s] += speciesoncell(g[i], s);
//       }
//     }
//   }
// }

double * neighbourhood(int x, int y) {
  return ngbhood;
}

void computeneighbourhood(Grid g) {
  int x,y,i,s;

  for (s = 0; s < nspecies; s++) {
    ngbhood[s] = 0;
  }

  for (x = 0; x < xmax; x++) {
    for (y = 0; y < ymax; y++) {
      i = xy2i(x,y);
      for (s = 0; s < nspecies; s++) {
        ngbhood[s] += speciesoncell(g[i], s);
      }
    }
  }
}
