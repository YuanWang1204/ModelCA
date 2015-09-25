#include <stdlib.h>
#include <stdio.h>
#include "initcell.h"
#include "initcellpinit.h"
#include "cellint.h"
#include "grid.h"
#include <math.h>

const OptionDef odinitcell [] = {
  {"pinitinclump", required, oneormore},
  {"pinitoutclump", required, oneormore},
  {"clumpradius", required, 1},
  {NULL,0,0}
};

int nspecies;
double * pinitinclump;
double * pinitoutclump;
double clumpradius;

int initparamsinitcell(Option * opts, int optsc) {
  Option optpinitinclump = getoption("pinitinclump", opts, optsc);

  if (nspecies != optpinitinclump.argsc) {
    fprintf(stderr, "Expected %d arguments for option pinitinclump, got %d\n", nspecies, optpinitinclump.argsc);
    return EXIT_FAILURE;
  }

  pinitinclump = malloc(sizeof(double) * nspecies);
  for (int i = 0; i < nspecies; i++) {
    pinitinclump[i] = atof(optpinitinclump.args[i]);
  }

  Option optpinitoutclump = getoption("pinitoutclump", opts, optsc);

  if (nspecies != optpinitoutclump.argsc) {
    fprintf(stderr, "Expected %d arguments for option pinitoutclump, got %d\n", nspecies, optpinitoutclump.argsc);
    return EXIT_FAILURE;
  }

  pinitoutclump = malloc(sizeof(double) * nspecies);
  for (int i = 0; i < nspecies; i++) {
    pinitoutclump[i] = atof(optpinitoutclump.args[i]);
  }

  Option optclumpradius = getoption("clumpradius", opts, optsc);
  clumpradius = atof(optclumpradius.args[0]);

  return EXIT_SUCCESS;
}

void freeparamsinitcell() {
  free(pinitinclump);
  pinitinclump=NULL;
  free(pinitoutclump);
  pinitoutclump=NULL;
}

void initcell(CellState * cs, int x, int y) {
  double rnd, psum;
  int newstate;

  double * pinit = NULL;

  // printf("x = %d, y = %d, xmax = %d, ymax = %d, dist = %f\n", x, y, xmax, ymax, sqrt(pow(x - (xmax / 2),2) + pow(y - (ymax / 2),2)));
  if (sqrt(pow(x - (xmax / 2),2) + pow(y - (ymax / 2),2)) <= clumpradius) {
    pinit = pinitinclump;
  } else {
    pinit = pinitoutclump;
  }

  // printf("pinit =");
  // for (int i = 0; i < nspecies; i++) {
  //   printf(" %f", pinit[i]);
  // }
  // printf("\n");

  rnd = drand48();
  newstate = 0;
  psum = pinit[newstate];
  while(rnd >= psum) {
    newstate++;
    psum += pinit[newstate];
  }
  if (newstate == nspecies) {
    newstate = nspecies - 1;
  }

  cs->val = newstate;
}
