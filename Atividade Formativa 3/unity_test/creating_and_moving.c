/*
Here we'll test if the code will allow the user to:
create only 10 rectangles;
create beyond the "world" boundaries
create a rect where there's another rectangle
move in way that is not supposed
*/

#include "../headers/helper.h"
#include "../headers/drawing.h"
#include "../headers/commands.h"

int Max_rectangles(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL]);
int Outside_boundaries(SCoordinates *s_coordinates);
int Rectangle_areas(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL]);
int Moving_rectangle(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL]);
int List_size(LRect *l_rect);

int main()
{
  SCoordinates *s_coordinates;
  int matrix[MAX_LINES][MAX_COL];
  int test1, test2, test3, test4;

  s_coordinates = (SCoordinates *)malloc(sizeof(SCoordinates));
  if (s_coordinates == NULL)
  {
    printf("Error, insuficient memory (coordinate)");
    return EXIT_FAILURE;
  }

  printf("\n------------------------------------------------------------------------------------------\n");
  printf("\nStarting tests, you will se the output from every function, don't worry about that part\n");
  printf("\n------------------------------------------------------------------------------------------\n");
  test1 = Max_rectangles(s_coordinates, matrix);
  test2 = Outside_boundaries(s_coordinates);
  test3 = Rectangle_areas(s_coordinates, matrix);
  test4 = Moving_rectangle(s_coordinates, matrix);

  printf("\n\n---------------------------\n");
  printf("\n\tRESULTS\n");
  printf("\n---------------------------\n");
  printf("\n1st test, maximum number of rectangles: %s\n", test1 == EXIT_SUCCESS ? "Sucess!" : "Failed.");
  printf("\n2nd test, world boundaries: %s\n", test2 == EXIT_SUCCESS ? "Sucess!" : "Failed.");
  printf("\n3rd test, rectangles over rectangles: %s\n", test3 == EXIT_SUCCESS ? "Sucess!" : "Failed.");
  printf("\n4th test, moving rectangles: %s\n", test4 == EXIT_SUCCESS ? "Sucess!" : "Failed.");

  free(s_coordinates);
  s_coordinates = NULL;
}

// First test, the maximum amount of rectangles
int Max_rectangles(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL])
{
  LRect *l_rect;
  int i;

  l_rect = NULL;

  printf("\n1st Test, checking how many rectangles can the user create\n");
  Initialize_matrix(matrix);

  for (i = 1; i < 15; i++)
  {
    s_coordinates->h = i + 3;
    s_coordinates->l = 3;
    s_coordinates->x = i * 3;
    s_coordinates->y = 1;
    if (Rectangle_len(l_rect) == 10)
      printf("\nLimit of 10 rectangles reached. To be able to create a new reactangle, please delete one (or more) rectangle(s)\n");
    else
    {
      if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
        l_rect = Add_rect(l_rect, s_coordinates);
    }
  }

  Update_matrix(matrix, l_rect);
  Draw_matrix(matrix);

  if (List_size(l_rect) != 10)
  {
    l_rect = Free_all_rect(l_rect);
    return EXIT_FAILURE;
  }

  l_rect = Free_all_rect(l_rect);
  return EXIT_SUCCESS;
}

int List_size(LRect *l_rect)
{
  LRect *aux;
  int size = 0;

  aux = l_rect;

  while (aux)
  {
    size++;
    aux = aux->next;
  }

  return size;
}

int Outside_boundaries(SCoordinates *s_coordinates)
{
  int good_to_go = EXIT_SUCCESS;
  char *input;

  printf("\n2nd Test, checking the \"world\" limits and sizes\n");
  input = (char *)malloc(sizeof(char) * 18);
  if (input == NULL)
  {
    printf("\nError while allocating memory (2nd test creating and moving)\n");
    return EXIT_FAILURE;
  }

  // A x lower than 1
  strcpy(input, "create -1,1+3,5");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError, creating a rectangle with the x lower than 1\n");
    good_to_go = EXIT_FAILURE;
  }

  // A x greater than 80
  strcpy(input, "create 81,1+3,5");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError, creating a rectangle with the x greater than 80\n");
    good_to_go = EXIT_FAILURE;
  }

  // A y lower than 1
  strcpy(input, "create 1,-1+3,5");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError, creating a rectangle with the y lower than 1\n");
    good_to_go = EXIT_FAILURE;
  }

  // A y greater than 25
  strcpy(input, "create 1,26+3,5");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError, creating a rectangle with the y greater than 25\n");
    good_to_go = EXIT_FAILURE;
  }

  // Incorret height and length, should be greater than 2
  strcpy(input, "create 1,1+1,5");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError, creating a rectangle with an invalid length\n");
    good_to_go = EXIT_FAILURE;
  }

  strcpy(input, "create 1,1+3,1");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError, creating a rectangle with an invalid height\n");
    good_to_go = EXIT_FAILURE;
  }

  // Checking if the user can create a rectangle with valid x and y values but h, l or h and l invalid
  strcpy(input, "create 76,1+6,5");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError, creating a rectangle with an invalid size, invalid length\n");
    good_to_go = EXIT_FAILURE;
  }

  strcpy(input, "create 1,26+5,5");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError, creating a rectangle with an invalid size, invalid height\n");
    good_to_go = EXIT_FAILURE;
  }

  strcpy(input, "create 76,26+5,5");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError, creating a rectangle with an invalid size, invalid height and length\n");
    good_to_go = EXIT_FAILURE;
  }

  free(input);
  return good_to_go;
}

