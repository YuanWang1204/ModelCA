#include "output.h"
#include "bosquet.h"
#include <stdio.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_statistics.h>
#include <math.h>
#include <cairo.h>

void output_header(char output_mode,int nspecies) {
  switch(output_mode) {
  case 'p':
    printf("pop0");
    for(int i = 1; i < nspecies; i++) {
      printf(",pop%d", i);
    }
    printf(",neighb_avg,neighb_min,neighb_quantile_1,neighb_quartile_fst,neighb_quartile_snd,neighb_quartile_trd,neighb_quantile_9,neighb_max");
    printf(",cluster_count,clustsize_avg,clustsize_min,clustsize_quantile_1,clustsize_quartile_fst,clustsize_quartile_snd,clustsize_quartile_trd,clustsize_quantile_9,clustsize_max");
    printf(",couvert");
    printf("\n");
    break;
  case 'c':
    printf("pop0");
    for(int i = 1; i < nspecies; i++) {
      printf(",pop%d", i);
    }
    printf(",iteration,size1,size2,trees_count1,trees_count2\n");
    break;
  /* case 'd': */
  /*   printf("size1,size2,trees_count1,trees_count2\n"); */
  /*   break; */
  case 'f':
    printf("[");
    break;
  case 'g':
    printf("species,x,y\n");
    break;
  case 'n':
    break;
  case 'd':
    break;
  }
}

void output_footer(char output_mode, Grid * s, int nspecies, int iteration, double neighbour_radius){
  switch(output_mode) {
  case 'p':
    break;
  case 'c':
    break;
  /* case 'd': */
  /*   printf("size1,size2,trees_count1,trees_count2\n"); */
  /*   break; */
  case 'f':
    printf("\n]");
    break;
  case 'n':
    break;
  case 'd':
    break;
  }
}

void output(char output_mode, Grid * s, int nspecies, int iteration, double neighbour_radius) {
  switch(output_mode) {
  case 'a':
    output_grid_trees_ASCII_ART(s, 2, 0, true);
    break;
  case 'p':
    output_population_csv(s, nspecies);
    break;
  case 'f':
    output_full_matrix(s, nspecies);
    break;
  case 'g':
    output_full_coords(s, nspecies);
    break;
  case 'c':
    output_clusters_dynamics(s, nspecies,iteration,neighbour_radius);
    break;
  /* case 'd' */
  /*   output_clusters_dynamics2(s, nspecies); */
  /*   break; */
  case 'n':
    break;
  case 'd':
    output_density(s);
    break;
  default:
    break;
  }
}
int blabla = 0;
unsigned int propagate_color(Grid * s, unsigned int colors[], size_t size, int x, int y, const unsigned int color) {
  /* x = x < 0 ? (x % s->xmax) + s->xmax : x % s->xmax; */
  /* y = y < 0 ? (y % s->ymax) + s->ymax : y % s->ymax; */
  size_t i = neighbousCoordinates(x,y,s->xmax,s->ymax);
  if((s->grid[i].occupation >= 0 || s->grid[i].neighbours > 0) && colors[i] != color){
    colors[i] = color; 

    return propagate_color(s, colors, size, x > 0 ? x-1 : s->xmax - 1, y > 0 ? y-1 : s->ymax - 1,color)
         + propagate_color(s, colors, size, x > 0 ? x-1 : s->xmax - 1, y                        ,color)
         + propagate_color(s, colors, size, x > 0 ? x-1 : s->xmax - 1, y < s->ymax - 1 ? y+1 : 0,color)
         + propagate_color(s, colors, size, x                        , y > 0 ? y-1 : s->ymax - 1,color)
         + propagate_color(s, colors, size, x                        , y < s->ymax - 1 ? y+1 : 0,color)
         + propagate_color(s, colors, size, x < s->xmax - 1 ? x+1 : 0, y > 0 ? y-1 : s->ymax - 1,color)
         + propagate_color(s, colors, size, x < s->xmax - 1 ? x+1 : 0, y                        ,color)
         + propagate_color(s, colors, size, x < s->xmax - 1 ? x+1 : 0, y < s->ymax - 1 ? y+1 : 0,color)
         + 1;

  } else {
    return 0;
  }
}

