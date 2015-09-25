#ifndef NEIGHBOURHOODSHAPE_H
#define NEIGHBOURHOODSHAPE_H

#include "parseoptions.h"

extern const OptionDef odneighbourhoodshape [];

int initparamsneighbourhoodshape(Option * opts, int optsc);
void initneighbourhoodshape();

int neighbourscoordinates(int xc, int yc, int retX[], int retY[]);

int neighbourhoodcontainersize;

#endif
