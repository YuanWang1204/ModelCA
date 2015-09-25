#include <stdlib.h>
#include <stdio.h>
#include "initcell.h"
#include "initcellpinit.h"
#include "cellarraydouble.h"
#include "speciesoncell.h"
#include <gsl/gsl_randist.h>
#include "stochasticity.h"

const OptionDef odinitcell [] = {
  {"initgaussianmean", required, oneormore},
  {"initgaussianstd", required, oneormore},
  {"initgaussianmin", required, 1},
  {"initgaussianmax", required, 1},
  {NULL,0,0}
};

//int nspecies;
double * initgaussianmean; //of size nspecies
double * initgaussianstd;
double initgaussianmin;
double initgaussianmax;

int initparamsinitcell(Option * opts, int optsc) {
  Option optinitgaussianmean = getoption("initgaussianmean", opts, optsc);
  if (nspecies != optinitgaussianmean.argsc) {
    fprintf(stderr, "Expected %d arguments for option initgaussianmean, got %d\n", nspecies, optinitgaussianmean.argsc);
    return EXIT_FAILURE;
  }
  initgaussianmean = malloc(sizeof(double) * nspecies);
  for (int i = 0; i < nspecies; i++) {
    initgaussianmean[i] = atof(optinitgaussianmean.args[i]);
  }

  Option optinitgaussianstd = getoption("initgaussianstd", opts, optsc);
  if (nspecies != optinitgaussianstd.argsc) {
    fprintf(stderr, "Expected %d arguments for option optinitgaussianstd, got %d\n", nspecies, optinitgaussianstd.argsc);
    return EXIT_FAILURE;
  }
  initgaussianstd = malloc(sizeof(double) * nspecies);
  for (int i = 0; i < nspecies; i++) {
    initgaussianstd[i] = atof(optinitgaussianstd.args[i]);
  }

  initgaussianmin = atof(getoption("initgaussianmin", opts, optsc).args[0]);
  initgaussianmax = atof(getoption("initgaussianmax", opts, optsc).args[0]);

  return EXIT_SUCCESS;
}

void freeparamsinitcell() {
  free(initgaussianmean);
  free(initgaussianstd);
  initgaussianmean=NULL;
  initgaussianstd=NULL;
}

double clampedgaussian(double mean, double sigma) {
  double variate = mean + gsl_ran_gaussian_ziggurat(rng, sigma);
  if(variate < initgaussianmin) return initgaussianmin;
  if(variate > initgaussianmax) return initgaussianmax;
  return variate;
}

void initcell(CellState * cs, int x, int y) {
  double rndvar;

  for (int s = 0; s < nspecies; s++) {
    rndvar = clampedgaussian(initgaussianmean[s], initgaussianstd[s]);
    cs->val[s] = rndvar;
  }
  // selectspecies = 0;
  // psum = pinit[selectspecies];
  // while(rnd >= psum) {
  //   selectspecies++;
  //   psum += pinit[selectspecies];
  // }
  // if (selectspecies == nspecies) {
  //   selectspecies = nspecies - 1;
  // }

  // for (int s = 0; s < nspecies; s++) {
  //   cs->val[s] = 0;
  // }
  // cs->val[selectspecies] = 1;
}
