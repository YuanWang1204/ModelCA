#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "caengine.h"
#include "transition.h"
#include "transitionmarkov.h"
#include "transitionsbaudena.h"
#include "neighbourhood.h"
#include "cellint.h"
#include <gsl/gsl_errno.h>

const OptionDef odtransitiondynamics[] = {
  {"g1", required, 1},
  {"mu1", required, 1},
  {"mu2", required, 1},
  {"mu3", required, 1},
  {"c2", required, 1},
  {"c3", required, 1},
  {NULL, 0, 0}
};

int nspecies=3;
double g1;
double mu1;
double mu2;
double mu3;
double c2;
double c3;

int initparamstransitiondynamics(Option * opts, int optsc) {
  g1=atof(getoption("g1", opts, optsc).args[0]);
  mu1=atof(getoption("mu1", opts, optsc).args[0]);
  mu2=atof(getoption("mu2", opts, optsc).args[0]);
  mu3=atof(getoption("mu3", opts, optsc).args[0]);
  c2=atof(getoption("c2", opts, optsc).args[0]);
  c3=atof(getoption("c3", opts, optsc).args[0]);

  return EXIT_SUCCESS;
}

//Tout spatialisé
double dtreesdt(double trees, double young) {
  return g1 * young - mu1 * trees;
}

double dgrassdt(double trees, double grass) {
  return c2 * grass * (1 - trees - grass) - mu2 * grass;
}

double dyoungdt(double trees, double grass, double young) {
  return c3 * trees * (1 - trees - grass - young) - mu3 * young - g1 * young - c2 * grass * young;
}

int odesys(double t, const double yt[], double dydt[], void * params) {
  int x, y, i;
  //trees
  for (x=0; x < xmax; x++) {
    for (y=0; y < ymax; y++) {
      i = xy2i(x,y) * nspecies;
      dydt[i] = dtreesdt(neighbourhood(x,y)[0], neighbourhood(x,y)[2]);
      //grass
      dydt[i + 1] = dgrassdt(neighbourhood(x,y)[0], neighbourhood(x,y)[1]);
      //young
      dydt[i + 2] = dyoungdt(neighbourhood(x,y)[0], neighbourhood(x,y)[1], neighbourhood(x,y)[2]);
    }
  }
  return GSL_SUCCESS;
}


// //Spatialisation de la dispersion uniquement
// double dtreesdt(double trees, double grass, double young, double neighbtrees, double neighbgrass, double neighbyoung) {
//   return g1 * young - mu1 * trees;
// }

// double dgrassdt(double trees, double grass, double young, double neighbtrees, double neighbgrass, double neighbyoung) {
//   return c2 * neighbgrass * (1 - trees - grass) - mu2 * grass;
// }

// double dyoungdt(double trees, double grass, double young, double neighbtrees, double neighbgrass, double neighbyoung) {
//   return c3 * neighbtrees * (1 - trees - grass - young) - mu3 * young - g1 * young - c2 * neighbgrass * young;
// }

// int odesys(double t, const double yt[], double dydt[], void * params) {
//   int x, y, i;
//   //trees
//   for (x=0; x < xmax; x++) {
//     for (y=0; y < ymax; y++) {
//       i = xy2i(x,y) * nspecies;
//       dydt[i] = dtreesdt(yt[i], yt[i+1], yt[i+2], neighbourhood(x,y)[0], neighbourhood(x,y)[1], neighbourhood(x,y)[2]);
//       //grass
//       dydt[i + 1] = dgrassdt(yt[i], yt[i+1], yt[i+2], neighbourhood(x,y)[0], neighbourhood(x,y)[1], neighbourhood(x,y)[2]);
//       //young
//       dydt[i + 2] = dyoungdt(yt[i], yt[i+1], yt[i+2], neighbourhood(x,y)[0], neighbourhood(x,y)[1], neighbourhood(x,y)[2]);
//     }
//   }
//   return GSL_SUCCESS;
// }

// double pTransition(CellState * state1, CellState * state2, const double neighbourhood[]) {
//     int neighbourhoodsize = 0;
//     for (int i = 0; i < nspecies; i++) {
//         neighbourhoodsize += neighbourhood[i];
//     }

//     double T = 1;
//     if (T < c2 * neighbourhood[2] / (double) neighbourhoodsize + c3 * neighbourhood[1] / (double) neighbourhoodsize) T = c2 * neighbourhood[2] / (double) neighbourhoodsize + c3 * neighbourhood[1] / (double) neighbourhoodsize;
//     if (T < mu1) T = mu1;
//     if (T < mu2) T = mu2;
//     if (T < mu3 + g1 + c2 * neighbourhood[2] / (double) neighbourhoodsize) T = mu3 + g1 + c2 * neighbourhood[2] / (double) neighbourhoodsize;

//     switch (state1->val) {
//         case 0: 
//             switch (state2->val) {
//                 case 0: return 1 - ((c2 * (neighbourhood[2] / (double) neighbourhoodsize)) + (c3 * (neighbourhood[1] / (double) neighbourhoodsize))) / T;
//                 break;
//                 case 1: return 0;
//                 break;
//                 case 2: return (c2 * neighbourhood[2] / (double) neighbourhoodsize) / T;
//                 break;
//                 case 3: return (c3 * neighbourhood[1] / (double) neighbourhoodsize) / T;
//                 break;
//                 default: return 0;
//             }
//         break;
//         case 1:
//             switch (state2->val) {
//                 case 0: return mu1 / T;
//                 break;
//                 case 1: return 1 - mu1 / T;
//                 break;
//                 case 2: return 0;
//                 break;
//                 case 3: return 0;
//                 break;
//                 default: return 0;
//             }
//         break;
//         case 2:
//             switch (state2->val) {
//                 case 0: return mu2 / T;
//                 break;
//                 case 1: return 0;
//                 break;
//                 case 2: return 1 - mu2 / T;
//                 break;
//                 case 3: return 0;
//                 break;
//                 default: return 0;
//             }
//         break;
//         case 3:
//             switch (state2->val) {
//                 case 0: return mu3 / T;
//                 break;
//                 case 1: return g1 / T;
//                 break;
//                 case 2: return (c2 * neighbourhood[2] / (double) neighbourhoodsize) / T;
//                 break;
//                 case 3: return 1 - (mu3 + g1 + c2 * neighbourhood[2] / (double)neighbourhoodsize) / T;
//                 break;
//                 default: return 0;
//             }
//         break;
//         default: return 0;
//     }
// }
