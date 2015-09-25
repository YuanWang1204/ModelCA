#ifndef INITGRID_H
#define INITGRID_H

extern const OptionDef odinitgrid [];

int initparamsinitgrid(Option * opts, int optsc);
void freeparamsinitgrid();

void initgrid(Grid g);

#endif
