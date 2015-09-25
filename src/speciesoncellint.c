#include "speciesoncell.h"
#include "cellint.h"
#include "cell.h"

double speciesoncell(CellState * cs, int s) {
  if (cs->val == s) {
    return 1.0;
  } else {
    return 0.0;
  }
}