unsigned int propagate_color_tmp(Grid * s, unsigned int colors[], size_t size, int x, int y, const unsigned int color) {
  size_t i = neighbousCoordinates(x,y,s->xmax,s->ymax);
  if((s->grid[i].tmp_occupation >= 0 || s->grid[i].tmp_neighbours > 0) && colors[i] != color){
    colors[i] = color; 

    return propagate_color_tmp(s, colors, size, x > 0 ? x-1 : s->xmax - 1, y > 0 ? y-1 : s->ymax - 1,color)
         + propagate_color_tmp(s, colors, size, x > 0 ? x-1 : s->xmax - 1, y                        ,color)
         + propagate_color_tmp(s, colors, size, x > 0 ? x-1 : s->xmax - 1, y < s->ymax - 1 ? y+1 : 0,color)
         + propagate_color_tmp(s, colors, size, x                        , y > 0 ? y-1 : s->ymax - 1,color)
         + propagate_color_tmp(s, colors, size, x                        , y < s->ymax - 1 ? y+1 : 0,color)
         + propagate_color_tmp(s, colors, size, x < s->xmax - 1 ? x+1 : 0, y > 0 ? y-1 : s->ymax - 1,color)
         + propagate_color_tmp(s, colors, size, x < s->xmax - 1 ? x+1 : 0, y                        ,color)
         + propagate_color_tmp(s, colors, size, x < s->xmax - 1 ? x+1 : 0, y < s->ymax - 1 ? y+1 : 0,color)
         + 1;

  } else {
    return 0;
  }
}

unsigned int get_cluster_size(Grid * s, int x, int y) {
  /* x = x < 0 ? (x % s->xmax) + s->xmax : x % s->xmax; */
  /* y = y < 0 ? (y % s->ymax) + s->ymax : y % s->ymax; */
  size_t i = neighbousCoordinates(x,y,s->xmax,s->ymax);
  if(s->grid[i].occupation >= 0 || s->grid[i].neighbours > 0){

    return get_cluster_size(s, x > 0 ? x-1 : s->xmax - 1, y > 0 ? y-1 : s->ymax - 1)
         + get_cluster_size(s, x > 0 ? x-1 : s->xmax - 1, y                        )
         + get_cluster_size(s, x > 0 ? x-1 : s->xmax - 1, y < s->ymax - 1 ? y+1 : 0)
         + get_cluster_size(s, x                        , y > 0 ? y-1 : s->ymax - 1)
         + get_cluster_size(s, x                        , y < s->ymax - 1 ? y+1 : 0)
         + get_cluster_size(s, x < s->xmax - 1 ? x+1 : 0, y > 0 ? y-1 : s->ymax - 1)
         + get_cluster_size(s, x < s->xmax - 1 ? x+1 : 0, y                        )
         + get_cluster_size(s, x < s->xmax - 1 ? x+1 : 0, y < s->ymax - 1 ? y+1 : 0)
         + 1;

  } else {
    return 0;
  }
}

unsigned int get_cluster_size_tmp(Grid * s, int x, int y) {
  /* x = x < 0 ? (x % s->xmax) + s->xmax : x % s->xmax; */
  /* y = y < 0 ? (y % s->ymax) + s->ymax : y % s->ymax; */
  size_t i = neighbousCoordinates(x,y,s->xmax,s->ymax);
  if(s->grid[i].tmp_occupation >= 0 || s->grid[i].tmp_neighbours > 0){

    return get_cluster_size_tmp(s, x > 0 ? x-1 : s->xmax - 1, y > 0 ? y-1 : s->ymax - 1)
         + get_cluster_size_tmp(s, x > 0 ? x-1 : s->xmax - 1, y                        )
         + get_cluster_size_tmp(s, x > 0 ? x-1 : s->xmax - 1, y < s->ymax - 1 ? y+1 : 0)
         + get_cluster_size_tmp(s, x                        , y > 0 ? y-1 : s->ymax - 1)
         + get_cluster_size_tmp(s, x                        , y < s->ymax - 1 ? y+1 : 0)
         + get_cluster_size_tmp(s, x < s->xmax - 1 ? x+1 : 0, y > 0 ? y-1 : s->ymax - 1)
         + get_cluster_size_tmp(s, x < s->xmax - 1 ? x+1 : 0, y                        )
         + get_cluster_size_tmp(s, x < s->xmax - 1 ? x+1 : 0, y < s->ymax - 1 ? y+1 : 0)
         + 1;

  } else {
    return 0;
  }
}

void output_density(Grid *s) {
  size_t savsize = s->xmax * s->ymax;

  int count_total = 0;

  for(int i = 0; i < savsize; i++) {
    if(s->grid[i].occupation >= 0) {
      count_total++;
    }
  }

  printf("%f\n", count_total / (float)savsize);
}

