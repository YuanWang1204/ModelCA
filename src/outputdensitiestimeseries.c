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

void outputstart(Grid g) {
  printf("t,");
  for(int i = 0; i < nspecies - 1; i++) {
    printf("s%d,", i);
  }  
  printf("s%d\n",nspecies - 1);
} 

void output(Grid g, double t) {
  unsigned int savsize = xmax * ymax;

  /* Population count */
  double count[nspecies];

  for(int s = 0; s < nspecies; s++) {
    count[s] = 0;
  }

  for(int i = 0; i < savsize; i++) {
    for(int s = 0; s < nspecies; s++) {
      count[s] += speciesoncell(g[i],s);
    }
  }

  printf("%f,", t);
  for(int i = 0; i < nspecies - 1; i++) {
    printf("%e,", count[i] / (float)savsize);
  }
  printf("%e", count[nspecies - 1] / (float)savsize);
  printf("\n");
}

void outputfinish(Grid g, double t) {}