#ifndef INITCELL_H
#define INITCELL_H

#include "cell.h"
#include "parseoptions.h"

extern const OptionDef odinitcell [];

int initparamsinitcell(Option * opts, int optsc);
void freeparamsinitcell();

void initcell(CellState * cs, int x, int y);

#endif
