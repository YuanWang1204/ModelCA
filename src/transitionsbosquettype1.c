#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "caengine.h"
#include "transition.h"
#include "transitionmarkov.h"
#include "transitionsbosquettype1.h"
#include "parseoptions.h"
#include "cell.h"

#include "speciesoncell.h"
#include "cellint.h"

const OptionDef odtransitiondynamics[] = {
  {"pr1", required, 1},
  {"pr2", required, 1},
  {"ntr1", required, 1},
  {"ntr2", required, 1},
  {"pm1", required, 1},
  {"pm2", required, 1},
  {"ntm1", required, 1},
  {"ntm2", required, 1},
  {NULL, 0, 0}
};

int nspecies=2;
double pr1;
double pr2;
double ntr1;
double ntr2;
double pm1;
double pm2;
double ntm1;
double ntm2;

int initparamstransitiondynamics(Option * opts, int optsc) {
  pr1=atof(getoption("pr1", opts, optsc).args[0]);
  pr2=atof(getoption("pr2", opts, optsc).args[0]);
  ntr1=atof(getoption("ntr1", opts, optsc).args[0]);
  ntr2=atof(getoption("ntr2", opts, optsc).args[0]);
  pm1=atof(getoption("pm1", opts, optsc).args[0]);
  pm2=atof(getoption("pm2", opts, optsc).args[0]);
  ntm1=atof(getoption("ntm1", opts, optsc).args[0]);
  ntm2=atof(getoption("ntm2", opts, optsc).args[0]);

  return EXIT_SUCCESS;
}


// double p_rec_type1(double neighbours, Species* sp) {
//   return f_type1(neighbours, sp->ntr1, sp->ntr2, sp->pr1, sp->pr2);
// }

// double p_mor_type1(double neighbours, Species* sp) {
//   return f_type1(neighbours, sp->ntm1, sp->ntm2, sp->pm1, sp->pm2);
// }

// double p_rec_type2(double neighbours, Species* sp) {
//   return f_type2(neighbours, sp->ntr1, sp->ntr2, sp->ntr3, sp->ntr4, 
//                              sp->pr1, sp->pr2, sp->pr3);
// }

// double p_mor_type2(double neighbours, Species* sp) {
//   return f_type2(neighbours, sp->ntm1, sp->ntm2, sp->ntm3, sp->ntm4, 
//                              sp->pm1, sp->pm2, sp->pm3);
// }


double f_type1(double x, 
                   double x0,
                   double x1,
                   double y0,
                   double y1) {
  if(x <= x0) {
    return y0;
  } else if (x >= x1) {
    return y1;
  } else {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
  }
}


double pTransition(CellState * state1, CellState * state2, const double neighbourhood[]) {
    int neighbourhoodtotal = 0;
    for (int i = 0; i < nspecies; i++) {
        neighbourhoodtotal += neighbourhood[i];
    }

    switch (state1->val) {
        case 0: 
            switch (state2->val) {
                case 0: 
                    return 1 - f_type1(neighbourhood[1] / neighbourhoodtotal,
                                       ntr1 / neighbourhoodtotal, 
                                       ntr2 / neighbourhoodtotal, 
                                       pr1, 
                                       pr2);
                break;
                case 1: //recrutement
                    return f_type1(neighbourhood[1] / neighbourhoodtotal,
                                   ntr1 / neighbourhoodtotal, 
                                   ntr2 / neighbourhoodtotal,
                                   pr1, 
                                   pr2);
                break;
                default: 
                  fprintf(stderr, "Unknown state %d\n", state2->val);
                return 0;
            }
        break;
        case 1: 
            switch (state2->val) {
                case 0: return f_type1(neighbourhood[1] / neighbourhoodtotal, // exclude self
                                       ntm1 / neighbourhoodtotal, 
                                       ntm2 / neighbourhoodtotal, 
                                       pm1, 
                                       pm2); //mort
                break;
                case 1: return 1 - f_type1(neighbourhood[1] / neighbourhoodtotal, // exclude self 
                                           ntm1 / neighbourhoodtotal, 
                                           ntm2 / neighbourhoodtotal, 
                                           pm1, 
                                           pm2);
                break;
                default: 
                  fprintf(stderr, "Unknown state %d\n", state2->val);
                  return 0;
            }
        break;
        default: 
          fprintf(stderr, "Unknown state %d\n", state1->val);
          return 0;
    }
}