void output_population_csv(Grid *s, int nspecies) {

  size_t savsize = s->xmax * s->ymax;

  /* Population count */
  int count[nspecies];
  int count_total = 0;
  int couvert = 0;

  for(int i = 0; i < nspecies; i++) {
    count[i] = 0;
  }

  for(int i = 0; i < savsize; i++) {
    if(s->grid[i].occupation >= 0) {
      /* Comptage individus par espèce */
      count[s->grid[i].occupation]++;
      count_total++;
    }
    if(s->grid[i].neighbours > 0 || s->grid[i].occupation >= 0) {
      couvert++;
    }
  }


  /* Number of neighbours per cell */
  /* int neighbmin = s->grid[0].neighbours; */
  /* int neighbmax = s->grid[0].neighbours; */
  
  /* /\* first get the min and the max *\/ */
  /* for(int i = 0; i < savsize; i++) { */
  /*   min = s->grid[i].neighbours < neighbmin ?  s->grid[i].neighbours : neighbmin; */
  /*   max = s->grid[i].neighbours > neighbmax ?  neighbmax : s->grid[i].neighbours ; */
  /* } */

  /* /\* Histogram *\/ */
  /* int neighbhisto[neighbmax + 1]; */
  /* for(int i = 0; i < neighbmax; i++) { */
  /*   neighbhisto = 0; */
  /* } */

  /* for(int i = 0; i < s->xmax * s->ymax; i++) { */
  /*   neighbhisto[s->grid[i].neighbours]++; */
  /* } */

  /* /\* Use the histogram to determine the quartiles (this way, no need to sort */
  /*    the array s->grid by neighbours) *\/ */


  /* number of neighbours for each tree */
  int neighbours[count_total];
  int j = 0;
  for(int i = 0; i < savsize; i++) {
    if(s->grid[i].occupation >= 0) {
      neighbours[j] = s->grid[i].neighbours;
      j++;
    }
  }
  
  double neighb_avg = gsl_stats_int_mean(neighbours, 1, count_total);
  gsl_sort_int(neighbours, 1, count_total);
  int neighb_min = neighbours[0];
  int neighb_max = neighbours[count_total - 1];
  double neighb_quartile_fst = gsl_stats_int_quantile_from_sorted_data(neighbours, 1, count_total, 0.25);
  double neighb_quartile_snd = gsl_stats_int_quantile_from_sorted_data(neighbours, 1, count_total, 0.50);
  double neighb_quartile_trd = gsl_stats_int_quantile_from_sorted_data(neighbours, 1, count_total, 0.75);
  double neighb_quantile_1 = gsl_stats_int_quantile_from_sorted_data(neighbours, 1, count_total, 0.1);
  double neighb_quantile_9 = gsl_stats_int_quantile_from_sorted_data(neighbours, 1, count_total, 0.9);
  

  /* clusters size */
  unsigned int colors[savsize];
  unsigned int cluster_count = 0;
  unsigned int clusters_size[savsize]; /* il y a strictement moins
                                              de savsize clusters */
  for(int i = 0; i < savsize; i++) {
    colors[i] = 0;
    clusters_size[i] = 0;
  }
  // pour chaque cellule
  int xy_i;
  for(int x = 0; x < s->xmax; x++) {
    for(int y = 0; y < s->ymax; y++) {
      // dès qu'on rencontre une cellule sans couleur mais avec des
      // voisins, on propage la couleur
      xy_i = neighbousCoordinates(x,y,s->xmax,s->ymax);
      if(colors[xy_i] == 0 && (s->grid[xy_i].occupation >= 0 || s->grid[xy_i].neighbours > 0)) {
        cluster_count++;
        clusters_size[cluster_count -1] = propagate_color(s,colors,savsize,x,y,cluster_count);
      }
    }
  }

  double clustsize_avg;
  int clustsize_min;
  int clustsize_max;
  double clustsize_quartile_fst;
  double clustsize_quartile_snd;
  double clustsize_quartile_trd;
  double clustsize_quantile_1;
  double clustsize_quantile_9;
  if(cluster_count > 0) {
    clustsize_avg = gsl_stats_uint_mean(clusters_size,1,cluster_count);
    gsl_sort_uint(clusters_size,1,cluster_count);
    clustsize_min = clusters_size[0];
    clustsize_max = clusters_size[cluster_count - 1];
    clustsize_quartile_fst = gsl_stats_uint_quantile_from_sorted_data(clusters_size, 1, cluster_count, 0.25);
    clustsize_quartile_snd = gsl_stats_uint_quantile_from_sorted_data(clusters_size, 1, cluster_count, 0.50);
    clustsize_quartile_trd = gsl_stats_uint_quantile_from_sorted_data(clusters_size, 1, cluster_count, 0.75);
    clustsize_quantile_1 = gsl_stats_uint_quantile_from_sorted_data(clusters_size, 1, cluster_count, 0.1);
    clustsize_quantile_9 = gsl_stats_uint_quantile_from_sorted_data(clusters_size, 1, cluster_count, 0.9);
  } else {
    clustsize_avg = 0;
    clustsize_min = 0;
    clustsize_max = 0;
    clustsize_quartile_fst = 0;
    clustsize_quartile_snd = 0;
    clustsize_quartile_trd = 0;
    clustsize_quantile_1 = 0;
    clustsize_quantile_9 = 0;
  }

  
  printf("%d", count[0]);
  for(int i = 1; i < nspecies; i++) {
    printf(",%d", count[i]);
  }
  printf(",%f,%d,%f,%f,%f,%f,%f,%d",neighb_avg,neighb_min,neighb_quantile_1,neighb_quartile_fst,neighb_quartile_snd,neighb_quartile_trd,neighb_quantile_9,neighb_max);
  /*cluster_count gives the number of clusters*/
  printf(",%d,%f,%d,%f,%f,%f,%f,%f,%d",cluster_count,clustsize_avg,clustsize_min,clustsize_quantile_1,clustsize_quartile_fst,clustsize_quartile_snd,clustsize_quartile_trd,clustsize_quantile_9,clustsize_max);
  printf(",%d",couvert);
  printf("\n");
}

