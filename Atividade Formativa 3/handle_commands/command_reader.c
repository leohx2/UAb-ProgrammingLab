#include "../headers/helper.h"

int Command_verify(char *input);

int Read_commands(SCoordinates *s_coordinates)
{
  char input[50];

  printf("\nYour command (or \"exit\" to leave and \"help\" for a user manual): ");

  Get_str_input(input);
  Str_to_lower(input);

  while (Command_verify(input) == EXIT_FAILURE)
  {
    printf("\nYour command (or \"exit\" to leave): ");
    Get_str_input(input);
    Str_to_lower(input);
  }

  if (strcmp("exit", input) == 0)
    return 0;
  else if (strcmp("help", input) == 0)
  {
    printf("\nList of commands:\n");
    printf("x,y >=1, x <= 80 and y <= 25\n");
    printf("create x,y+l,h\nmoveright x,y+p\nmoveleft x,y+p\nexit\nhelp\n");
    printf("create x,y+l,h - Creates a rectangle where (x,y) are the coordinates of the bottom-left corner, and (l,h) are the length and height, respectively.\n");
    printf("moveright x,y+p - Moves the rectangle located at coordinates (x,y) to the right by p positions.\n");
    printf("moveleft x,y+p - Moves the rectangle containing the point (x,y) to the left by p positions.\n");
    return (Read_commands(s_coordinates));
  }
  return 1;
}

int Command_verify(char *input)
{
  char *token, *coordinates, command;

  if (input == NULL)
    return EXIT_FAILURE;

  token = strtok(input, " ");

  // First case, exit or help.
  if (strcmp(token, "exit") == 0 || strcmp(token, "help") == 0)
    return EXIT_SUCCESS;

  // Verify command
  if (strcmp(token, "create") != 0 && strcmp(token, "moveright") != 0 && strcmp(token, "moveleft") != 0)
  {
    printf("\nInvalid input, heres a list of all valid commands\n");
    printf("create x,y+l,h\nmoveright x,y+p\nmoveleft x,y+p\nexit\nhelp\n");
    return EXIT_FAILURE;
  }

  // Now we know that the command is right, let's see the instruction.
  command = token[0];
  token = strtok(NULL, " ");

  // the create and move instructions are different, create has 2 coordinates plus width and height, and moove has 2 coordinates and a position.
  printf("Testing token: %s\n", token);
  // TODO -> verificação das coordenadas!
  // TODO -> verificar se há mais argumentos do que o necessário.
  return EXIT_SUCCESS;
}