#include "parseoptions.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

const Option nulloption = {NULL, NULL, 0};
const OptionDef nulloptiondef = {NULL, 0, 0};

void freeoption(Option * opt) {
  free(opt->name);
  opt->name=NULL;
  for (int i = 0; i < opt->argsc; i++) {
    free(opt->args[i]);
    opt->args[i]=NULL;
  }
  free(opt->args);
  opt->args=NULL;
}
void freeoptiondef(OptionDef * od) {
  free(od->name);
  od->name=NULL;
}

/* parse 1 option and it's arguments */
Option parse_option_args(char * optargs[], int size) {
  Option op;

  op.name = strdup(optargs[0] + strlen(OPTION_PREFIX));
  op.args = malloc(sizeof(char*) * size - 1);

  for (int i = 0; i < size - 1; i++) {
    op.args[i] = strdup(optargs[i+1]);
  }
  op.argsc = size - 1;

  return op;
}

bool checkrequirements(OptionDef od[], Option opt[], int optsize) {
  Option curopt;
  bool requirementsmet = true;

  // Check that options are unique
  for (int i = 0; i < optsize - 1; i++) {
    for (int j = i+1; j < optsize; j++ ) {
      if(strcmp(opt[i].name, opt[j].name) == 0) {
        fprintf(stderr, "Duplicate option %s\n", opt[i].name);
        requirementsmet = false;
      }
    }  
  } 

  // Check that all options declared as required are present in opt
  for (int i = 0; od[i].name != NULL; i++) {
    if (od[i].requirement == required) {
      curopt = getoption(od[i].name, opt, optsize);
      if (curopt.name == NULL) {
        fprintf(stderr, "Missing option %s\n", od[i].name);
        requirementsmet = false;
      }
    }
  }

  // Unknown options
  for (int i = 0; i < optsize; i++) {
    if (getoptiondef(opt[i].name, od).name == NULL) {
      fprintf(stderr, "Ignoring option %s\n", opt[i].name);
    }
  }

  // Check that all given options have the required number of arguments
  OptionDef curod;
  for (int i = 0; i < optsize; i++) {
    curod = getoptiondef(opt[i].name, od);
    if (curod.name != NULL) {
      switch(curod.nargs) {
        case any:
        break;
        case oneormore:
          if (opt[i].argsc == 0) {
            fprintf(stderr, "Missing arguments for option %s\n", opt[i].name);
            requirementsmet = false;
          }
        break;
        default:
          if (curod.nargs >= 0){
            if (opt[i].argsc < curod.nargs) {
              fprintf(stderr, "Missing arguments for option %s\n", opt[i].name);
              requirementsmet = false;
            } 
            else if (opt[i].argsc > curod.nargs) {
              fprintf(stderr, "Too many arguments for option %s\n", opt[i].name);
              requirementsmet = false;
            }
          } else {
            fprintf(stderr, "Unknown nargs requirement for option %s\n", opt[i].name);
            requirementsmet = false;
          }
        break;
      }
    }
  }

  return requirementsmet;
}

Option getoption(char name[], Option opt[], int optsize) {
  int i;
  for (i = 0; (i < optsize) && (strcmp(name, opt[i].name) != 0); i++);
  if (i < optsize) {
    return opt[i]; 
  } else {
    return nulloption;
  }
}

OptionDef getoptiondef(char name[], OptionDef od[]) {
  int i;
  for (i = 0; (od[i].name != NULL) && (strcmp(name, od[i].name) != 0); i++);
  if (od[i].name != NULL) {
    return od[i];
  } else {
    return nulloptiondef;
  }
}

int parseoptions(OptionDef od[], int argc, char* argv[],  Option ** opts) {
  //int last_opt_pos;
  int optsc = 0;

  *opts = malloc(sizeof(Option) * argc);

  /* parse all options and args into struct options */

  if ((argc > 1) && (strncmp(argv[1],OPTION_PREFIX, 2) != 0)) {
    fprintf(stderr, "Option list must start with an option name, e.g. \"--someoption\".\n");
  }
  int optposition = 1;
  while (optposition < argc) {
    int optsandargssize = 1;
    while ((optposition + optsandargssize < argc) && (strncmp(argv[optposition + optsandargssize],OPTION_PREFIX, 2) != 0)) {
      optsandargssize++;
    }
    (*opts)[optsc] = parse_option_args(&argv[optposition], optsandargssize);
    optposition+=optsandargssize;
    optsc++;
  }

  // for (int i = 0; od[i].name != NULL; i++) {
  //   printf("optdef %d: %s\n", i, od[i].name);
  // }

  // for (int i = 0; i < optsc; i++) {
  //   printf("option %d: %s\n", i, (*opts)[i].name);
  // }

  if(!checkrequirements(od, (*opts), optsc)) {
    return -1;
  }
  
  return optsc;
}
