#include <stdlib.h>
#include <stdio.h>
#include "initcell.h"
#include "initcellpinit.h"
#include "cellint.h"

const OptionDef odinitcell [] = {
  {"pinit", required, oneormore},
  {NULL,0,0}
};

int nspecies;
double * pinit;

int initparamsinitcell(Option * opts, int optsc) {
  Option curopt = getoption("pinit", opts, optsc);

  if (nspecies != curopt.argsc) {
    fprintf(stderr, "Expected %d arguments for option pinit, got %d\n", nspecies, curopt.argsc);
    return EXIT_FAILURE;
  }

  pinit = malloc(sizeof(double) * nspecies);
  for (int i = 0; i < nspecies; i++) {
    pinit[i] = atof(curopt.args[i]);
  }

  return EXIT_SUCCESS;
}

void freeparamsinitcell() {
  free(pinit);
  pinit=NULL;
}

void initcell(CellState * cs, int x, int y) {
  double rnd, psum;
  int newstate;

  rnd = drand48();
  newstate = 0;
  psum = pinit[newstate];
  while(rnd >= psum) {
    newstate++;
    psum += pinit[newstate];
  }
  if (newstate == nspecies) {
    newstate = nspecies - 1;
  }

  cs->val = newstate;
}
