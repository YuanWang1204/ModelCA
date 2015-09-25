#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include "bosquet.h"
#include "output.h"

#define Grid_TEST_SIZE_X 10
#define Grid_TEST_SIZE_Y 7

Grid sempty;
Grid s1;

void init_test_Grid() {
  sempty.grid = malloc(sizeof(CellState) * Grid_TEST_SIZE_X * Grid_TEST_SIZE_Y);
  sempty.xmax = Grid_TEST_SIZE_X;
  sempty.ymax = Grid_TEST_SIZE_Y,
  s1.grid = malloc(sizeof(Cell) * Grid_TEST_SIZE_X * Grid_TEST_SIZE_Y);
  s1.xmax = Grid_TEST_SIZE_X;
  s1.ymax = Grid_TEST_SIZE_Y;
}

void clear_Grid() {
  for(int i = 0; i < Grid_TEST_SIZE_X * Grid_TEST_SIZE_Y; i++) {
    s1.grid[i].occupation = -1;
    s1.grid[i].neighbours = 0;
    sempty.grid[i].occupation = -1;
    sempty.grid[i].neighbours = 0;
    s1.grid[i].tmp_occupation = -1;
    s1.grid[i].tmp_neighbours = 0;
    sempty.grid[i].tmp_occupation = -1;
    sempty.grid[i].tmp_neighbours = 0;
  }
}

void reset_Grid() {
  clear_Grid();

  /* put an individual of species 0 at (x,y) = (0,1) */
  s1.grid[Grid_TEST_SIZE_X].occupation = 0;
  /* with it's shadow cast only on it's own cell */
  s1.grid[Grid_TEST_SIZE_X].neighbours = 1;
  /* another one at (x,y)=(xmax - 2, ymax - 1) */
  s1.grid[Grid_TEST_SIZE_X * Grid_TEST_SIZE_Y - 2].occupation = 1;
  s1.grid[Grid_TEST_SIZE_X * Grid_TEST_SIZE_Y - 2].neighbours = 1;
}


void test_output_clusters_dynamics() {
  const double neighbour_radius = 3;
  int addtoX[(int) pow((neighbour_radius * 2 + 1),2)]; //coordinates where to add shadow
  int addtoY[(int) pow((neighbour_radius * 2 + 1),2)];
  int addtoSize;
  int j;

  // test rien ne change
  clear_Grid();
  s1.grid[neighbousCoordinates(0,1, s1.xmax, s1.ymax)].occupation = 1;
  s1.grid[neighbousCoordinates(0,1, s1.xmax, s1.ymax)].tmp_occupation = 1;
  coordsInDisk(0, 1, neighbour_radius, addtoX, addtoY, &addtoSize);
  for(int i = 0; i < addtoSize; i++) {
    j = neighbousCoordinates(addtoX[i], addtoY[i], s1.xmax, s1.ymax);
    s1.grid[j].neighbours += 1;
    s1.grid[j].tmp_neighbours += 1;
  }

  output_clusters_dynamics(&s1,1,1,neighbour_radius);

  // test un arbre apparaît
  clear_Grid();
  s1.grid[neighbousCoordinates(0,1, s1.xmax, s1.ymax)].occupation = 1;
  s1.grid[neighbousCoordinates(0,1, s1.xmax, s1.ymax)].tmp_occupation = 1;
  coordsInDisk(0, 1, neighbour_radius, addtoX, addtoY, &addtoSize);
  for(int i = 0; i < addtoSize; i++) {
    j = neighbousCoordinates(addtoX[i], addtoY[i], s1.xmax, s1.ymax);
    s1.grid[j].neighbours += 1;
    s1.grid[j].tmp_neighbours += 1;
  }

  s1.grid[neighbousCoordinates(0,2, s1.xmax, s1.ymax)].occupation = 1;
  coordsInDisk(0,2,neighbour_radius, addtoX, addtoY, &addtoSize);
  for(int i = 0; i < addtoSize; i++) {
    j = neighbousCoordinates(addtoX[i], addtoY[i], s1.xmax, s1.ymax);
    s1.grid[j].neighbours += 1;
  }

  
  output_grid_trees_ASCII_ART(&s1, 2, 1, true); 

  output_clusters_dynamics(&s1,1,1,neighbour_radius);

  // test un arbre disparait
  clear_Grid();
  s1.grid[neighbousCoordinates(0,1, s1.xmax, s1.ymax)].occupation = 1;
  s1.grid[neighbousCoordinates(0,1, s1.xmax, s1.ymax)].tmp_occupation = 1;
  coordsInDisk(0, 1, neighbour_radius, addtoX, addtoY, &addtoSize);
  for(int i = 0; i < addtoSize; i++) {
    j = neighbousCoordinates(addtoX[i], addtoY[i], s1.xmax, s1.ymax);
    s1.grid[j].neighbours += 1;
    s1.grid[j].tmp_neighbours += 1;
  }

  s1.grid[neighbousCoordinates(0,2, s1.xmax, s1.ymax)].tmp_occupation = 1;
  coordsInDisk(0,2,neighbour_radius, addtoX, addtoY, &addtoSize);
  for(int i = 0; i < addtoSize; i++) {
    j = neighbousCoordinates(addtoX[i], addtoY[i], s1.xmax, s1.ymax);
    s1.grid[j].tmp_neighbours += 1;
  }

  output_clusters_dynamics(&s1,1,1,neighbour_radius);
}

