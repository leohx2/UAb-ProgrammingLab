#include "headers/helper.h"
#include "headers/drawing.h"
#include "headers/commands.h"

int main()
{
  SCoordinates *s_coordinates;
  LRect *l_rect;
  int matrix[MAX_LINES][MAX_COL];

  l_rect = NULL;
  s_coordinates = (SCoordinates *)malloc(sizeof(SCoordinates));
  if (s_coordinates == NULL)
  {
    printf("Error, insuficient memory (coordinate)");
    return EXIT_FAILURE;
  }

  while (Read_commands(s_coordinates))
  {
    Initialize_matrix(matrix);

    if (s_coordinates->command == 'c')
    {
      if (Rectangle_len(l_rect) == 10)
        printf("\nLimit of 10 rectangles reached. To be able to create a new reactangle, please delete one (or more) rectangle(s)\n");
      else
      {
        if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
        {
          l_rect = Add_rect(l_rect, s_coordinates);
          Collision_warning(l_rect, l_rect);
        }
      }
    }
    else if (s_coordinates->command == 'd')
      l_rect = Delete_rectangle(l_rect, s_coordinates);
    else
      Move_rectangle(l_rect, s_coordinates);
    Update_rect(l_rect);
    Update_matrix(matrix, l_rect);
    Draw_matrix(matrix);
  }

  free(s_coordinates);
  s_coordinates = NULL;
  l_rect = Free_all_rect(l_rect);
}