#include "speciesoncell.h"
#include "cellarraydouble.h"
#include "cell.h"

double speciesoncell(CellState * cs, int s) {
  return cs->val[s];
}