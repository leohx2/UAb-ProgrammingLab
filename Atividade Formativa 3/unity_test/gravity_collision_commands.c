/*
Here we'll test:
The gravity system;
If a rectangle stays in the top of another rectangle
Invalid commands;
Collision;
*/

#include "../headers/helper.h"
#include "../headers/drawing.h"
#include "../headers/commands.h"
#include <time.h>

int Gravity(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL]);
int Stacking_rectangles(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL]);
int Wrong_command(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL]);
int Collision_test(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL]);
int Efolio_B_Test(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL]);
int main()
{
  SCoordinates *s_coordinates;
  int matrix[MAX_LINES][MAX_COL];
  int test1, test2, test3, test4, test5;

  s_coordinates = (SCoordinates *)malloc(sizeof(SCoordinates));
  if (s_coordinates == NULL)
  {
    printf("Error, insuficient memory (coordinate)");
    return EXIT_FAILURE;
  }

  printf("\n------------------------------------------------------------------------------------------\n");
  printf("\nStarting tests, here you will see the output from every function and, after that, the result\n");
  printf("\n------------------------------------------------------------------------------------------\n");

  test1 = Gravity(s_coordinates, matrix);
  test2 = Stacking_rectangles(s_coordinates, matrix);
  test3 = Wrong_command(s_coordinates, matrix);
  test4 = Collision_test(s_coordinates, matrix);
  test5 = Efolio_B_Test(s_coordinates, matrix);

  printf("\n\n---------------------------\n");
  printf("\n\tRESULTS\n");
  printf("\n---------------------------\n");
  printf("\n1st test, checking if the gravity works: %s\n", test1 == EXIT_SUCCESS ? "Sucess!" : "Failed.");
  printf("\n2nd Test, stacking rectangles: %s\n", test2 == EXIT_SUCCESS ? "Sucess!" : "Failed.");
  printf("\n3rd test, wrong commands: %s\n", test3 == EXIT_SUCCESS ? "Sucess!" : "Failed.");
  printf("\n4th test, collision: %s\n", test4 == EXIT_SUCCESS ? "Sucess!" : "Failed.");
  printf("\n5th efolioB suggestion: %s\n", test5 == EXIT_SUCCESS ? "Sucess!" : "Failed.");

  free(s_coordinates);
  s_coordinates = NULL;
}

// Here we'll create 10 rectangles, they will all have y > 1 but they wont be at top of another rectangle,
// in the end, they should be at the floor (y == 1).
int Gravity(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL])
{
  LRect *l_rect, *aux;
  int i, random_number, good_to_go = EXIT_SUCCESS;

  l_rect = NULL;

  printf("\n1st Test, checking if the gravity works\n");
  Initialize_matrix(matrix);
  srand(time(NULL));

  for (i = 1; i <= 10; i++)
  {
    s_coordinates->h = 5;
    s_coordinates->l = 3;
    s_coordinates->x = i * 4;

    random_number = (rand() % 10) + 2;
    s_coordinates->y = random_number;

    if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
      l_rect = Add_rect(l_rect, s_coordinates);
  }

  printf("\nFor testing purposes only, here's the matrix without gravity:\n");
  Update_matrix(matrix, l_rect);
  Draw_matrix(matrix);

  printf("\n\nThe result after gravity: \n\n");
  Initialize_matrix(matrix);
  Update_rect(l_rect);
  Update_matrix(matrix, l_rect);
  Draw_matrix(matrix);

  aux = l_rect;

  while (aux)
  {
    if (aux->y > 1)
    {
      good_to_go = EXIT_FAILURE;
      printf("\nError, the rectangle %d is not on the right position\n", aux->id);
    }
    aux = aux->next;
  }

  l_rect = Free_all_rect(l_rect);
  return good_to_go;
}

int Stacking_rectangles(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL])
{
  LRect *l_rect, *aux;
  int i, good_to_go = EXIT_SUCCESS;

  printf("\n\n2nd Test, stacking rectangles:\n");
  Initialize_matrix(matrix);

  for (i = 1; i <= 8; i++)
  {
    s_coordinates->h = 3;
    s_coordinates->l = 5;
    s_coordinates->x = 1;
    s_coordinates->y = i * 3;

    if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
      l_rect = Add_rect(l_rect, s_coordinates);
  }

  Update_rect(l_rect);
  Update_matrix(matrix, l_rect);
  Draw_matrix(matrix);

  aux = l_rect;
  i = 22;
  while (aux)
  {
    if (aux->y != i)
    {
      good_to_go = EXIT_FAILURE;
      printf("\nError, the rectangle %d is not stacked properly\n", aux->id);
    }
    aux = aux->next;
    i -= 3;
  }

  l_rect = Free_all_rect(l_rect);

  return good_to_go;
}

