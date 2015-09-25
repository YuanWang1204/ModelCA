#include "stochasticity.h"
#include <stdlib.h>
#include "parseoptions.h"
#include <time.h>
#include <stdio.h>
#include <gsl/gsl_rng.h>

const OptionDef odstochasticity [] = {
  {"seed", optional, 1},
  {NULL, 0, 0}
};

int seed;

int initparamsstochasticity(Option * opts, int optsc){
  Option curopt = getoption("seed", opts, optsc);
  if (curopt.name == NULL) {
    seed = time(NULL);
  } else {
    seed = atoi(curopt.args[0]);
  }

  return EXIT_SUCCESS;
}



const gsl_rng_type * rngtype;
gsl_rng * rng;

void initstochasticity() {
  rngtype = gsl_rng_ranlxs0; 
  //gsl_rng_ranlxd1
  //gsl_rng_ranlxd2
  rng = gsl_rng_alloc(rngtype);
  gsl_rng_set(rng, seed);
}

void cleanupstochasticity() {
  gsl_rng_free(rng);
}