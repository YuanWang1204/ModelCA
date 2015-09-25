#include "cell.h"
#include "cellint.h"
#include <stdlib.h>

CellState * malloccellstate() {
  return malloc(sizeof(CellState));
}

void freecellstate(CellState * cs) {
  free(cs);
}

void copycellstate(CellState * cs1, CellState * cs2) {
  *cs2 = *cs1;
}
