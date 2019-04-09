/*
Ant Colony Algorithm for clustering
Authors: Fabricio Vilasboas, Calebe Bianchini, Leandro Nunes
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "types.h"

int cell_has_item(const int x, const int y, Cell **B)
{
  int res = (B[x][y].item == NULL) ? 0 : 1;
  return res;
}

int ant_has_item(Ant *ant)
{
  int res = (ant->item == NULL) ? 0 : 1;
  return res;
}

double manhattan_distance(double *item1, double *item2, const int n)
{
  int i;
  double sum = 0.0;
  for(i = 0; i < n; ++i)
  {
    sum += fabsf(item1[i]-item2[i]);
  }
  return sum/(double)n;
}

double euclidian_distance(double *item1, double *item2, const int n)
{
  int i;
  double sum = 0.0;

  for (i = 0; i < n; i++)
  {
    sum += pow(item1[i] - item2[i], 2);
  }
  return sqrt(sum/(double)n);
}

double f(Cell **B, double *item, const int x, const int y, const int nb, const int m, const int s_items)
{
  int i, j;
  double sum = 0.0, cont = 0.0;
  for (i = x - nb; i < x + nb; i++)
    for (j = y - nb; j < y + nb; j++)
    {
      if (i >= 0 && i < m && j >= 0 && j < m)
      {
        if (cell_has_item(i, j, B) == 1)
        {
          //sum += euclidian_distance(item, B[i][j].item, s_items);
          sum += manhattan_distance(item, B[i][j].item, s_items);
        }
      }
    }
  return fabs(sum/(nb*nb));
}

void ant_dynamic(Ant *ant, Cell **B, const int m, const int nb, const int s_items, const double max_value)
{
  double pp, pd, fx;
  size_t size = sizeof(double)*s_items;

  //Probability of the ant drop the item
  if (ant_has_item(ant) == 1 && cell_has_item(ant->x, ant->y, B) == 0)
  {
    pd = f(B, ant->item, ant->x, ant->y, nb, m, s_items);
    pd = 1-(pd/max_value);
    //fprintf(stderr, "\tDrop; pd: %f\n", pd);
    if (pd >= 0.6)
    {
      B[ant->x][ant->y].item = (double*)malloc(size);
      memcpy(B[ant->x][ant->y].item, ant->item, size);
      free(ant->item);
      ant->item = NULL;
      B[ant->x][ant->y].item_id = ant->item_id;
      ant->item_id = -1;
      //fprintf(stdout, "\tDroped item %d on cell [%d, %d] with pd = %f.\n", B[ant->x][ant->y].item_id, ant->x, ant->y, pd);
    }
    return;
  }

  // Probability of the ant pick the item
  if (ant_has_item(ant) == 0 && cell_has_item(ant->x, ant->y, B) == 1)
  {
    pp = f(B, B[ant->x][ant->y].item, ant->x, ant->y, nb, m, s_items);
    pp = 1-(pp/max_value);
    //fprintf(stderr, "\tPick; pp: %f\n", pp);
    if (pp <= 0.6)
    {
      ant->item = (double*)malloc(size);
      memcpy(ant->item, B[ant->x][ant->y].item, size);
      free(B[ant->x][ant->y].item);
      B[ant->x][ant->y].item = NULL;
      ant->item_id = B[ant->x][ant->y].item_id;
      B[ant->x][ant->y].item_id = -1;
      //fprintf(stdout, "\tPicked item %d on cell [%d, %d] with pp = %f.\n", ant->item_id, ant->x, ant->y, pp);
    }
    return;
  }
}

void move_ant(Ant *ant, Cell **B, const int m, const int nb, const int s_items, const double max_value)
{
  int direction, moved = 0, stuck = 0, step = 5;

  do
  {
    direction = rand()%4;

    switch (direction)
    {
      case 0: // Move up [x-1,y];
        stuck += 1;
        if ((ant->x - step) >= 0)
        {
          if (B[ant->x-step][ant->y].has_ant == 0)
          {
            B[ant->x][ant->y].has_ant = 0;
            ant->x -= step;
            B[ant->x][ant->y].has_ant = 1;
            ant_dynamic(ant, B, m, nb, s_items, max_value);
            moved = 1;
          }
        }
      break;
      case 1: // Move down [x+1,y];
        stuck += 1;
        if ((ant->x + step) < m)
        {
          if (B[ant->x+step][ant->y].has_ant == 0)
          {
            B[ant->x][ant->y].has_ant = 0;
            ant->x += step;
            B[ant->x][ant->y].has_ant = 1;
            ant_dynamic(ant, B, m, nb, s_items, max_value);
            moved = 1;
          }
        }
      break;
      case 2: // Move right [x,y+1];
        stuck += 1;
        if ((ant->y + step) < m)
        {
          if (B[ant->x][ant->y+1].has_ant == 0)
          {
            B[ant->x][ant->y].has_ant = 0;
            ant->y += step;
            B[ant->x][ant->y].has_ant = 1;
            ant_dynamic(ant, B, m, nb, s_items, max_value);
            moved = 1;
          }
        }
      break;
      case 3: // Move left [x,y-1];
        stuck += 1;
        if ((ant->y - step) >= 0)
        {
          if (B[ant->x][ant->y-step].has_ant == 0)
          {
            B[ant->x][ant->y].has_ant = 0;
            ant->y -= step;
            B[ant->x][ant->y].has_ant = 1;
            ant_dynamic(ant, B, m, nb, s_items, max_value);
            moved = 1;
          }
        }
      break;
    }
  } while(moved == 0 && stuck <= 4);
}

void simulte(Ant *ants, Cell **B, const int m, const int n_ants, const int nb,
            const int s_items, const int max_it, const double max_value)
{
  int it, ant;

  for (it = 0; it < max_it; it++)
  {
    for (ant = 0; ant < n_ants; ant++)
    {
      move_ant(&ants[ant], B, m, nb, s_items, max_value);
      //fprintf(stdout, "%d,%d,%d,%d,%d\n", it, ant, ants[ant].x, ants[ant].y, ants[ant].item_id);
    }
  }
}

Cell** grid_allocation(const int m)
{
  Cell **B;
  int i, j;

  B = (Cell **)malloc(m * sizeof(Cell*));
  for(i = 0; i < m; i++) B[i] = (Cell*)malloc(m * sizeof(Cell));

  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++)
    {
      B[i][j].item = NULL;
      B[i][j].has_ant = 0;
      B[i][j].item_id = -1;
    }

  return B;
}

double** items_allocation(const int n_items, const int s_items, const double max_value)
{
  double **items;
  int i, j;

  items = (double **)malloc(n_items * sizeof(double*));
  for(i = 0; i < n_items; i++) items[i] = (double*)malloc(s_items * sizeof(double));

  for (i = 0; i < n_items; i++)
    for (j = 0; j < s_items; j++)
    {
      items[i][j] = i*j;//rand()%(int)max_value;
    }
  return items;
}

/* Initialize grid with ants and items ramdomilly.
  In the first step ants are carryng no items.
*/
void initialize(const int m, const int n_ants, const int n_items, const int s_items, Cell **B, Ant *ants, double **items)
{
  int i = 0, j, x, y, ant_prob, item_prob;
  size_t size = sizeof(double)*s_items;

  // Initialize Cell with items
  do
  {
    x = rand()%m;
    y = rand()%m;
    if (cell_has_item(x, y, B) == 0 && B[x][y].has_ant == 0)
    {
      B[x][y].item = (double*)malloc(size);
      memcpy(B[x][y].item, items[i], size);
      B[x][y].item_id = i;
      i++;
    }
  } while(i != n_items);
  free(items);

  // Initialize Cell with ants
  i = 0;
  do
  {
    x = rand()%m;
    y = rand()%m;
    if (cell_has_item(x, y, B) == 0 && B[x][y].has_ant == 0)
    {
      ants[i].x = x;
      ants[i].y = y;
      ants[i].item = NULL;
      ants[i].item_id = -1;
      B[x][y].has_ant = 1;
      i++;
    }
  } while(i != n_ants);

}