void output_full_matrix(Grid *s, int nspecies) {
  int xy_i = 0;

  printf("\n[");
  for (int x = 0; x < s->xmax; x++) {
    if (x > 0) { printf(","); }
    printf("\n[");
    for (int y = 0; y < s->ymax; y++) {
      xy_i = neighbousCoordinates(x,y,s->xmax,s->ymax);
      if (y > 0) { printf(","); }
      printf("%d",s->grid[xy_i].occupation);
    }
    printf("]");
  }
}

void output_full_coords(Grid *s, int nspecies) {
  int xy_i = 0;

  for (int x = 0; x < s->xmax; x++) {
    for (int y = 0; y < s->ymax; y++) {
      xy_i = neighbousCoordinates(x,y,s->xmax,s->ymax);
      if (s->grid[xy_i].occupation >= 0){
        printf("%d,%d,%d\n",s->grid[xy_i].occupation,x,y);
      }
    }
  }
  printf("\n");
}

void output_clusters_dynamics(Grid *s, int nspecies, int iteration, double neighbour_radius) {
  size_t savsize = s->xmax * s->ymax;

  unsigned int colors[savsize];
  unsigned int cluster_count = 0;
  unsigned int cluster_size1[savsize]; /* il y a forcément moins
                                          de savsize clusters */

  for(int i = 0; i < savsize; i++) {
    colors[i] = 0;
    cluster_size1[i] = 0;
  }
  
  /* Population count */
  int count[nspecies];

  for(int i = 0; i < nspecies; i++) {
    count[i] = 0;
  }

  for(int i = 0; i < savsize; i++) {
    if(s->grid[i].occupation >= 0) {
      /* Comptage individus par espèce */
      count[s->grid[i].occupation]++;
    } else {

    }
  }

  // chercher les clusters à l'instant précédent
  int xy_i;
  for(int x = 0; x < s->xmax; x++) {
    for(int y = 0; y < s->ymax; y++) {
      // dès qu'on rencontre une cellule sans couleur mais avec des
      // voisins, on propage la couleur
      xy_i = neighbousCoordinates(x,y,s->xmax,s->ymax);
      if(colors[xy_i] == 0 && (s->grid[xy_i].tmp_occupation >= 0 || s->grid[xy_i].tmp_neighbours > 0)) {
        cluster_count++;
        cluster_size1[cluster_count -1] = propagate_color_tmp(s,colors,savsize,x,y,cluster_count);
      }
    }
  }

  int cluster_size2[cluster_count]; //number of cells in cluster at current timestep
  int trees_count1[cluster_count]; //number of cells in cluster at last timestep
  int trees_count2[cluster_count]; //number of cells in cluster at current timestep
  for(int i = 0; i < cluster_count; i++) {
    cluster_size2[i] = cluster_size1[i];
    trees_count1[i] = 0;
    trees_count2[i] = 0;
  }

  int coords_shadow_x[(int) pow((neighbour_radius * 2 + 1),2)];
  int coords_shadow_y[(int) pow((neighbour_radius * 2 + 1),2)];
  int coords_shadow_size;
  int xy_k;
  
  for(int x = 0; x < s->xmax; x++) {
    for(int y = 0; y < s->ymax; y++) {
      xy_i = neighbousCoordinates(x,y,s->xmax,s->ymax);
      if(colors[xy_i] > 0) {
        //compter les arbres à l'instant précédent
        if(s->grid[xy_i].tmp_occupation > -1) {
          trees_count1[colors[xy_i]-1]++;
        }

        // compter le nombre d'arbres à l'instant courant et
        // calculer la nouvelle superficie (nombre de cellules)

        // Si il y a un arbre à l'isntant courant sur la case (x,y)
        if(s->grid[xy_i].occupation > -1) {
          trees_count2[colors[xy_i]-1]++;
          // si c'est un nouvel arbre
          if(s->grid[xy_i].tmp_occupation == -1) {
            //regarder les cases où il projette de l'ombre, et les
            //ajouter à la superficie courante si elles n'étaient pas
            //déjà dans le bosquet à l'instant précédent.
            coordsInDisk(x,y,neighbour_radius, coords_shadow_x, coords_shadow_y, &coords_shadow_size);
            for(int k = 0; k < coords_shadow_size; k++) {
              xy_k = neighbousCoordinates(coords_shadow_x[k],coords_shadow_y[k],s->xmax,s->ymax);
              if(colors[xy_k] != colors[xy_i]) {
                cluster_size2[colors[xy_i] - 1]++;
              }
            }
          }
        } else { //Il n'y a pas d'arbre à l'isntant courant sur cette case
          //si il y avait un arbre avant
          if(s->grid[xy_i].tmp_occupation > -1) {
            //regarder les cases où il projetait de l'ombre, et
            //enlever de la superficie courante celles qui n'avaient
            //qu'un seul voisin.
            coordsInDisk(x,y,neighbour_radius, coords_shadow_x, coords_shadow_y, &coords_shadow_size);
            for(int k = 0; k < coords_shadow_size; k++) {
              xy_k = neighbousCoordinates(coords_shadow_x[k],coords_shadow_y[k],s->xmax,s->ymax);
              if(coords_shadow_x[k] == x && coords_shadow_y[k] == y) {
                // si la case de l'arbre disparu lui-même n'est couverte par
                // aucun autre arbre, on l'enlève
                if(s->grid[xy_k].tmp_neighbours == 0)  {
                  cluster_size2[colors[xy_i] - 1]--;
                }
              } else {
                // on enlève toutes les autres cases qui étaient
                // couverte par cet arbre et aucun autre
                if(s->grid[xy_k].tmp_neighbours == 1 && s->grid[xy_k].tmp_occupation == -1) {
                  cluster_size2[colors[xy_i] - 1]--;
                }
              }
            }
          }
        }
      }
    }
  }

  for(int i = 0; i < cluster_count; i++){
    //printf("size1,size2,trees_count1,trees_count2");
    printf("%d", count[0]);
    for(int i = 1; i < nspecies; i++) {
      printf(",%d", count[i]);
    }
    printf(",%d,%d,%d,%d,%d\n",iteration,cluster_size1[i],cluster_size2[i],trees_count1[i],trees_count2[i]);
  }
}

