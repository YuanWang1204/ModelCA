#include "neighbourhood.h"
#include "neighbourhoodshape.h"
#include <math.h>
#include <stdlib.h>

#include "neighbourhoodcompute.h"
#include "speciesoncell.h"
#include "cellint.h"

#include <stdio.h>

const OptionDef odneighbourhood [] = {
  {NULL, 0, 0}
};

int initparamsneighbourhood(Option * opts, int optsc) {

  return EXIT_SUCCESS;
}

double ** neighbourscount = NULL; //xmax * ymax * nspecies
int * xneighbours = NULL; //number of neighbours
int * yneighbours = NULL; //number of neighbours
double * neighbourhoodavg = NULL;//nspecies

void mallocneighbourhood() {
  neighbourscount = malloc(sizeof(double*) * xmax * ymax);
  for (int x =0; x < xmax; x++) {
    for (int y = 0; y < ymax; y++) {
      neighbourscount[xy2i(x,y)] = malloc(sizeof(double) * nspecies);
    }
  }
  xneighbours = malloc(sizeof(int) * neighbourhoodcontainersize);
  yneighbours = malloc(sizeof(int) * neighbourhoodcontainersize);
  neighbourhoodavg = malloc(sizeof(double) * nspecies);
}

void freeneighbourhood() {
  for (int x =0; x < xmax; x++) {
    for (int y = 0; y < ymax; y++) {
      free(neighbourscount[xy2i(x,y)]);
      neighbourscount[xy2i(x,y)] = NULL;
    }
  }
  free(neighbourscount);
  neighbourscount = NULL;
  free(neighbourhoodavg);
}

// void initneighbourhood(Grid g) {
//   int x, y, s, i,neighboursSize;

//   for (x = 0; x < xmax; x++) {
//     for (y = 0; y < ymax; y++) {
//       for (s = 0; s < nspecies; s++) {
//         neighbourscount[xy2i(x,y)][s] = 0;
//       }
//     }
//   }

//   for (x = 0; x < xmax; x++) {
//     for (y = 0; y < ymax; y++) {
//       neighboursSize = neighbourscoordinates(x,y, xneighbours, yneighbours);
//       //update neighbour count of neighbouring neighbours.

//       for (int ir = 0; ir < neighboursSize; ir++) {
//         i = xy2i(xneighbours[ir], yneighbours[ir]);
//         for (s = 0; s < nspecies; s++) {
//           neighbourscount[i][s] += speciesoncell(g[i], s);
//         }

//       } 
//     }
//   }

//   for (s = 0; s < nspecies; s++) {
//     neighbourhoodavg[s] = 0;

//     for (x = 0; x < xmax; x++) {
//       for (y = 0; y < ymax; y++) {
//         neighbourhoodavg[s] += neighbourscount[xy2i(x,y)][s];
//       }
//     }
//     // printf("sum neighbours %d %f\n", s, neighbourhoodavg[s]);
//     neighbourhoodavg[s] /= (double) xmax * ymax;
//   }
// }

double * neighbourhood(int x, int y) {
  return neighbourhoodavg;
}

void computeneighbourhood(Grid g) {
  int x,y;

  for (x = 0; x < xmax; x++) {
    for (y = 0; y < ymax; y++) {
      computecellneighbourhood(g, x, y, neighbourscount[xy2i(x,y)]);
    }
  }

  for (int s = 0; s < nspecies; s++) {
    neighbourhoodavg[s] = 0;

    for (x = 0; x < xmax; x++) {
      for (y = 0; y < ymax; y++) {
        neighbourhoodavg[s] += neighbourscount[xy2i(x,y)][s];
      }
    }
    // printf("sum neighbours %d %f\n", s, neighbourhoodavg[s]);

    neighbourhoodavg[s] /= (double) xmax * ymax;
  }
}