void grid_print(Cell **B, const int m)
{
  int i, j;
  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++)
      //fprintf(stdout, "%d,%d,%d\n", i, j, cell_has_item(i, j, B));
      fprintf(stdout, "%d\n", cell_has_item(i, j, B));
}

int main (int argc, char **argv)
{
  /*
  Variables clarification:
    m: square grid size;
    n_ants: number of ants;
    max_it: max iterations;
    nb: number of neighbors;
    k1: threshold constant defined by the user;
    k2: threshold constant defined by the user;
    a: scale dimensionality factor
    f: perceived fraction of items;
    B: grid of cells;
    ants: vector of ants;
    items: matrix of items;
    n_items: number of items;
    s_items: number of elementes per item.
  */

  // A razão de proporcionalidade de m -> n_ants e n_items é de uma ordem de grandeza.
  int m = 100, n_ants = 100, max_it = 1000000, nb = 30, n_items = 500, s_items = 100;
  double **items, max_value = 1000;
  Cell **B;
  Ant *ants = (Ant*)malloc(sizeof(Ant) * n_ants);
  srand(time(NULL));
  //fprintf(stdout, "Allocating B\n");
  B = grid_allocation(m);
  //fprintf(stdout, "Allocating items\n");
  items = items_allocation(n_items, s_items, max_value);
  //fprintf(stdout, "Initialize\n");
  initialize(m, n_ants, n_items, s_items, B, ants, items);
  //fprintf(stdout, "Simulate\n");
  simulte(ants, B, m, n_ants, nb, s_items, max_it, max_value);
  grid_print(B, m);

  return 0;
}
