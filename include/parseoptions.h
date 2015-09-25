#ifndef PARSEOPTIONS_H
#define PARSEOPTIONS_H

#define OPTION_PREFIX "--"

typedef struct option {
  char* name;
  char** args;
  int argsc;
} Option;

extern const Option nulloption;

typedef enum {any=-1, oneormore=-2} nargs_t;
typedef enum {required, optional} optreq_t;

typedef struct optiondef {
    char * name;
    optreq_t requirement;
    nargs_t nargs;
} OptionDef;

extern const OptionDef nulloptiondef;

int parseoptions(OptionDef od[], int argc, char* argv[], Option ** opts);
Option getoption(char name[], Option opt[], int optsize);
OptionDef getoptiondef(char name[], OptionDef od[]);

void freeoption(Option * opt);
void freeoptiondef(OptionDef * od);

#endif
