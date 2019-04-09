#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct Cell
{
  double* item;
  int item_id;
  int has_ant;
} Cell;

typedef struct Ant
{
  int x, y;
  double* item;
  int item_id;
} Ant;

#endif
