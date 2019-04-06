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

int ant_has_item(Ant ant)
{
  int res = (ant.item == NULL) ? 0 : 1;
  return res;
}

float cosine_similarity(float *item1, float *item2, const int n)
{
  int i;
  float dot = 0.0, denom_a = 0.0, denom_b = 0.0 ;
  for(i = 0; i < n; ++i)
  {
    dot += item1[i] * item2[i] ;
    denom_a += item1[i] * item1[i] ;
    denom_b += item2[i] * item2[i] ;
  }
  return dot / (sqrt(denom_a) * sqrt(denom_b));
}

float dist(float *item1, float *item2, const int n)
{
  int i;
  float sum = 0.0;
  //fprintf(stdout, "\tdist:1; n: %d.\n", n);
  for (i = 0; i < n; i++)
  {
    //fprintf(stdout, "\t\n");
    sum += item1[i]*item1[i] - item2[i]*item2[i];
  }
  //fprintf(stdout, "\tdist:3\n");
  return sqrt(sum);
}

float f(Cell **B, float *item, const int x, const int y, const int nb, const int m, const int s_items)
{
  int i, j;
  float sum = 0;
  //fprintf(stdout, "\tf:1 -> x: %d, y: %d.\n", x, y);
  for (i = x - nb; i < x + nb; i++)
    for (j = y - nb; j < y + nb; j++)
    {
      //fprintf(stdout, "\tf:2 -> x: %d, y: %d.\n", i, j);
      if (i >= 0 && i < m && j >= 0 && j < m)
      {
        if (cell_has_item(i, j, B) == 1)
        {
          //fprintf(stdout, "\tf:3\n");
          //sum += 1-dist(item, B[i][j].item, s_items);
          sum += cosine_similarity(item, B[i][j].item, s_items);
        }
      }
    }
  //fprintf(stdout, "\tf:4\n");
  return abs(sum/nb);
}

void ant_dynamic(Ant *ant, Cell **B, const float k1, const float k2, const int m,
                const int nb, const int s_items)
{
  float pp, pd, fx;
  size_t size = sizeof(float)*s_items;

  //Probability of the ant drop the item
  if (ant_has_item(*ant) == 1 && cell_has_item(ant->x, ant->y, B) == 0)
  {
    pd = f(B, ant->item, ant->x, ant->y, nb, m, s_items);
    //pd = (fx < k2) ? 2*fx : 1;
    fprintf(stdout, "\tDrop; pd: %.2f\n", pd);
    if (pd >= 0.5)
    {
      B[ant->x][ant->y].item = (float*)malloc(size);
      memcpy(B[ant->x][ant->y].item, ant->item, size);
      ant->item = NULL;
      B[ant->x][ant->y].item_id = ant->item_id;
      ant->item_id = -1;
      fprintf(stdout, "\tDroped item %d on cell [%d, %d] with pd = %.2f.\n", B[ant->x][ant->y].item_id, ant->x, ant->y, pd);
    }
    return;
  }
  // Probability of the ant pick the item
  if (ant_has_item(*ant) == 0 && cell_has_item(ant->x, ant->y, B) == 1)
  {
    fprintf(stdout, "\tPick.\n");
    //pp = pow( k1 / (k1 + f(B, B[ant->x][ant->y].item, ant->x, ant->y, nb, m, s_items) ), 2);
    pp = f(B, B[ant->x][ant->y].item, ant->x, ant->y, nb, m, s_items);
    if (pp < 0.5)
    {
      ant->item = (float*)malloc(size);
      memcpy(ant->item, B[ant->x][ant->y].item, size);
      B[ant->x][ant->y].item = NULL;
      ant->item_id = B[ant->x][ant->y].item_id;
      B[ant->x][ant->y].item_id = -1;
      fprintf(stdout, "\tPicked item %d on cell [%d, %d] with pp = %.2f.\n", ant->item_id, ant->x, ant->y, pp);
    }
    return;
  }
}

