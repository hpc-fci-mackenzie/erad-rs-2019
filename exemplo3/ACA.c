/*
Ant Colony Algorithm for clustering
Authors: Fabricio Vilasboas, Calebe Bianchini, Leandro Nunes
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "types.h"

float euclidian_distance(float *item1, float *item2, const int n)
{
  int i;
  float sum = 0;

  for (i = 0; i < n; i++)
    sum = item1[i]*item1[i] + item2[i]*item2[i];
  return sqrtf(sum);
}

float pick_probability(const float k1, const float f)
{
  return powf(k1/(k1+f), 2);
}

float drop_probability(const float k2, const float f)
{
  return powf(f/(k2+f), 2);
}

int cell_has_item(const int x, const int y, Cell **B)
{
  int res = (B[x][y].item == NULL) ? 0 : 1;
  return res;
}

int ant_has_item(Ant ant)
{
  // Trocar para operação ternária
  int res = (ant.item == NULL) ? 0 : 1;
  return res;
}

void ant_dynamic(Ant ant, Cell **B, const float k1, const float k2, const float f)
{
  //Probability of the ant drop the item
  if (ant_has_item(ant) && !cell_has_item(ant.x, ant.y, B))
  {
    if (drop_probability(k2,f) > 0.6)
    {
      B[ant.x][ant.y].item = (float*)malloc(sizeof(ant.item));
      memcpy(B[ant.x][ant.y].item, ant.item, sizeof(ant.item));
      ant.item = NULL;
      B[ant.x][ant.y].item_id = ant.item_id;
      ant.item_id = -1;
    }
  }
  // Probability of the ant pick the item
  if (cell_has_item(ant.x, ant.y, B) && !ant_has_item(ant))
  {
    if (pick_probability(k1,f) > 0.6)
    {
      ant.item = (float*)malloc(sizeof(B[ant.x][ant.y].item));
      memcpy(ant.item, B[ant.x][ant.y].item, sizeof(B[ant.x][ant.y].item));
      B[ant.x][ant.y].item = NULL;
      ant.item_id = B[ant.x][ant.y].item_id;
      B[ant.x][ant.y].item_id = -1;
    }
  }
}

void move_ant(Ant ant, Cell **B, float **items, const int m, const float k1, const float k2, const float f)
{
  int direction, moved = 0;

  do
  {
    direction = rand()%4;

    switch (direction) {
      case 0: // Move up [x-1,y];
        if ((ant.x - 1) >= 0)
        {
          if (!B[ant.x][ant.y].has_ant)
          {
            B[ant.x][ant.y].has_ant = 0;
            ant.x -= 1;
            ant_dynamic(ant, B, k1, k2, f);
            moved = 1;
          }
        }
      break;
      case 1: // Move down [x+1,y];
        if ((ant.x + 1) > m)
        {
          if (!B[ant.x][ant.y].has_ant)
          {
            B[ant.x][ant.y].has_ant = 0;
            ant.x += 1;
            ant_dynamic(ant, B, k1, k2, f);
            moved = 1;
          }
        }
      break;
      case 2: // Move right [x,y+1];
        if ((ant.y + 1) >= 0)
        {
          if (!B[ant.x][ant.y].has_ant)
          {
            B[ant.x][ant.y].has_ant = 0;
            ant.y += 1;
            ant_dynamic(ant, B, k1, k2, f);
            moved = 1;
          }
        }
      break;
      case 3: // Move left [x,y-1];
        if ((ant.y - 1) >= 0)
        {
          if (!B[ant.x][ant.y].has_ant)
          {
            B[ant.x][ant.y].has_ant = 0;
            ant.y -= 1;
            ant_dynamic(ant, B, k1, k2, f);
            moved = 1;
          }
        }
      break;
    }
  } while(!moved);

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
    }

  return B;
}

float** items_allocation(const int n_items, const int s_items)
{
  float **items;
  int i, j;

  items = (float **)malloc(n_items * sizeof(float*));
  for(i = 0; i < n_items; i++) items[i] = (float*)malloc(s_items * sizeof(float));

  for (i = 0; i < n_items; i++)
    for (j = 0; j < s_items; j++)
      items[i][j] = rand();

  return items;
}

/* Initialize grid with ants and items ramdomilly.
  In the first step ants are carryng no items.
*/
void initialize(const int m, const int n_ants, const int n_items, Cell **B, Ant *ants, float **items)
{
  int i = 0, x, y, ant_prob, item_prob;
  size_t item_size = sizeof(float) * n_items;

  // Initialize Cell with items
  do
  {
    x = rand()%m;
    y = rand()%m;
    if (!cell_has_item(x, y, B) && !B[x][y].has_ant)
    {
      B[x][y].item = (float*)malloc(item_size);
      memcpy(B[x][y].item, items[i], item_size);
      B[x][y].item_id = i;
      fprintf( stdout, "i: %d, x: %d, y: %d, has_ant: %d, has_item: %d, item_id: %d.\n", i, x, y, B[x][y].has_ant, cell_has_item(x, y, B), B[x][y].item_id);
      i++;
    }
  } while(i != n_items);

  // Initialize Cell with ants
  i = 0;
  do
  {
    x = rand()%m;
    y = rand()%m;
    if (!cell_has_item(x, y, B) && !B[x][y].has_ant)
    {
      ants[i].x = x;
      ants[i].y = y;
      ants[i].item = NULL;
      ants[i].item_id = -1;
      B[x][y].has_ant = 1;
      B[x][y].ant_id = i;
      fprintf( stdout, "i: %d, x: %d, y: %d, has_ant: %d, has_item: %d, and_id: %d.\n", i, x, y, B[x][y].has_ant, cell_has_item(x, y, B), B[x][y].ant_id);
      i++;
    }
  } while(i != n_ants);

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
    f: perceived fraction of items;
    B: grid of cells;
    ants: vector of ants;
    items: matrix of items;
    n_items: number of items;
    s_items: number of elementes per item.
  */

  int m = 10, n_ants = 50, max_it = 1000, nb = 3, n_items = 50, s_items = 100;
  float k1 = 0, k2 = 0, f = 0, **items;
  Cell **B;
  Ant *ants = (Ant*)malloc(sizeof(Ant) * n_ants);

  B = grid_allocation(m);
  items = items_allocation(n_items, s_items);
  initialize(m, n_ants, n_items, B, ants, items);

  return 0;
}