/* void output_clusters_dynamics2(Grid *s, int nspecies) { */
/*   size_t savsize = s->xmax * s->ymax; */

/*   unsigned int colors1[savsize]; */
/*   unsigned int colors2[savsize]; */
/*   unsigned int cluster_count1 = 0; */
/*   unsigned int cluster_count2 = 0; */
/*   unsigned int cluster_size1[savsize]; /\* il y a forcément moins*\/ */
/*   unsigned int cluster_size2[savsize]; /\* il y a forcément moins */
/*                                           de savsize clusters *\/ */
/*   for(int i = 0; i < savsize; i++) { */
/*     colors1[i] = 0; */
/*     cluster_size1[i] = 0; */
/*     colors2[i] = 0; */
/*     cluster_size2[i] = 0; */
/*   } */

/*   // chercher les clusters à l'instant précédent et à l'instant courant */
/*   int xy_i; */
/*   for(int x = 0; x < s->xmax; x++) { */
/*     for(int y = 0; y < s->ymax; y++) { */
/*       // dès qu'on rencontre une cellule sans couleur mais avec des */
/*       // voisins, on propage la couleur */
/*       xy_i = neighbousCoordinates(x,y,s->xmax,s->ymax); */
/*       if(colors1[xy_i] == 0 && s->grid[xy_i].tmp_neighbours > 0) { */
/*         cluster_count1++; */
/*         cluster_size1[cluster_count1 -1] = propagate_color_tmp(s,colors1,savsize,x,y,cluster_count1); */
/*       } */
/*       if(colors2[xy_i] == 0 && s->grid[xy_i].neighbours > 0) { */
/*         cluster_count2++; */
/*         cluster_size2[cluster_count2 -1] = propagate_color(s,colors2,savsize,x,y,cluster_count2); */
/*       } */
/*     } */
/*   } */

/*   int trees_count1[cluster_count1]; //number of cells in cluster at last timestep */
/*   int trees_count2[cluster_count2]; //number of cells in cluster at current timestep */
/*   for(int i = 0; i < cluster_count1; i++) { */
/*     trees_count1[i] = 0; */
/*   } */
/*   for(int i = 0; i < cluster_count2; i++) { */
/*     trees_count2[i] = 0; */
/*   } */

/*   //compter les arbres */
/*   for(int x = 0; x < s->xmax; x++) { */
/*     for(int y = 0; y < s->ymax; y++) { */
/*       xy_i = neighbousCoordinates(x,y,s->xmax,s->ymax); */
/*       if(colors1[xy_i] > 0) { */
/*         //compter les arbres à l'instant précédent */
/*         if(s->grid[xy_i].tmp_occupation > -1) { */
/*           trees_count1[colors1[xy_i]-1]++; */
/*         } */
/*         //compter les arbres à l'instant courant */
/*         if(s->grid[xy_i].occupation > -1) { */
/*           trees_count2[colors2[xy_i]-1]++; */
/*         } */
/*       } */
/*     } */
/*   } */

/*   //etablir la correspondance entre les bosquets à t-1 et t */
/*   bool corresp [cluster_count1][cluster_count2]; */
/*   for (int i = 0; i < cluster_count1; i++) { */
/*     for (int j = 0; j < cluster_count2; j++) { */
/*       corresp[i][j] = false; */
/*     } */
/*   } */
  
