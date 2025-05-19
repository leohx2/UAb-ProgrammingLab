#ifndef DRAWING_H
#define DRAWING_H
#include "helper.h"

void Update_matrix(int m[MAX_LINES][MAX_COL], LRect *l_rect);
void Draw_matrix(int m[MAX_LINES][MAX_COL]);
int Update_rect(LRect *l_rect);
int Valid_action(LRect *l_rect, int x, int y, int h, int l, int id);
LRect *Free_all_rect(LRect *l_rect);
LRect *Free_rect(LRect *l_rect);
LRect *Delete_rectangle(LRect *l_rect, SCoordinates *s_coordinates);

#endif