int Rectangle_areas(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL])
{
  LRect *l_rect;
  int good_to_go = EXIT_SUCCESS;

  l_rect = NULL;
  printf("\n3rd Test, checking if the user can create a rectangle over another rectangle\n");
  Initialize_matrix(matrix);

  // Creating the first rectangle
  s_coordinates->h = 5;
  s_coordinates->l = 12;
  s_coordinates->x = 1;
  s_coordinates->y = 1;
  if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
    l_rect = Add_rect(l_rect, s_coordinates);

  // Trying the first invalid command
  s_coordinates->h = 5;
  s_coordinates->l = 12;
  s_coordinates->x = 8;
  s_coordinates->y = 3;
  if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
  {
    l_rect = Add_rect(l_rect, s_coordinates);
    printf("\nError creating an rectangle over another, test 1\n");
    good_to_go = EXIT_FAILURE;
  }

  // Trying the second invalid command, checking the limits
  s_coordinates->h = 5;
  s_coordinates->l = 12;
  s_coordinates->x = 12;
  s_coordinates->y = 5;
  if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
  {
    l_rect = Add_rect(l_rect, s_coordinates);
    printf("\nError creating an rectangle over another, test 1\n");
    good_to_go = EXIT_FAILURE;
  }

  if (List_size(l_rect) > 1)
    good_to_go = EXIT_FAILURE;

  l_rect = Free_all_rect(l_rect);
  return good_to_go;
}

int Moving_rectangle(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL])
{
  LRect *l_rect, *aux;
  int id_1, id_2;
  int good_to_go = EXIT_SUCCESS;

  l_rect = NULL;
  printf("\n4th Test, checking if the user can move to wrong directions\n");
  Initialize_matrix(matrix);

  // Creating two rectangles next to each other
  s_coordinates->h = 3;
  s_coordinates->l = 3;
  s_coordinates->x = 1;
  s_coordinates->y = 1;
  if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
    l_rect = Add_rect(l_rect, s_coordinates);

  s_coordinates->h = 5;
  s_coordinates->l = 3;
  s_coordinates->x = 4;
  s_coordinates->y = 1;
  if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
    l_rect = Add_rect(l_rect, s_coordinates);

  // Trying to move the 1st rect to the right
  s_coordinates->x = 1;
  s_coordinates->y = 1;
  s_coordinates->p = 1;
  s_coordinates->command = 'r';
  Move_rectangle(l_rect, s_coordinates);

  // Trying to move the 1st rect to the left
  s_coordinates->x = 1;
  s_coordinates->y = 1;
  s_coordinates->p = 1;
  s_coordinates->command = 'l';
  Move_rectangle(l_rect, s_coordinates);

  // Trying to move the 1st rect to the left
  s_coordinates->x = 4;
  s_coordinates->y = 1;
  s_coordinates->p = 1;
  s_coordinates->command = 'l';
  Move_rectangle(l_rect, s_coordinates);

  // Trying to move the 2nd rect to the right, should work
  s_coordinates->x = 4;
  s_coordinates->y = 1;
  s_coordinates->p = 2;
  s_coordinates->command = 'r';
  Move_rectangle(l_rect, s_coordinates);

  aux = l_rect;

  // 2nd rectangle
  if (aux->x != 6)
  {
    printf("\nError while moving the 2nd rectangle\n");
    good_to_go = EXIT_FAILURE;
  }

  aux = aux->next;
  // 1st rectangle
  if (aux->x != 1)
  {
    printf("\nError while moving the 1st rectangle, it wasn't supposed to move\n");
    good_to_go = EXIT_FAILURE;
  }

  return good_to_go;
}
