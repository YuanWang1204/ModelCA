#include "transitionode.h"
#include "transition.h"
#include "cell.h"
#include "cellarraydouble.h"
#include <stdlib.h>
#include "speciesoncell.h"
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_errno.h>
#include "parseoptions.h"

const OptionDef odtransitionengine[] = {
  {"timestep", required, 1},
  {NULL, 0, 0}
};

gsl_odeiv2_step * stepper;
gsl_odeiv2_driver * driver;
double timestep;
double * yt;
double * yerr;
gsl_odeiv2_system sys;

int initparamstransitionengine(Option * opts, int optsc) {
  timestep = atof(getoption("timestep", opts, optsc).args[0]);
  return EXIT_SUCCESS;
}

void inittransitionengine() {
  sys.function = &odesys;
  sys.jacobian = NULL;
  sys.dimension = xmax * ymax * nspecies;
  sys.params = NULL;
  stepper = gsl_odeiv2_step_alloc(gsl_odeiv2_step_rkf45, xmax * ymax * nspecies);
  driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rkf45, timestep, 1e-6, 0.0);
  yt = malloc(sizeof(double) * xmax * ymax * nspecies);
  yerr = malloc(sizeof(double) * xmax * ymax * nspecies);
  for (int i = 0; i < xmax * ymax * nspecies; i++) {
    yerr[i] = 0;
  }
}

void cleanuptransitionengine() {
  free(yt);
  free(yerr);
  gsl_odeiv2_driver_free(driver);
  gsl_odeiv2_step_free(stepper);
}

double nextgridstate(double t, Grid curstate, Grid nextstate) {
  int i;
  double tcopy = t;
  for (int x = 0; x < xmax; x++) {
    for (int y = 0; y < ymax; y++) {
      for (int s = 0; s < nspecies; s++) {
        i = xy2i(x,y);
        yt[i * nspecies + s] = curstate[i]->val[s];
      }
    }
  }

  int status = gsl_odeiv2_driver_apply(driver, &tcopy, t + timestep, yt);
  //int status = gsl_odeiv2_step_apply(stepper, tcopy, timestep, yt, yerr, NULL, NULL, &sys);
  if (status != GSL_SUCCESS) {
    fprintf(stderr, "transitionode.nextgridstate: Error when applying step function.\n");
    exit(EXIT_FAILURE);
  }

  gsl_odeiv2_driver_reset(driver);

  // double sumerr[3] = {0,0,0};
  // for (int x = 0; x < xmax; x++) {
  //   for (int y = 0; y < ymax; y++) {
  //     for (int s = 0; s < nspecies; s++) {
  //       i = xy2i(x,y);
  //       sumerr[s] += yerr[i * nspecies + s];
  //     }
  //   }
  // }
  // for (int s = 0; s < nspecies; s++) {
  //   printf("%.20f\t", sumerr[s]);
  // }
  // printf("\n");

  for (int x = 0; x < xmax; x++) {
    for (int y = 0; y < ymax; y++) {
      for (int s = 0; s < nspecies; s++) {
        i = xy2i(x,y);
        if (yt[i*nspecies + s] > 1) yt[i*nspecies + s] = 1;
        if (yt[i*nspecies + s] < 0) yt[i*nspecies + s] = 0;
        nextstate[i]->val[s] = yt[i * nspecies + s];
      }
    }
  }

  return t + timestep;
}

double getcurrenterror(int x,int y,int s) {
  return yerr[xy2i(x,y) * nspecies + s];
}