void test_get_neighbours() {
  printf("get_neighbours(s1, 0, 0) == 0\n");
  assert(get_neighbours(s1, 0, 0) == 0);
  printf("get_neighbours(s1, 0, 1) == 1\n");
  assert(get_neighbours(s1, 0, 1) == 1);
  printf("get_neighbours(s1, -2, -1) == 1\n");
  assert(get_neighbours(s1, -2, -1) == 1);
  printf("get_neighbours(s1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y + 1) == 1\n");
  assert(get_neighbours(s1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y + 1) == 1);
}

void test_get_occupation() {
  printf("get_occupation(s1, 0, 0) == %d\n",get_occupation(s1, 0, 0));
  assert(get_occupation(s1, 0, 0) == -1);
  printf("get_occupation(s1, 0, 1) == %d\n",get_occupation(s1, 0, 1) == 0);
  assert(get_occupation(s1, 0, 1) == 0);
  printf("get_occupation(s1, -2, -1) == %d\n",get_occupation(s1, -2, -1) == 1);
  assert(get_occupation(s1, -2, -1) == 1);
  printf("get_occupation(s1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y + 1) == %d\n",get_occupation(s1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y + 1) == 1);
  assert(get_occupation(s1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y + 1) == 0);
}

void test_set_neighbours() {
  set_neighbours(&s1, 1,0,4);
  printf("get_neighbours(s1, 1, 0) == %f\n", get_neighbours(s1, 1, 0));
  assert(get_neighbours(s1, 1, 0) == 4);

  set_neighbours(&s1, Grid_TEST_SIZE_X - 1, Grid_TEST_SIZE_Y, 4);
  printf("get_neighbours(s1, Grid_TEST_SIZE_X - 1, Grid_TEST_SIZE_Y) == 4\n");
  assert(get_neighbours(s1, Grid_TEST_SIZE_X - 1, Grid_TEST_SIZE_Y) == 4);
}

void test_set_occupation() {
  set_occupation(&s1, 1,0,2);
  printf("get_occupation(s1, 1, 0) == 2\n");
  assert(get_occupation(s1, 1, 0) == 2);
  set_occupation(&s1, Grid_TEST_SIZE_X - 1, Grid_TEST_SIZE_Y, 2);
  printf("get_occupation(s1, Grid_TEST_SIZE_X - 1, Grid_TEST_SIZE_Y) == 2\n");
  assert(get_occupation(s1, Grid_TEST_SIZE_X - 1, Grid_TEST_SIZE_Y) == 2);
}

void test_neighbousCoordinates() {
  int x, y;
  x = 3;
  y = 4;
  assert(neighbousCoordinates(0,0, 7, 5) == 0);
  assert(neighbousCoordinates(7,0, 7, 5) == 0);
  assert(neighbousCoordinates(0,5, 7, 5) == 0);
  assert(neighbousCoordinates(7,5, 7, 5) == 0);
  assert(neighbousCoordinates(-1,0, 7, 5) == 6);
  assert(neighbousCoordinates(0,-1, 7, 5) == 28);
  assert(neighbousCoordinates(-1,-1, 7, 5) == 34);
  assert(neighbousCoordinates(-2,-1, 7, 5) == 33);
  assert(neighbousCoordinates(Grid_TEST_SIZE_X - 1, Grid_TEST_SIZE_Y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y) == neighbousCoordinates(-1, -1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y));
  assert(neighbousCoordinates(x, y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y) == neighbousCoordinates(Grid_TEST_SIZE_X + x, Grid_TEST_SIZE_Y + y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y));
  assert(neighbousCoordinates(x, y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y) == neighbousCoordinates(- Grid_TEST_SIZE_X + x, - Grid_TEST_SIZE_Y + y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y));
  assert(neighbousCoordinates(x, y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y) == neighbousCoordinates(2 * Grid_TEST_SIZE_X + x, - Grid_TEST_SIZE_Y + y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y));
  assert(neighbousCoordinates(x, y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y) == neighbousCoordinates(-2 * Grid_TEST_SIZE_X + x, 3 * Grid_TEST_SIZE_Y + y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y));
}