/*   for(int x = 0; x < s->xmax; x++) { */
/*     for(int y = 0; y < s->ymax; y++) { */
/*       xy_i = neighbousCoordinates(x,y,s->xmax,s->ymax); */
/*       if(s->grid[xy_i].tmp_occupation > -1 && s->grid[xy_i].occupation > -1 ) { */
/*         corresp[colors1[xy_i] - 1][colors2[xy_i] - 1] = true; */
/*       } */
/*     } */
/*   } */

/*   for (int i = 0; i < cluster_count1; i++) { */
/*     for (int j = 0; j < cluster_count2; j++) { */
/*       //printf("size1,size2,trees_count1,trees_count2"); */
/*       if(corresp[i][j]) { */
/*         printf("%d,%d,%d,%d\n",cluster_size1[i],cluster_size2[i],trees_count1[j],trees_count2[j]); */
/*       } */
/*     } */
/*   } */
/* } */

void output_grid_trees_ASCII_ART(Grid *s, int cell_width, int iter, bool print_neighbours) {
  int x,y,i;
  
  printf("----step %d----\n", iter);

  for (y = 0; y < s->ymax; y++) {
    /* print trees */
    for (x = 0; x < s->xmax - 1; x++) {
      i = neighbousCoordinates(x,y,s->xmax,s->ymax);
      if(s->grid[i].occupation == -1) {
	printf("%-*c", cell_width, '.');
      } else {
	printf("%-*d",cell_width, s->grid[i].occupation);
      }
    }
    if(s->grid[neighbousCoordinates(s->xmax - 1,y,s->xmax,s->ymax)].occupation == -1) {
      printf("%c", '.');
    } else {
      printf("%d",s->grid[neighbousCoordinates(s->xmax - 1,y,s->xmax,s->ymax)].occupation);
    }

    /* print neighbours */
    if(print_neighbours) {
      printf("  |  ");
      for (x = 0; x < s->xmax - 1; x++) {
	i = neighbousCoordinates(x,y,s->xmax,s->ymax);
	if(s->grid[i].neighbours == 0) {
	  printf("%-*c", cell_width, '.');
	} else {
	  printf("%-*d",cell_width, s->grid[i].neighbours);
	}
      }
      if(s->grid[neighbousCoordinates(s->xmax - 1,y,s->xmax,s->ymax)].neighbours == 0) {
	printf("%c", '.');
      } else {
	printf("%d",s->grid[neighbousCoordinates(s->xmax - 1,y,s->xmax,s->ymax)].neighbours);
      }
    }
    printf("\n");
  }
}

void output_species(Species sp[], int nspecies) {
  for (int i = 0; i < nspecies; i++) {
    if (sp[i].p_recrutement == &get_p_rec_type1) {
      printf("Species %d= pr1=%.10f, pr2=%.10f, ntr1=%.10f, ntr2=%.10f, pm1=%.10f, pm2=%.10f, ntm1=%.10f, ntm2=%.10f\n",i, sp[i].pr1, sp[i].pr2, sp[i].ntr1, sp[i].ntr2, sp[i].pm1, sp[i].pm2, sp[i].ntm1, sp[i].ntm2);
    } else if (sp[i].p_recrutement == &get_p_rec_type2) {
      printf("Species %d= pr1=%.10f, pr2=%.10f, pr3=%.10f, ntr1=%.10f, ntr2=%.10f, ntr3=%.10f, ntr4=%.10f, pm1=%.10f, pm2=%.10f, pm3=%.10f, ntm1=%.10f, ntm2=%.10f, ntm3=%.10f, ntm4=%.10f\n",i, sp[i].pr1, sp[i].pr2, sp[i].pr3, sp[i].ntr1, sp[i].ntr2, sp[i].ntr3, sp[i].ntr4, sp[i].pm1, sp[i].pm2, sp[i].pm3, sp[i].ntm1, sp[i].ntm2, sp[i].ntm3, sp[i].ntm4);
    }
  }
}

typedef struct _EC {
  unsigned int entry_x;
  unsigned int entry_y;
  unsigned int color;
} EC;

typedef struct _RGB {
  double r;
  double g;
  double b;
} RGB;

/**
 * Converts an HSL color value to RGB. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes h, s, and l are contained in the set [0, 1] and
 * returns r, g, and b in the set [0, 1].
 *
 * @param   Number  h       The hue
 * @param   Number  s       The saturation
 * @param   Number  l       The lightness
 * @return  Array           The RGB representation
 */