void move_ant(Ant *ant, Cell **B, const int m, const int nb, const float k1,
              const float k2, const int s_items)
{
  int direction, moved = 0, stuck = 0;

  fprintf(stdout, "\tx: %d, y: %d\n", ant->x, ant->y);
  do
  {
    direction = rand()%4;

    switch (direction)
    {
      case 0: // Move up [x-1,y];
        stuck += 1;
        if ((ant->x - 1) >= 0)
        {
          if (B[ant->x-1][ant->y].has_ant == 0)
          {
            fprintf(stdout, "\tMove up\n");
            B[ant->x][ant->y].has_ant = 0;
            ant->x -= 1;
            B[ant->x][ant->y].has_ant = 1;
            ant_dynamic(ant, B, k1, k2, m, nb, s_items);
            moved = 1;
          }
        }
      break;
      case 1: // Move down [x+1,y];
        stuck += 1;
        if ((ant->x + 1) > m)
        {
          if (B[ant->x+1][ant->y].has_ant == 0)
          {
            fprintf(stdout, "\tMove down\n");
            B[ant->x][ant->y].has_ant = 0;
            ant->x += 1;
            B[ant->x][ant->y].has_ant = 1;
            ant_dynamic(ant, B, k1, k2, m, nb, s_items);
            moved = 1;
          }
        }
      break;
      case 2: // Move right [x,y+1];
        stuck += 1;
        if ((ant->y + 1) >= 0)
        {
          if (B[ant->x][ant->y+1].has_ant == 0)
          {
            fprintf(stdout, "\tMove rigth\n");
            B[ant->x][ant->y].has_ant = 0;
            ant->y += 1;
            B[ant->x][ant->y].has_ant = 1;
            ant_dynamic(ant, B, k1, k2, m, nb, s_items);
            moved = 1;
          }
        }
      break;
      case 3: // Move left [x,y-1];
        stuck += 1;
        if ((ant->y - 1) >= 0)
        {
          if (B[ant->x][ant->y-1].has_ant == 0)
          {
            fprintf(stdout, "\tMove left\n");
            B[ant->x][ant->y].has_ant = 0;
            ant->y -= 1;
            B[ant->x][ant->y].has_ant = 1;
            ant_dynamic(ant, B, k1, k2, m, nb, s_items);
            moved = 1;
          }
        }
      break;
    }
    fprintf(stdout, "\tx: %d, y: %d -> stuck: %d.\n", ant->x, ant->y, stuck);
  } while(moved == 0 && stuck <= 4);
}

void simulte(Ant *ants, Cell **B, const int m, const int n_ants, const int nb,
            const int s_items, const int max_it, const float k1, const float k2)
{
  int it, i;

  for (it = 0; it < max_it; it++)
  {
    fprintf(stdout, "\nIteration: %d.\n", it);
    for (i = 0; i < n_ants; i++)
    {
      fprintf(stdout, "Ant: %d.\n", i);
      move_ant(&ants[i], B, m, nb, k1, k2, s_items);
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
    {
      items[i][j] = rand()%1000;
      //fprintf(stdout, "tems_allocation; item[%d][%d]: %.2f.\n", i, j, items[i][j]);
    }

  return items;
}

/* Initialize grid with ants and items ramdomilly.
  In the first step ants are carryng no items.
*/
void initialize(const int m, const int n_ants, const int n_items, const int s_items, Cell **B, Ant *ants, float **items)
{
  int i = 0, j, x, y, ant_prob, item_prob;
  size_t size = sizeof(float)*s_items;

  // Initialize Cell with items
  do
  {
    x = rand()%m;
    y = rand()%m;
    if (cell_has_item(x, y, B) == 0 && B[x][y].has_ant == 0)
    {
      B[x][y].item = (float*)malloc(size);
      memcpy(B[x][y].item, items[i], size);
      B[x][y].item_id = i;
      //fprintf( stdout, "i: %d, x: %d, y: %d, has_ant: %d, has_item: %d, item_id: %d.\n", i, x, y, B[x][y].has_ant, cell_has_item(x, y, B), B[x][y].item_id);
      i++;
    }
  } while(i != n_items);

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
      //fprintf( stdout, "i: %d, x: %d, y: %d, has_ant: %d, has_item: %d.\n", i, x, y, B[x][y].has_ant, cell_has_item(x, y, B));
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

  int m = 1000, n_ants = 1000, max_it = 100000, nb = 10, n_items = 10000, s_items = 100;
  float k1 = 0.5, k2 = 0.5, **items;
  Cell **B;
  Ant *ants = (Ant*)malloc(sizeof(Ant) * n_ants);
  srand(time(NULL));
  fprintf(stdout, "Allocating B\n");
  B = grid_allocation(m);
  fprintf(stdout, "Allocating items\n");
  items = items_allocation(n_items, s_items);
  fprintf(stdout, "Initialize\n");
  initialize(m, n_ants, n_items, s_items, B, ants, items);
  fprintf(stdout, "Simulate\n");
  simulte(ants, B, m, n_ants, nb, s_items, max_it, k1, k2);

  return 0;
}
