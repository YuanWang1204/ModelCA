#include "init.h"
#include "transition.h"
#include "stochasticity.h"
#include "neighbourhoodshape.h"

void init() {
  inittransitionengine();
  initstochasticity();
  initneighbourhoodshape();
}

void cleanup() {
  cleanuptransitionengine();
  cleanupstochasticity();
}