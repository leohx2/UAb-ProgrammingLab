#include "../headers/helper.h"

int Command_verify(char *input, SCoordinates *s_coordinates);
int Coordinates_verify(char *coordinates, SCoordinates *s_coordinates);
int Verify_last_part(char *last_part, SCoordinates *s_coordinates);
int Valid_amount_of_arguments(char *str);

int Read_commands(SCoordinates *s_coordinates)
{
  char input[50];

  printf("\nYour command (or \"exit\" to leave and \"help\" for a user manual): ");
  Get_str_input(input);
  Str_to_lower(input);

  while (Command_verify(input, s_coordinates) == EXIT_FAILURE)
  {
    printf("\nYour command (or \"exit\" to leave and \"help\" for a user manual): ");
    Get_str_input(input);
    Str_to_lower(input);
  }

  if (strcmp("exit", input) == 0)
    return 0;
  else if (strcmp("help", input) == 0)
  {
    printf("\nList of commands:\n");
    printf("x,y >=1, x <= 80 and y <= 25\n");
    printf("create x,y+l,h - Creates a rectangle where (x,y) are the coordinates of the bottom-left corner, and (l,h) are the length and height, respectively.\n");
    printf("moveright x,y+p - Moves the rectangle located at coordinates (x,y) to the right by p positions.\n");
    printf("moveleft x,y+p - Moves the rectangle containing the point (x,y) to the left by p positions.\n");
    printf("exit\nhelp\n");
    return (Read_commands(s_coordinates));
  }
  return 1;
}

int Command_verify(char *input, SCoordinates *s_coordinates)
{
  char *token, token_backup[20];

  if (input == NULL)
    return EXIT_FAILURE;

  // Getting the fist part of the command, can be exit, help, create, moveleft or moveright
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
  if (token[0] == 'c')
    s_coordinates->command = token[0];
  else
    s_coordinates->command = token[4];

  // The 2nd part of our command can be "x,y+l,h" or "x,y+p"
  token = strtok(NULL, " ");

  strcpy(token_backup, token);
  if (Valid_amount_of_arguments(token_backup) == EXIT_FAILURE)
    return EXIT_FAILURE;

  // The strtok modify permantly the string, and we still need the "full" command to verify it's last part.
  // The token will change due the strtok on Coordinates_verify, erasing the 2nd part of it.
  strcpy(token_backup, token);

  // Check if there's more arguments than the necessary

  if (token == NULL)
  {
    printf("\nCommand invalid, missing coordinates\n");
    return EXIT_FAILURE;
  }
  // the create and move instructions are different, create has 2 coordinates plus width and height, and moove has 2 coordinates and a position.
  if (Coordinates_verify(token, s_coordinates) == EXIT_FAILURE)
    return EXIT_FAILURE;
  return Verify_last_part(token_backup, s_coordinates);
}

int Coordinates_verify(char *coordinates, SCoordinates *s_coordinates)
{
  char *x_and_y, *token;
  int valid_coordinate = 0;

  if (coordinates == NULL)
    return EXIT_FAILURE;

  // Get the first part of the coordinates x and y.
  token = strtok(coordinates, "+");

  // Get the x value
  x_and_y = strtok(token, ",");

  valid_coordinate = atoi(x_and_y);
  if (valid_coordinate <= 0 || valid_coordinate > 80)
  {
    printf("\nInvalid \"x\" coordinate, notice that 1 <= x <= 80\n");
    return EXIT_FAILURE;
  }
  s_coordinates->x = valid_coordinate;

  // Get the y value
  x_and_y = strtok(NULL, ",");
  if (x_and_y == NULL)
  {
    printf("\nInvalid, missing \"y\" value\n");
    return EXIT_FAILURE;
  }

  valid_coordinate = atoi(x_and_y);
  if (valid_coordinate < 1 || valid_coordinate > 25)
  {
    printf("\nInvalid \"y\" coordinate, notice that 1 <= y <= 25\n");
    return EXIT_FAILURE;
  }
  s_coordinates->y = valid_coordinate;

  // Check if there's more commands than the necessary
  x_and_y = strtok(NULL, ",");
  if (x_and_y != NULL)
  {
    printf("\nInvalid coordinate format, more elements than the necessary (only x, y and p or l and h)\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// By last part I mean the the "l,h" or "p"
int Verify_last_part(char *last_part, SCoordinates *s_coordinates)
{
  char *token;
  int valid_number;

  // getting the las part of the string x,y+l,h or x,y+p.
  token = strtok(last_part, "+");
  token = strtok(NULL, "+");

  if (token == NULL)
  {
    printf("\nInvalid, missing the %s\n", s_coordinates->command == 'c' ? "\"l,h\" values" : "\"p\" value");
    return EXIT_FAILURE;
  }

  if (s_coordinates->command != 'c')
  {
    valid_number = atoi(token);
    if (valid_number <= 0)
    {
      printf("\nInvalid, the \"p\" value while moving must be a number greater than 1\n");
      return EXIT_FAILURE;
    }
    s_coordinates->p = valid_number;
    s_coordinates->l = 0; //
    s_coordinates->h = 0;
  }
  else
  {
    // Get the value of l
    token = strtok(token, ",");

    // check if the sum of x and l - 1 is lower than 80, to respect the canvas boundary.
    valid_number = atoi(token);
    if (valid_number <= 0)
    {
      printf("\nInvalid, the \"l\" value must be a number greater than 0\n");
      return EXIT_FAILURE;
    }
    else if ((valid_number + s_coordinates->x - 1) > 80)
    {
      printf("\nInvalid, rectangle is bigger than the allowed. Notice that the sum of x and l must be lower than 80\n");
      return EXIT_FAILURE;
    }
    else
      s_coordinates->l = valid_number;

    // Get the value of h
    token = strtok(NULL, ",");

    if (token == NULL)
    {
      printf("\nInvalid, missing h value\n");
      return EXIT_FAILURE;
    }
    // check if the sum of y and h - 1 is lower than 25, to respect the canvas boundary.
    valid_number = atoi(token);
    if (valid_number <= 0)
    {
      printf("\nInvalid, the \"h\" value must be a number greater than 0\n");
      return EXIT_FAILURE;
    }
    else if ((valid_number + s_coordinates->y - 1) > 25)
    {
      printf("\nInvalid, rectangle is bigger than the allowed. Notice that the sum of y and h must be lower than 25\n");
      return EXIT_FAILURE;
    }
    else
      s_coordinates->h = valid_number;

    s_coordinates->p = 0;
  }

  // Check if there's any unnecessary extra command
  token = strtok(NULL, ",");
  if (token != NULL)
  {
    printf("\nInvalid, more arguments than the necessary. When creating please insert only \"x,y+l,h\", when moving, only \"x,y+p\"\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int Valid_amount_of_arguments(char *str)
{
  int i = 0;

  str = strtok(str, "+");
  while (str)
  {
    str = strtok(NULL, "+");
    i++;
  }
  if (i == 2)
    return EXIT_SUCCESS;

  printf("\nInvalid number of arguments between the \"+\" signal\n");
  return EXIT_FAILURE;
}