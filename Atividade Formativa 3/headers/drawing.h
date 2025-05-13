#ifndef DRAWING_H
#define DRAWING_H
#include "helper.h"

typedef struct SRect
{
  int x;
  int y;
  int l;
  int h;
  int id;
  struct SRect *next;
} LRect;

void Update_matrix(int m[MAX_LINES][MAX_COL], LRect *l_rect);
void Draw_matrix(int m[MAX_LINES][MAX_COL]);
void Update_rect(LRect *l_rect);
int Valid_action(LRect *l_rect, int x, int y, int h, int l, int id);

#endif