#ifndef STOCHASTICITY_H
#define STOCHASTICITY_H

#include "parseoptions.h"
#include <gsl/gsl_rng.h>

extern int seed;
extern gsl_rng * rng;

extern const OptionDef odstochasticity [];
int initparamsstochasticity(Option * opts, int optsc);
void initstochasticity();
void cleanupstochasticity();


#endif