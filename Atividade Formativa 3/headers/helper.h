#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINES 25
#define MAX_COL 80
typedef struct
{
  char command; // can be 'c', 'l', 'r' or 'd' for create, move left, move right and delete
  int x;
  int y;
  int l; // width
  int h; // height
  int p; // number of positions to move.
} SCoordinates;

typedef struct SRect
{
  int x;
  int y;
  int l;
  int h;
  int id;
  struct SRect *next;
} LRect;

void Str_to_lower(char *s);
void Get_str_input(char *input);
void Initialize_matrix(int m[MAX_LINES][MAX_COL]);
void Print_commands();
void Collision_detection(LRect *all_rectangles, LRect *current_rectangle);
int Is_there_a_rectangle(LRect *l_rect, SCoordinates *s_coordinates, int mode);
void Collision_warning(LRect *all_rect, LRect *current_rect);

#endif