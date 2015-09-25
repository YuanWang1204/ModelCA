#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "caengine.h"
#include "grid.h"
#include "transition.h"
#include "cell.h"
#include "output.h"
#include "neighbourhood.h"
#include "parseoptions.h"
#include "initgrid.h"

static Grid g;
static Grid gridbuffer;

const OptionDef odcaengine [] = {
  {"tmax", required, 1},
  {"outputevery", optional, 1},
  {NULL,0,0}
};

double tmax;
int outputevery;
bool flag_tmax;
bool flag_stable;

int initparamscaengine(Option * opts, int optsc) {
  Option opttmax = getoption("tmax", opts, optsc);
  if (opttmax.name != NULL) {
    tmax=atof(opttmax.args[0]);
    flag_tmax=true;
  } else {
    tmax=0;
    flag_tmax=false;
  }

  Option optoutputevery = getoption("outputevery", opts, optsc);
  if (optoutputevery.name != NULL) {
    outputevery=atoi(optoutputevery.args[0]);
  } else {
    outputevery=1;
  }

  flag_stable=false;

  return EXIT_SUCCESS;
}

bool stopcondition(Grid g, double t, int iter) {
  return (t >= tmax);
}

void run() {
    g = mallocgrid();
    gridbuffer = mallocgrid();
  
    initgrid(g);
    copygrid(g, gridbuffer);
  
    mallocneighbourhood();
    computeneighbourhood(g);
    
    outputstart(g);

    double t = 0;
    int iter = 0;
    for(t = 0; !stopcondition(g,t,iter); t = step(t)) {
      if(iter % outputevery == 0) {
        output(g,t);
      }
      iter++;
    }
    output(g, t);
    outputfinish(g, t);  
  
    freegrid(&g);
    freegrid(&gridbuffer);
    freeneighbourhood();
}

double step(double t) {
  /* save grid state into buffer */
  copygrid(g, gridbuffer);
     
  t = nextgridstate(t, gridbuffer, g);

  /* update neighbourhood */  
  computeneighbourhood(g);

  return t;
}

