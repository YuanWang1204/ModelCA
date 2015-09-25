#include <stdlib.h>
#include "init.h"
#include "initparams.h"
#include "caengine.h"
#include <stdio.h>
#include "stochasticity.h"

// #include "transition.h"

int main(int argc, char* argv[]) {

  if (initparams(argc, argv) == EXIT_FAILURE) return EXIT_FAILURE;

  init();

  run();  

  cleanup();

  freeparams();
  
  return EXIT_SUCCESS;
}
