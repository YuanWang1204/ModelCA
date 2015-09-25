#ifndef GRID_H
#define GRID_H

#include "cell.h"
#include "parseoptions.h"


/* A Grid is coded as a 1D array of Cells. The rows of 2D grid
 * representing the Grid are concatenated into a 1D array:
 * (x, y) -> y*ymax + x */

typedef CellState ** Grid;

extern int xmax;
extern int ymax;

extern const OptionDef odgrid [];
int initparamsgrid(Option * opts, int optsc);

unsigned int xy2i(int x, int y);

CellState * cell(Grid g, int x, int y);

//void getgridcell(Grid g, int x, int y);

void updategrid(Grid g, int x, int y, unsigned short int newstate);

Grid mallocgrid();
void freegrid(Grid * g);

void copygrid(const Grid g1, Grid g2);

#endif /*GRID_H*/
