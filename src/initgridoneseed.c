#include "grid.h"
#include "initgrid.h"
#include "cell.h"
#include "cellarraydouble.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "parseoptions.h"
#include "speciesoncell.h"

const OptionDef odinitgrid [] = {
  {"initbase", required, oneormore},
  {"initseed", required, oneormore},
  {NULL,0,0}
};

double * initbase;
double * initseed;

int initparamsinitgrid(Option * opts, int optsc) {
  Option optinitbase = getoption("initbase", opts, optsc);
  if (nspecies != optinitbase.argsc) {
    fprintf(stderr, "Expected %d arguments for option initbase, got %d\n", nspecies, optinitbase.argsc);
    return EXIT_FAILURE;
  }
  initbase = malloc(sizeof(double) * nspecies);
  for (int i = 0; i < nspecies; i++) {
    initbase[i] = atof(optinitbase.args[i]);
  }

  Option optinitseed = getoption("initseed", opts, optsc);
  if (nspecies != optinitseed.argsc) {
    fprintf(stderr, "Expected %d arguments for option initseed, got %d\n", nspecies, optinitseed.argsc);
    return EXIT_FAILURE;
  }
  initseed = malloc(sizeof(double) * nspecies);
  for (int i = 0; i < nspecies; i++) {
    initseed[i] = atof(optinitseed.args[i]);
  }
  
  return EXIT_SUCCESS;
}

void freeparamsinitgrid() {
    free(initbase);
    free(initseed);
}

void initgrid(Grid g) {
  int x,y,s;

  for (x = 0; x < xmax; x++) {
    for (y = 0; y < ymax; y++) {
      for (s = 0; s < nspecies; s++) {
        g[xy2i(x,y)]->val[s] = initbase[s];
      }
    }
  }

  for (s = 0; s < nspecies; s++) {
    g[xy2i(xmax / 2, ymax / 2)]->val[s] = initseed[s];
  }
}