double hue2rgb(double p, double q, double t){
    if(t < 0) t += 1;
    if(t > 1) t -= 1;
    if(t < 1/6.0) return p + (q - p) * 6 * t;
    if(t < 1/2.0) return q;
    if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
    return p;
}
RGB hslToRgb(double h, double s, double l){
    double r, g, b;
    double q,p;
    RGB res;

    if(s == 0){
      r = l;
      g = l;
      b = l; // achromatic
    }else{
      q = l < 0.5 ? l * (1 + s) : l + s - l * s;
      p = 2 * l - q;
      r = hue2rgb(p, q, h + 1/3.0);
      g = hue2rgb(p, q, h);
      b = hue2rgb(p, q, h - 1/3.0);
    }

    res.r = r;
    res.g = g;
    res.b = b;

    return res;
}

unsigned int * global_colors_prev;
unsigned int * global_colors;
unsigned int global_color_count; //nombre de couleurs utilisées

#define COLORMAPSIZE 1000
RGB colormap[COLORMAPSIZE];

void colorize_init(int savsize) {

  global_colors = malloc(sizeof(unsigned int) * savsize);
  global_colors_prev = malloc(sizeof(unsigned int) * savsize);
  for (int i = 0; i < savsize; i++) {
    global_colors[i] = 0;
    global_colors_prev[i] = 0;
  }

  global_color_count = 0;

  double rnd;
  for (int i = 0; i < COLORMAPSIZE; i++) {
    rnd = drand48();
    colormap[i] = hslToRgb(rnd, 1, 0.5);
  }
}

void colorize(Grid *s, int iteration) {

  int i = 0;
  unsigned int * global_colors_pivot;
  size_t savsize = s->xmax * s->ymax;
  unsigned int clusters_count = 0;
  EC clusters_EC[savsize];
  unsigned int clusters_sizes[savsize];
  unsigned int clusters_count_prev = 0;
  EC clusters_EC_prev[savsize];
  int clusters_sizes_prev[savsize];

  //copier les couleurs dans le tableau tmp
  global_colors_pivot = global_colors_prev;
  global_colors_prev = global_colors;
  global_colors = global_colors_pivot;

  // réinitialiser les couleurs courantes
  for (int i = 0; i < savsize; i++) {
    global_colors[i] = 0;
  }
  
  /************************/
  /* trouver les couleurs */
  /************************/

  // 1ere passe: trouver tous les anciens clusters et les classer par taille décroissante
  // et propager leurs couleurs en privilegiant les plus gros
  bool cluster_already_visited = false;
  for (int x = 0; x < s->xmax; x++) {
    for (int y = 0; y < s->ymax; y++) {
      i = neighbousCoordinates(x,y,s->xmax,s->ymax);
      if(global_colors_prev[i] > 0 && s->grid[i].neighbours > 0) {
        //si la couleur presente n'est pas déjà dans les clusters déjà
        //vus
        cluster_already_visited = false;
        for (int c = 0; c < clusters_count_prev; c++) {
          if (global_colors_prev[i] == clusters_EC_prev[c].color) {
            cluster_already_visited = true;
          }
        }
        
        if(!cluster_already_visited) {
          printf("BBB\n");
            //printf("old %d, xy %d %d, col %d \n", iteration, x,y, global_colors_prev[i]);
            clusters_sizes_prev[clusters_count_prev] = get_cluster_size_tmp(s, x, y);
            clusters_EC_prev[clusters_count_prev].entry_x=x;
            clusters_EC_prev[clusters_count_prev].entry_y=y;
            clusters_EC_prev[clusters_count_prev].color=global_colors_prev[i];
            clusters_count_prev++;
        }
      }
    }
  }

  printf("AAA %d \n", clusters_count_prev);
  
  size_t prev_clusters_by_increasing_size[clusters_count_prev];
  gsl_sort_int_index(prev_clusters_by_increasing_size, clusters_sizes_prev, 1, clusters_count_prev);
  int c;
  for (int c_ind = clusters_count_prev - 1; c_ind > -1; c_ind--) {
    c = prev_clusters_by_increasing_size[c_ind];
    i = neighbousCoordinates(clusters_EC_prev[c].entry_x,
                 clusters_EC_prev[c].entry_y,
                 s->xmax,
                 s->ymax);
    if(global_colors[i] == 0) {
      clusters_sizes[clusters_count] = propagate_color(s, global_colors, savsize,
                                                       clusters_EC_prev[c].entry_x,
                                                       clusters_EC_prev[c].entry_y,
                                                       clusters_EC_prev[c].color);
      clusters_EC[clusters_count].entry_x = clusters_EC_prev[c].entry_x;
      clusters_EC[clusters_count].entry_y = clusters_EC_prev[c].entry_y;
      clusters_EC[clusters_count].color = clusters_EC_prev[c].color;
      clusters_count++;
    }
  }

  

  /* for (int x = 0; x < s->xmax; x++) { */
  /*   for (int y = 0; y < s->ymax; y++) { */
  /*     i = neighbousCoordinates(x,y,s->xmax,s->ymax); */
  /*     if(global_colors_prev[i] > 0 && s->grid[i].neighbours > 0 && global_colors[i] == 0) { */
  /*       //printf("old %d, xy %d %d, col %d \n", iteration, x,y, global_colors_prev[i]); */
  /*       clusters_sizes[clusters_count] = propagate_color_tmp(s, global_colors, savsize, x, y, global_colors_prev[i]); */
  /*       clusters_EC[clusters_count].entry_x=x; */
  /*       clusters_EC[clusters_count].entry_y=y; */
  /*       clusters_EC[clusters_count].color=global_colors_prev[i]; */
  /*       clusters_count++; */
  /*     } */
  /*   } */
  /* } */


  // 2eme passe: propager des nouvelles couleurs sur les nouveaux clusters
  for (int x = 0; x < s->xmax; x++) {
    for (int y = 0; y < s->ymax; y++) {
      i = neighbousCoordinates(x,y,s->xmax,s->ymax);
      if( s->grid[i].neighbours > 0 && global_colors[i] == 0) {
        //printf("new %d, xy %d %d, col %d \n", iteration, x,y, global_colors_prev[i]);
        global_color_count++;
        clusters_sizes[clusters_count] = propagate_color(s, global_colors, savsize, x, y, global_color_count);
        clusters_EC[clusters_count].entry_x=x;
        clusters_EC[clusters_count].entry_y=y;
        clusters_EC[clusters_count].color=global_color_count;
        clusters_count++;
      }
    }
  }

  //3eme passe: repérer les doublons et leur donner de nouvelles couleurs
  size_t doublons[clusters_count];
  int doublons_count = 0;
  int biggest_doublon = 0;

  for (int k = 0; k < clusters_count - 1; k++) {
    doublons_count = 0;
    for (int l = k+1; l < clusters_count; l++) {
      if(clusters_EC[k].color == clusters_EC[l].color) {
        doublons[doublons_count] = l;
        doublons_count++;
      }
    }
    if (doublons_count > 0) {
      // ne pas oublier le cluster k
      //printf("dou %d, xy %d %d, col %d \n", iteration, clusters_EC[k].entry_x, clusters_EC[k].entry_y, global_colors_prev[i]);
      doublons[doublons_count] = k;
      doublons_count++;

      // find biggest so we can keep its color
      for (int i = 0; i < doublons_count; i++) {
        if (clusters_sizes[doublons[i]] > clusters_sizes[doublons[biggest_doublon]]) {
          biggest_doublon = i;
        }
      }

      for(int i = 0; i < doublons_count; i++) {
        if (i != biggest_doublon) {
          //printf("dou %d, xy %d %d, col %d \n", iteration, clusters_EC[doublons[i]].entry_x, clusters_EC[doublons[i]].entry_y, global_colors_prev[i]);
          propagate_color(s, global_colors, savsize,
                          clusters_EC[doublons[i]].entry_x,
                          clusters_EC[doublons[i]].entry_y,
                          global_color_count);
          global_color_count++;
        }
      }
    }
  }
}

