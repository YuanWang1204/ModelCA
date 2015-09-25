#include "transitionmarkov.h"
#include "transition.h"
#include "cell.h"
#include "cellint.h"
#include <stdlib.h>
#include "speciesoncell.h"
#include "neighbourhood.h"

const OptionDef odtransitionengine[] = {
  {NULL, 0, 0}
};

int initparamstransitionengine(Option * opts, int optsc) {
  return EXIT_SUCCESS;
}

void inittransitionengine() {}
void cleanuptransitionengine() {}


void nextcellstate(double t, CellState * curstate, const double curneighbourhood[], CellState * newstate) {
  double rnd,psum;

  rnd = drand48();
  newstate->val = 0;
  psum = pTransition(curstate, newstate, curneighbourhood);
  while(rnd >= psum) {
    (newstate->val)++;
    psum += pTransition(curstate, newstate, curneighbourhood);
  }
  if (newstate->val == nspecies) {
    newstate->val = nspecies - 1;
  }
}

double nextgridstate(double t, Grid curstate, Grid nextstate) {
  /* update cell states asynchronously*/ 
  for (int x = 0; x < xmax; x++) {
    for (int y = 0; y < ymax; y++) {
      nextcellstate(t,
                    cell(curstate, x, y), 
                    neighbourhood(x, y), 
                    cell(nextstate, x, y));
    }
  }

  return t + 1.0;
}
