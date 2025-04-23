#include "headers/helper.h"
#include "headers/commands.h"

int main()
{
  SCoordinates *s_coordinates;

  s_coordinates = (SCoordinates *)malloc(sizeof(s_coordinates));
  if (s_coordinates == NULL)
  {
    printf("Error, insuficient memory (coordinate)");
    return EXIT_FAILURE;
  }

  while (Read_commands(s_coordinates))
  {
    // if (s_coordinates->command == 'c')
    //   printf("\nCommand: %c, x: %d, y: %d, l: %d, h: %d\n", s_coordinates->command, s_coordinates->x, s_coordinates->y, s_coordinates->l, s_coordinates->h);
    // else
    //   printf("\nCommand: %c, x: %d, y: %d, p: %d\n", s_coordinates->command, s_coordinates->x, s_coordinates->y, s_coordinates->p);
  }

  free(s_coordinates);
  s_coordinates = NULL;
}