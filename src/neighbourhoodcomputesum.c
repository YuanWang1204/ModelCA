#include "neighbourhoodcompute.h"
#include "cell.h"
#include "speciesoncell.h"
#include "neighbourhoodshape.h"


// int * xneighbours = NULL;
// int * yneighbours = NULL;

//   xneighbours = malloc(sizeof(int) * neighbourhoodcontainersize); //(int) pow((neighbourradius * 2 + 1),2));
//   yneighbours = malloc(sizeof(int) * neighbourhoodcontainersize); //(int) pow((neighbourradius * 2 + 1),2));

void computecellneighbourhood(Grid g, int x, int y, double neighbourhood[]) {
  int j, s;

  int xneighbours[neighbourhoodcontainersize];
  int yneighbours[neighbourhoodcontainersize];

  for (s = 0; s < nspecies; s++) {
    neighbourhood[s] = 0;
  }

  int neighboursSize = neighbourscoordinates(x,y, xneighbours, yneighbours);
  //update neighbour count of neighbouring neighbours.
  for (int ir = 0; ir < neighboursSize; ir++) {
    j = xy2i(xneighbours[ir], yneighbours[ir]);
    for (s = 0; s < nspecies; s++) {
      neighbourhood[s] += speciesoncell(g[j], s);
    }
  }
}
