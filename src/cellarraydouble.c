#include "cell.h"
#include "cellarraydouble.h"
#include "speciesoncell.h"
#include <stdlib.h>

CellState * malloccellstate() {
  CellState * cs = malloc(sizeof(CellState));
  cs->val = malloc(sizeof(double) * nspecies);
  return cs;
}

void freecellstate(CellState * cs) {
  free(cs->val);
  free(cs);
}

void copycellstate(CellState * cs1, CellState * cs2) {
  for (int s = 0; s < nspecies; s++) {
    cs2->val[s] = cs1->val[s];
  }
}
