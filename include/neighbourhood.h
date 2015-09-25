#ifndef NEIGHBOURHOOD_H
#define NEIGHBOURHOOD_H

#include "grid.h"

extern const OptionDef odneighbourhood [];

int initparamsneighbourhood(Option * opts, int optsc);

void mallocneighbourhood();
//void initneighbourhood(Grid g);
void freeneighbourhood();

double * neighbourhood(int x, int y);

void computeneighbourhood(Grid g);

#endif
