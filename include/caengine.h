#ifndef CAENGINE_H
#define CAENGINE_H

#include <stdbool.h>
#include "grid.h"
#include "parseoptions.h"


// /* STABILITY, MSER */
// #define MSER_IGNORE_LAST_POINTS 5
// #define MSER_M 5
// #define TEST_STABILITY_EVERY 50
// // the simulation is considered stable when d <= iter / STABILITY_LAG_FACTOR

struct _SimuAggregState;
typedef struct _SimuAggregState SimuAggregState;

extern double tmax;
extern int outputevery;
extern bool flag_tmax;
extern bool flag_stable;

extern const OptionDef odcaengine [];
int initparamscaengine(Option * opts, int optsc);

void run();
double step();
bool stopcondition(Grid g, double t, int iter);

// void initglobals();
// void updateglobals(Grid g, int iter);
// void freeglobals();

#endif /*CAENGINE_H*/
