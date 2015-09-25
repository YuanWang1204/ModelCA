#ifndef TRANSITIONODE_H
#define TRANSITIONODE_H

#include "cell.h"

extern double timestep;

int odesys(double t, const double y[], double dydt[], void * neighbourhood);

double getcurrenterror(int x,int y,int s);

#endif /*BOSQUET_H*/