#ifndef OUTPUT_H
#define OUTPUT_H

#include "grid.h"

extern const OptionDef odoutput[];
int initparamsoutput(Option * opts, int optsc);

void outputstart(Grid g);
void output(Grid g, double t);
void outputfinish(Grid g, double t);

#endif /*OUTPUT_H*/
