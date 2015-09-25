#ifndef CELL_H
#define CELL_H

#include <stdbool.h>

//typedef int CellState;
typedef struct _CellState CellState;

CellState * malloccellstate();
void freecellstate(CellState * cs); //free memory associated with pointer

void copycellstate(CellState * cs1, CellState * cs2);

#endif
