#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct Cell
{
  float* item;
  int item_id;
  int has_ant;
  int ant_id;
} Cell;

typedef struct Ant
{
  int x, y;
  float* item;
  int item_id;
} Ant;

#endif
