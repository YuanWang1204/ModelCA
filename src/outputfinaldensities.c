#include "output.h"
#include "grid.h"
#include "cell.h"
#include <stdio.h>
#include <stdlib.h>

#include "speciesoncell.h"
#include "cellint.h"

const OptionDef odoutput[] = {
  {NULL, 0, 0}
};

int initparamsoutput(Option * opts, int optsc) {
  return EXIT_SUCCESS;
}

void outputstart(Grid g) {}

void output(Grid g, double t) {
}

void outputfinish(Grid g, double t) {
  unsigned int savsize = xmax * ymax;

  /* Population count */
  int count[nspecies];

  for(int s = 0; s < nspecies; s++) {
    count[s] = 0;
  }

  for(int i = 0; i < savsize; i++) {
    for (int s = 0; s < nspecies; s++) {
      count[s] += speciesoncell(g[i], s);
    }
  }

  for(int i = 0; i < nspecies - 1; i++) {
    printf("%f,", count[i] / (float)savsize);
  }
  printf("%f", count[nspecies - 1] / (float)savsize);
  printf("\n");
}