void test_coordsInDisk() {
  int returnX[(int) pow((2 * 2 + 1),2)];
  int returnY[(int) pow((2 * 2 + 1),2)];
  int returnSize;
  printf("positive return values only\n");
  coordsInDisk(4,5,2.1, returnX, returnY, &returnSize);
  assert(returnSize == 13);
  assert(returnX[0] ==  4 && returnY[0] == 3);
  assert(returnX[1] ==  3 && returnY[1] == 4);
  assert(returnX[2] ==  4 && returnY[2] == 4);
  assert(returnX[3] ==  5 && returnY[3] == 4);
  assert(returnX[4] ==  2 && returnY[4] == 5);
  assert(returnX[5] ==  3 && returnY[5] == 5);
  assert(returnX[6] ==  4 && returnY[6] == 5);
  assert(returnX[7] ==  5 && returnY[7] == 5);
  assert(returnX[8] ==  6 && returnY[8] == 5);
  assert(returnX[9] ==  3 && returnY[9] == 6);
  assert(returnX[10] ==  4 && returnY[10] == 6);
  assert(returnX[11] ==  5 && returnY[11] == 6);
  assert(returnX[12] ==  4 && returnY[12] == 7);
  printf("positive and negative return values\n");
  coordsInDisk(0,0,2.001, returnX, returnY, &returnSize);
  assert(returnSize == 13);
  assert(returnX[0] ==  0 && returnY[0] == -2);
  assert(returnX[1] ==  -1 && returnY[1] == -1);
  assert(returnX[2] ==  0 && returnY[2] == -1);
  assert(returnX[3] ==  1 && returnY[3] == -1);
  assert(returnX[4] ==  -2 && returnY[4] == 0);
  assert(returnX[5] ==  -1 && returnY[5] == 0);
  assert(returnX[6] ==  0 && returnY[6] == 0);
  assert(returnX[7] ==  1 && returnY[7] == 0);
  assert(returnX[8] ==  2 && returnY[8] == 0);
  assert(returnX[9] ==  -1 && returnY[9] == 1);
  assert(returnX[10] ==  0 && returnY[10] == 1);
  assert(returnX[11] ==  1 && returnY[11] == 1);
  assert(returnX[12] ==  0 && returnY[12] == 2);
}

