#include <stdlib.h>
#include "initparams.h"
#include "parseoptions.h"
#include <stdio.h>
#include "time.h"
#include <stdbool.h>
#include <math.h>
#include "stochasticity.h"
#include "caengine.h"
#include "grid.h"
#include "initgrid.h"
#include "initcell.h"
#include "transition.h"
#include "neighbourhood.h"
#include "neighbourhoodshape.h"
#include "output.h"
#include <string.h>

 void addoptiondefs(OptionDef ** od, const OptionDef newod []) {
   int size1=0;
   int size2=0;

   for (size1 = 0; (*od)[size1].name != NULL; size1++) {}
   for (size2 = 0; newod[size2].name != NULL; size2++) {}

   OptionDef * res = malloc(sizeof(OptionDef) * (size1 + size2 + 1));
   //(*od) = realloc((*od), sizeof(OptionDef) * (size1 + size2 + 1));

   for(int i = 0; i < size1; i++) {
     res[i].name = strdup((*od)[i].name);
     res[i].requirement = (*od)[i].requirement;
     res[i].nargs = (*od)[i].nargs;
   }
  
   for (int i = 0; i < size2; i++) {
     res[size1 + i].name = strdup(newod[i].name);
     res[size1 + i].requirement = newod[i].requirement;
     res[size1 + i].nargs = newod[i].nargs;
   }

   res[size1 + size2].name = NULL;
   res[size1 + size2].requirement = 0;
   res[size1 + size2].nargs = 0;

   for (int i = 0; i < size1; i++) {
     freeoptiondef(&(*od)[i]);
   }
   free(*od);
   *od=res;
 }

//void addoptiondefs(OptionDef ** od, const OptionDef od2 []) {
//  int size1=0;
//  int size2=0;
//
//  for (size1 = 0; (*od)[size1].name != NULL; size1++) {}
//  for (size2 = 0; od2[size2].name != NULL; size2++) {}
//
//  OptionDef * odnew = malloc(sizeof(OptionDef) * (size1 + size2 + 1));
//  
//  for (int i = 0; i < size1; i++) {
//    odnew[i].name = (*od)[i].name;
//    odnew[i].requirement = (*od)[i].requirement;
//    odnew[i].nargs = (*od)[i].nargs;    
//  }
//
//  for (int i = 0; i < size2; i++) {
//    odnew[size1 + i].name = od2[i].name;
//    odnew[size1 + i].requirement = od2[i].requirement;
//    odnew[size1 + i].nargs = od2[i].nargs;
//  }
//  odnew[size1 + size2 + 1].name = NULL;
//  odnew[size1 + size2 + 1].requirement = 0;
//  odnew[size1 + size2 + 1].nargs = 0;
//
//  free(*od);
//
//  *od = odnew;
//}



int initparams(int argc, char * argv[]) {

  Option * opts = NULL;

  OptionDef * optsdef = malloc(sizeof(OptionDef));
  optsdef[0].name=NULL;
  optsdef[0].requirement=0;
  optsdef[0].nargs=0;

  addoptiondefs(&optsdef, odstochasticity);
  addoptiondefs(&optsdef, odcaengine);
  addoptiondefs(&optsdef, odgrid);
  addoptiondefs(&optsdef, odinitgrid);
  addoptiondefs(&optsdef, odinitcell);
  addoptiondefs(&optsdef, odtransitionengine);
  addoptiondefs(&optsdef, odtransitiondynamics);
  addoptiondefs(&optsdef, odneighbourhood);
  addoptiondefs(&optsdef, odneighbourhoodshape);
  addoptiondefs(&optsdef, odoutput);

  int optsc = parseoptions(optsdef, argc, argv, &opts);
  if (optsc == -1) {
    fprintf(stderr, "Options requirements not met.\n");
    return EXIT_FAILURE;
  }
  
  initparamsstochasticity(opts,optsc);
  initparamscaengine(opts, optsc);
  initparamsgrid(opts, optsc);
  initparamsneighbourhood(opts, optsc);
  initparamsneighbourhoodshape(opts, optsc);
  initparamsinitgrid(opts, optsc);
  initparamsinitcell(opts, optsc);
  initparamstransitionengine(opts, optsc);
  initparamstransitiondynamics(opts, optsc);
  initparamsoutput(opts, optsc);

  for (int i = 0; i < optsc; i++) {
    freeoption(&(opts[i]));
  }
  free(opts);
  opts=NULL;
  int odi;
  for (odi = 0; optsdef[odi].name != NULL; odi++) {
    freeoptiondef(&(optsdef[odi]));
  }
  free(optsdef);
  optsdef=NULL;

  return EXIT_SUCCESS;
}

void freeparams() {
    freeparamsinitcell();
}
