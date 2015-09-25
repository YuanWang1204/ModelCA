#ifndef TRANSITION_H
#define TRANSITION_H

#include "grid.h"
#include "cell.h"
#include "parseoptions.h"

extern const OptionDef odtransitionengine[];
extern const OptionDef odtransitiondynamics[];

int initparamstransitionengine(Option * opts, int optsc);
int initparamstransitiondynamics(Option * opts, int optsc);

void inittransitionengine();
void cleanuptransitionengine();

double nextgridstate(double t, Grid curstate, Grid nextstate);
//void nextcellstate(double t, CellState * curstate, const double neighbourhood[], CellState * nextstate);

#endif /*BOSQUET_H*/