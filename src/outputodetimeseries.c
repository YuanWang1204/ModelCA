#include "output.h"
#include "grid.h"
#include "cell.h"
#include <stdio.h>
#include <stdlib.h>

#include "speciesoncell.h"
#include "cellint.h"
#include "transitionode.h"

const OptionDef odoutput[] = {
  {NULL, 0, 0}
};

int initparamsoutput(Option * opts, int optsc) {
  return EXIT_SUCCESS;
}

void outputstart(Grid g) {
  printf("t,");
  for(int i = 0; i < nspecies - 1; i++) {
    printf("s%d,err%d,", i,i);
  }  
  printf("s%d,err%d\n",nspecies - 1,nspecies - 1);
} 

void output(Grid g, double t) {
  //unsigned int savsize = xmax * ymax;

  /* Population count */
  double count[nspecies];
  double errsum[nspecies];

  for(int s = 0; s < nspecies; s++) {
    count[s] = 0;
    errsum[s] = 0;
  }

  for(int x = 0; x < xmax; x++) {
    for(int y = 0; y < ymax; y++) {
      for(int s = 0; s < nspecies; s++) {
        count[s] += speciesoncell(g[xy2i(x,y)],s);
        errsum[s] += getcurrenterror(x,y,s);
      }
    }
  }

  printf("%f,", t);
  for(int i = 0; i < nspecies - 1; i++) {
    printf("%.5e,%.5e,", count[i] / ((double) xmax * ymax), errsum[i]/ ((double) xmax * ymax));
  }
  printf("%.5e,%.5e", count[nspecies - 1] / ((double) xmax * ymax), errsum[nspecies - 1] / ((double) xmax * ymax));
  printf("\n");
}

void outputfinish(Grid g, double t) {}
