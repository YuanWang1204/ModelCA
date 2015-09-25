#include <stdlib.h>
#include <stdio.h>
#include "initcell.h"
#include "stochasticity.h"

const OptionDef odinitcell [] = {
  {NULL,0,0}
};

int initparamsinitcell(Option * opts, int optsc) {
  return EXIT_SUCCESS;
}

void freeparamsinitcell() {
}

void initcell(CellState * cs, int x, int y) {
}
