#ifndef COMMANDS_H
#define COMMANDS_H

#include "helper.h"
#include "drawing.h"

int Read_commands(SCoordinates *s_coordinates);
int Random_id(LRect *l_rect);
LRect *Add_rect(LRect *l_rect, SCoordinates *s_condinates);
void Move_rectangle(LRect *l_rect, SCoordinates *s_coordinates);
int Rectangle_len(LRect *l_rect);
int Command_verify(char *input, SCoordinates *s_coordinates);

#endif