// Here we'll try a few
int Wrong_command(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL])
{
  LRect *l_rect, *aux;
  char *input;
  int good_to_go = EXIT_SUCCESS;

  printf("\n\n3rd Test, testing wrong commands:\n");

  input = (char *)malloc(sizeof(char) * 20);
  if (input == NULL)
  {
    printf("\nError while allocating memory (3rd test gravity, collison and commands)\n");
    return EXIT_FAILURE;
  }

  printf("\n\"create 1,1+3,4+1\":\n");
  strcpy(input, "create 1,1+3,4+1");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 1\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"create 1,1\":\n");
  strcpy(input, "create 1,1");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 2\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"create 1,1,1+3\":\n");
  strcpy(input, "create 1,1,1+3");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 3\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"create 1,1+3,3,3\":\n");
  strcpy(input, "create 1,1+3,3,3");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 4\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"create 1,1+1,2\":\n");
  strcpy(input, "create 1,1+1,2");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 5\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"create 1,1+2,1\":\n");
  strcpy(input, "create 1,1+2,1");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 6\n");
    good_to_go = EXIT_FAILURE;
  }

  // Creating a rectangle for the next tests
  s_coordinates->h = 5;
  s_coordinates->l = 5;
  s_coordinates->x = 1;
  s_coordinates->y = 1;

  if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
    l_rect = Add_rect(l_rect, s_coordinates);

  printf("\n\"moveleft 1,1+3,4\":\n");
  strcpy(input, "moveleft 1,1+3,4");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 7\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"moveright 1,1+3,4\":\n");
  strcpy(input, "moveright 1,1+3,4");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 8\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"moveright 1,1,3\":\n");
  strcpy(input, "moveright 1,1,3");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 9\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"moveright 1,1,3+1\":\n");
  strcpy(input, "moveright 1,1,3+1");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 10\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"moveright 1+1+1\":\n");
  strcpy(input, "moveright 1+1+1");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 11\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"moveright 1,1\":\n");
  strcpy(input, "moveright 1,1");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 12\n");
    good_to_go = EXIT_FAILURE;
  }

  printf("\n\"moveright 1,1+1+1\":\n");
  strcpy(input, "moveright 1,1+1+1");
  if (Command_verify(input, s_coordinates) == EXIT_SUCCESS)
  {
    printf("\nError while creating, input 13\n");
    good_to_go = EXIT_FAILURE;
  }

  free(input);
  l_rect = Free_all_rect(l_rect);
  return good_to_go;
}

int Collision_test(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL])
{
  LRect *l_rect;
  int i, collisions = 0, good_to_go = EXIT_SUCCESS;
  int x[5] = {1, 1, 11, 11, 6};
  int y[5] = {1, 4, 1, 4, 1};
  int h[5] = {3, 3, 3, 3, 6};
  int l[5] = {5, 5, 5, 5, 5};

  l_rect = NULL;
  printf("\n\n4th Test, collisions:\n");
  Initialize_matrix(matrix);

  /*creating 5 rectangles
  which should give us 4 collisions 2 in the right, 2 in the left
  */
  for (i = 0; i < 5; i++)
  {
    s_coordinates->x = x[i];
    s_coordinates->y = y[i];
    s_coordinates->l = 5;
    s_coordinates->h = h[i];
    if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
    {
      l_rect = Add_rect(l_rect, s_coordinates);
      collisions += Collision_warning(l_rect, l_rect);
    }
  }

  Update_matrix(matrix, l_rect);
  Draw_matrix(matrix);

  l_rect = Free_all_rect(l_rect);

  if (collisions != 4)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int Efolio_B_Test(SCoordinates *s_coordinates, int matrix[MAX_LINES][MAX_COL])
{
  LRect *l_rect;
  int collisions = 0, i;
  int x[4] = {1, 5, 22, 21};
  int y[4] = {1, 6, 10, 12};
  int l[4] = {15, 12, 6, 6};
  int h[4] = {5, 3, 9, 7};

  l_rect = NULL;
  /*
  create 1,1+15,5
  create 5,6+12,3
  create 22,10+6,9
  create 21,12+6,7
  moveleft 23,11+2
  Moveright 10,8+2
  Delete 23,2

  1 collision at the end
  */

  for (i = 0; i < 4; i++)
  {
    s_coordinates->x = x[i];
    s_coordinates->y = y[i];
    s_coordinates->l = l[i];
    s_coordinates->h = h[i];
    if (Valid_action(l_rect, s_coordinates->x, s_coordinates->y, s_coordinates->h, s_coordinates->l, -10) == EXIT_SUCCESS)
    {
      l_rect = Add_rect(l_rect, s_coordinates);
      collisions += Collision_warning(l_rect, l_rect);
    }
    Update_rect(l_rect);
  }

  s_coordinates->x = 23;
  s_coordinates->y = 11;
  s_coordinates->p = 2;
  s_coordinates->command = 'l';
  Move_rectangle(l_rect, s_coordinates);
  Update_rect(l_rect);

  s_coordinates->x = 10;
  s_coordinates->y = 8;
  s_coordinates->command = 'r';
  Move_rectangle(l_rect, s_coordinates);
  Update_rect(l_rect);

  s_coordinates->x = 23;
  s_coordinates->y = 2;
  s_coordinates->command = 'd';
  l_rect = Delete_rectangle(l_rect, s_coordinates);
  Update_rect(l_rect);

  collisions += Collision_warning(l_rect, l_rect);

  Initialize_matrix(matrix);
  Update_matrix(matrix, l_rect);
  Draw_matrix(matrix);

  l_rect = Free_all_rect(l_rect);

  if (collisions != 1)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}