void test_add_tree() {
  int add_x = 0;
  int add_y = 0;
  int expected_neighbours[Grid_TEST_SIZE_X * Grid_TEST_SIZE_Y];
  double expected_occupation[Grid_TEST_SIZE_X * Grid_TEST_SIZE_Y];
  double shadow_radius1 = 1.4;

  reset_Grid();

  /* empty Grid */

  for (int x = 0; x < Grid_TEST_SIZE_X; x++) {
    for (int y = 0; y < Grid_TEST_SIZE_Y; y++) {
      expected_neighbours[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 0;
      expected_occupation[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = -1;
    }
  }

  expected_occupation[neighbousCoordinates(add_x,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 0;

  expected_neighbours[neighbousCoordinates(add_x,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1;
  expected_neighbours[neighbousCoordinates(add_x,add_y + 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1;
  expected_neighbours[neighbousCoordinates(add_x,add_y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1;
  expected_neighbours[neighbousCoordinates(add_x - 1,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1;
  //expected_neighbours[neighbousCoordinates(add_x - 1,add_y + 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1;
  //expected_neighbours[neighbousCoordinates(add_x - 1,add_y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1;
  expected_neighbours[neighbousCoordinates(add_x + 1,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1;
  //expected_neighbours[neighbousCoordinates(add_x + 1,add_y + 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1;
  //expected_neighbours[neighbousCoordinates(add_x + 1,add_y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1;

  add_tree(&sempty, add_x, add_y, 0, shadow_radius1);

  printf("adding tree to empty savana, test shadow\n");
  for (int x = 0; x < Grid_TEST_SIZE_X; x++) {
    for (int y = 0; y < Grid_TEST_SIZE_Y; y++) {
      assert(get_neighbours(sempty, x,y) == expected_neighbours[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)]);
    }
  }

  printf("adding tree to empty savana, test occupation\n");
  for (int x = 0; x < Grid_TEST_SIZE_X; x++) {
    for (int y = 0; y < Grid_TEST_SIZE_Y; y++) {
      assert(get_occupation(sempty,x,y) == expected_occupation[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)]);
    }
  }

  /* non-empty Grid */
  reset_Grid();

  for (int x = 0; x < Grid_TEST_SIZE_X; x++) {
    for (int y = 0; y < Grid_TEST_SIZE_Y; y++) {
      expected_neighbours[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = s1.grid[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours;
      expected_occupation[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = s1.grid[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].occupation;
    }
  }

  expected_occupation[neighbousCoordinates(add_x,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 0;

  expected_neighbours[neighbousCoordinates(add_x,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1
    + s1.grid[neighbousCoordinates(add_x,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours;
  expected_neighbours[neighbousCoordinates(add_x,add_y + 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1
    + s1.grid[neighbousCoordinates(add_x,add_y + 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours;
  expected_neighbours[neighbousCoordinates(add_x,add_y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1
    + s1.grid[neighbousCoordinates(add_x,add_y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours;
  expected_neighbours[neighbousCoordinates(add_x - 1,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1
    + s1.grid[neighbousCoordinates(add_x - 1,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours;
  /* expected_neighbours[neighbousCoordinates(add_x - 1,add_y + 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1 */
  /*   + s1.grid[neighbousCoordinates(add_x - 1,add_y + 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours; */
  /* expected_neighbours[neighbousCoordinates(add_x - 1,add_y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1 */
  /*   + s1.grid[neighbousCoordinates(add_x - 1,add_y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours; */
  expected_neighbours[neighbousCoordinates(add_x + 1,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1
    + s1.grid[neighbousCoordinates(add_x + 1,add_y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours;
  /* expected_neighbours[neighbousCoordinates(add_x + 1,add_y + 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1 */
  /*   + s1.grid[neighbousCoordinates(add_x + 1,add_y + 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours; */
  /* expected_neighbours[neighbousCoordinates(add_x + 1,add_y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = 1 */
  /*   + s1.grid[neighbousCoordinates(add_x + 1,add_y - 1, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours; */

  printf("adding tree to non-empty savana, test shadow\n");
  add_tree(&s1, add_x, add_y, 0, shadow_radius1);
  for (int x = 0; x < Grid_TEST_SIZE_X; x++) {
    for (int y = 0; y < Grid_TEST_SIZE_Y; y++) {
      assert(get_neighbours(s1,x,y) == expected_neighbours[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)]);
    }
  }

  printf("adding tree to non-empty savana, test occupation\n");
  add_tree(&s1, 0, 0, 0, shadow_radius1);
  for (int x = 0; x < Grid_TEST_SIZE_X; x++) {
    for (int y = 0; y < Grid_TEST_SIZE_Y; y++) {
      assert(get_occupation(s1,x,y) == expected_occupation[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)]);
    }
  }
}

void test_remove_tree() {
  int expected_neighbours[Grid_TEST_SIZE_X * Grid_TEST_SIZE_Y];
  double expected_occupation[Grid_TEST_SIZE_X * Grid_TEST_SIZE_Y];
  double shadow_radius1 = 1.4;

  reset_Grid();

  /* non-empty Grid */

  for (int x = 0; x < Grid_TEST_SIZE_X; x++) {
    for (int y = 0; y < Grid_TEST_SIZE_Y; y++) {
      expected_neighbours[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = s1.grid[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].neighbours;
      expected_occupation[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)] = s1.grid[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)].occupation;
    }
  }

  add_tree(&s1, 0, 0, 0, shadow_radius1);
  remove_tree(&s1,0,0,shadow_radius1);

  printf("removing tree from non-empty Grid, test shadow\n");
  for (int x = 0; x < Grid_TEST_SIZE_X; x++) {
    for (int y = 0; y < Grid_TEST_SIZE_Y; y++) {
      assert(get_neighbours(s1,x,y) == expected_neighbours[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)]);
    }
  }

  printf("removing tree from non-empty Grid, test occupation\n");
  for (int x = 0; x < Grid_TEST_SIZE_X; x++) {
    for (int y = 0; y < Grid_TEST_SIZE_Y; y++) {
      assert(get_occupation(s1,x,y) == expected_occupation[neighbousCoordinates(x,y, Grid_TEST_SIZE_X, Grid_TEST_SIZE_Y)]);
    }
  }
}

void test_p_recrutement() {
  //différentes espèces de chaque type pour tester les bornes ntr1 (floats et int)
  int ntestspecies = 3;
  static Species sp_type1 [] = {
    {.pr1=0.1,.prs=0.2,.pm1=0.3,.pm2=0.2,.ntr1=1,.ntr2=6,.ntm1=2,.ntm2=4,
     .p_recrutement = &p_recrutement_type1,
     .init_p_recrutement_table = &init_p_recrutement_table_type1},
    {.pr1=0.1,.prs=0.2,.pm1=0.3,.pm2=0.2,.ntr1=1.1,.ntr2=6,.ntm1=2,.ntm2=4,
     .p_recrutement = &p_recrutement_type1,
     .init_p_recrutement_table = &init_p_recrutement_table_type1},
    {.pr1=0.1,.prs=0.2,.pm1=0.3,.pm2=0.2,.ntr1=1.1,.ntr2=5.9,.ntm1=2,.ntm2=4,
     .p_recrutement = &p_recrutement_type1,
     .init_p_recrutement_table = &init_p_recrutement_table_type1}
  };
  sp_type1[0].pr2 = sp_type1[0].pr1 + sp_type1[0].prs * (sp_type1[0].ntr2 - sp_type1[0].ntr1);
  sp_type1[1].pr2 = sp_type1[1].pr1 + sp_type1[1].prs * (sp_type1[1].ntr2 - sp_type1[1].ntr1);
  sp_type1[2].pr2 = sp_type1[2].pr1 + sp_type1[2].prs * (sp_type1[2].ntr2 - sp_type1[2].ntr1);
  static Species sp_type2 [] = {
    {.pr1=0.1,.pr2=0.2,.pr3=0,.pm1=0.3,.pm2=0.2,.ntr1=1,.ntr2=6, .ntr3 = 8,.ntm1=2,.ntm2=4,
     .p_recrutement = &p_recrutement_type2,
     .init_p_recrutement_table = &init_p_recrutement_table_type2},
    {.pr1=0.1,.pr2=0.2,.pr3=0,.pm1=0.3,.pm2=0.2,.ntr1=1.1,.ntr2=6, .ntr3 = 8.2,.ntm1=2,.ntm2=4,
     .p_recrutement = &p_recrutement_type2,
     .init_p_recrutement_table = &init_p_recrutement_table_type2},
    {.pr1=0.1,.pr2=0.2,.pr3=0,.pm1=0.3,.pm2=0.2,.ntr1=1.1,.ntr2=5.4, .ntr3 = 8,.ntm1=2,.ntm2=4,
     .p_recrutement = &p_recrutement_type2,
     .init_p_recrutement_table = &init_p_recrutement_table_type2}
  };
  sp_type2[0].prs = (sp_type2[0].pr2 - sp_type2[0].pr1) / ((double) (sp_type2[0].ntr2 - sp_type2[0].ntr1));
  sp_type2[1].prs = (sp_type2[1].pr2 - sp_type2[1].pr1) / ((double) (sp_type2[1].ntr2 - sp_type2[1].ntr1));
  sp_type2[2].prs = (sp_type2[2].pr2 - sp_type2[2].pr1) / ((double) (sp_type2[2].ntr2 - sp_type2[2].ntr1));

  for(int i = 0; i < ntestspecies; i++) {
    init_species(&sp_type1[i], 1);
    init_species(&sp_type2[i], 1);

    assert((*sp_type1[i].p_recrutement)(0, &sp_type1[i]) == sp_type1[i].pr1);
    assert((*sp_type1[i].p_recrutement)(1, &sp_type1[i]) == sp_type1[i].pr1);
    assert((*sp_type1[i].p_recrutement)(2, &sp_type1[i]) == sp_type1[i].pr1 + (2 - sp_type1[i].ntr1) * sp_type1[i].prs);
    assert((*sp_type1[i].p_recrutement)(3, &sp_type1[i]) == sp_type1[i].pr1 + (3 - sp_type1[i].ntr1) * sp_type1[i].prs);
    assert((*sp_type1[i].p_recrutement)(6, &sp_type1[i]) == sp_type1[i].pr2);
    assert((*sp_type1[i].p_recrutement)(7, &sp_type1[i]) == sp_type1[i].pr2);

    assert((*sp_type2[i].p_recrutement)(1, &sp_type2[i]) == sp_type2[i].pr1);
    assert((*sp_type2[i].p_recrutement)(2, &sp_type2[i]) == sp_type2[i].pr1 + (2 - sp_type2[i].ntr1) * (sp_type2[i].pr2 - sp_type2[i].pr1)/((double) sp_type2[i].ntr2 - sp_type2[i].ntr1));
    assert((*sp_type2[i].p_recrutement)(3, &sp_type2[i]) == sp_type2[i].pr1 + (3 - sp_type2[i].ntr1) * (sp_type2[i].pr2 - sp_type2[i].pr1)/((double) sp_type2[i].ntr2 - sp_type2[i].ntr1));
    assert((*sp_type2[i].p_recrutement)(6, &sp_type2[i]) == sp_type2[i].pr1 + (6 - sp_type2[i].ntr1) * (sp_type2[i].pr2 - sp_type2[i].pr1)/((double) sp_type2[i].ntr2 - sp_type2[i].ntr1)
           ||
           (*sp_type2[i].p_recrutement)(6, &sp_type2[i]) ==  sp_type2[i].pr2 + (6 - sp_type2[i].ntr2) * (sp_type2[i].pr3 - sp_type2[i].pr2) / ((double) (sp_type2[i].ntr3 - sp_type2[i].ntr2)));
    assert((*sp_type2[i].p_recrutement)(7, &sp_type2[i]) == sp_type2[i].pr2 + (7 - sp_type2[i].ntr2) * (sp_type2[i].pr3 - sp_type2[i].pr2) / ((double) (sp_type2[i].ntr3 - sp_type2[i].ntr2)));
    assert((*sp_type2[i].p_recrutement)(ceil(sp_type2[i].ntr3), &sp_type2[i]) == sp_type2[i].pr3);

    assert(p_mortality(0, &sp_type1[i]) == sp_type1[i].pm1);
    assert(p_mortality(1, &sp_type1[i]) == sp_type1[i].pm1);
    assert(p_mortality(2, &sp_type1[i]) == sp_type1[i].pm1);
    assert(p_mortality(3, &sp_type1[i]) == sp_type1[i].pm1 + (sp_type1[i].pm2 - sp_type1[i].pm1)/(float)(sp_type1[i].ntm2 - sp_type1[i].ntm1));
    assert(p_mortality(4, &sp_type1[i]) == sp_type1[i].pm1 + 2 * (sp_type1[i].pm2 - sp_type1[i].pm1)/(float)(sp_type1[i].ntm2 - sp_type1[i].ntm1));
    assert(p_mortality(5, &sp_type1[i]) == sp_type1[i].pm2);

    assert(p_mortality(0, &sp_type2[i]) == sp_type2[i].pm1);
    assert(p_mortality(1, &sp_type2[i]) == sp_type2[i].pm1);
    assert(p_mortality(2, &sp_type2[i]) == sp_type2[i].pm1);
    assert(p_mortality(3, &sp_type2[i]) == sp_type2[i].pm1 + (sp_type2[i].pm2 - sp_type2[i].pm1)/(float)(sp_type2[i].ntm2 - sp_type2[i].ntm1));
    assert(p_mortality(4, &sp_type2[i]) == sp_type2[i].pm1 + 2 * (sp_type2[i].pm2 - sp_type2[i].pm1)/(float)(sp_type2[i].ntm2 - sp_type2[i].ntm1));
    assert(p_mortality(5, &sp_type1[i]) == sp_type2[i].pm2);
  }

}

int main() {
  init_test_Grid();
  reset_Grid();

  /* srand48(time(NULL)); */
  time(NULL);

  printf("test_neighbousCoordinates\n");
  test_neighbousCoordinates();

  printf("test_coordsInDisk\n");
  test_coordsInDisk();

  printf("test_get_neighbours\n");
  test_get_neighbours();

  printf("test_get_occupation\n");
  test_get_occupation();

  printf("test_set_neighbours\n");
  test_set_neighbours();

  printf("test_set_occupation\n");
  test_set_occupation();

  printf("test_add_tree\n");
  test_add_tree();

  printf("test_remove_tree\n");
  test_remove_tree();

  printf("test_p_recrutement\n");
  test_p_recrutement();

  printf("test_output_clusters_dynamics\n");
  test_output_clusters_dynamics();

  return EXIT_SUCCESS;
}
