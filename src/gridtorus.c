#include "grid.h"

unsigned int xy2i(int x, int y) {
  // wrap around when x and y are beyond borders.
  x = x < 0 ? ((x + 1) % xmax) + xmax - 1 : x % xmax;
  y = y < 0 ? ((y + 1) % ymax) + ymax - 1 : y % ymax;
  // x = x < 0 ? x + xmax * (-x / xmax) : x % xmax;
  // y = y < 0 ? y + ymax * (-y / ymax) : y % ymax;
  return xmax * y + x;
}