void output_grid_png(Grid *s, int nspecies, int iter, int itermax, char filename_prefix[]) {

  cairo_surface_t *surface;
  cairo_t *cr;

  double size_factor = 5;
  
  /*********/
  /* Cairo */
  /*********/

  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, s->xmax * size_factor, s->ymax * size_factor);
  cr = cairo_create (surface);

  int itermax_tmp = itermax;
  int itermax_str_width=1;
  while (itermax_tmp > 9) {
    itermax_tmp = itermax_tmp / 10;
    itermax_str_width++;
  }

  char filename [OUTPUT_GRID_PNG_FILENAME_MAX_LENGTH]="";
  sprintf(filename, "%s%0*d.png",filename_prefix, itermax_str_width, iter);

  // Paint the background white
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  // Draw the cover with the color of the corresponding cluster
  int i;
  for (int x = 0; x < s-> xmax; x++) {
    for (int y = 0; y < s-> ymax; y++) {
      i = neighbousCoordinates(x,y,s->xmax, s->ymax);
      if (s->grid[i].neighbours > 0) {
        cairo_rectangle(cr, x * size_factor,y * size_factor,1 * size_factor,1 * size_factor);
        cairo_set_source_rgb(cr, 145/255.0,214/255.0,108/255.0);
        //cairo_set_source_rgb(cr, colormap[global_colors[i] % COLORMAPSIZE].r, colormap[global_colors[i] % COLORMAPSIZE].g, colormap[global_colors[i] % COLORMAPSIZE].b);
        cairo_fill(cr);
      }
    }
  }

  // Draw the trees black
  for (int x = 0; x < s-> xmax; x++) {
    for (int y = 0; y < s-> ymax; y++) {
      if (s->grid[neighbousCoordinates(x,y,s->xmax, s->ymax)].occupation != -1) {
        cairo_rectangle(cr, x * size_factor,y * size_factor,1 * size_factor,1 * size_factor);
      }
    }
  }
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_fill(cr);

  cairo_destroy(cr);
  fprintf(stderr, "writing png to %s\n", filename);
  cairo_surface_write_to_png(surface, filename);
  cairo_surface_destroy(surface);
}
