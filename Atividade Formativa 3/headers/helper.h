#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINES 25
#define MAX_COL 80
typedef struct
{
  char command; // can be 'c', 'l' or 'r' for create, move left and move right.
  int x;
  int y;
  int l; // width
  int h; // height
  int p; // number of positions to move.
} SCoordinates;

void Str_to_lower(char *s);
void Get_str_input(char *input);
void Initialize_matrix(int m[MAX_LINES][MAX_COL]);

#endif