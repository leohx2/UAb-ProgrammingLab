#include "headers/helper.h"
#include "headers/drawing.h"
#include <ctype.h>
#include <time.h>

void Str_to_lower(char *s)
{
  int i;

  for (i = 0; s[i]; i++)
    s[i] = tolower(s[i]);
}

// Get user input without the '\n' in the end
void Get_str_input(char *input)
{
  scanf(" %[^\n]s", input);

  if (input[strlen(input) - 1] == '\n')
    input[strlen(input) - 1] = '\0';
}

void Initialize_matrix(int m[MAX_LINES][MAX_COL])
{
  int l, c;

  for (l = 0; l < MAX_LINES; l++)
    for (c = 0; c < MAX_COL; c++)
      m[l][c] = -2;
}

// Generates a random ID between 0 an 9;
int Random_id(LRect *l_rect)
{
  LRect *aux;
  int id_limit = 10;
  int id;

  srand(time(NULL));
  id = rand() % 10;
  if (l_rect == NULL)
    return id;

  // Verify if there's no other ID like that
  aux = l_rect;
  while (aux)
  {
    if (aux->id == id)
    {
      id = rand() % 10;

      // restart the verification
      aux = l_rect;
    }
    else
      aux = aux->next;
  }

  return id;
}

int Rectangle_len(LRect *l_rect)
{
  LRect *aux = l_rect;
  int size = 0;

  while (aux)
  {
    aux = aux->next;
    size++;
  }

  return size;
}

void Print_commands()
{
  printf("(x,y >=1, x <= 80 and y <= 25)\n");
  printf("-> create x,y+l,h - Creates a rectangle where (x,y) are the coordinates of the bottom-left corner, and (l,h) are the length and height, respectively.\n");
  printf("-> moveright x,y+p - Moves the rectangle located at coordinates (x,y) to the right by p positions.\n");
  printf("-> moveleft x,y+p - Moves the rectangle containing the point (x,y) to the left by p positions.\n");
  printf("-> delete x,y - Delete the rectangle containning the point (x,y).\n");
  printf("-> exit\n-> help\n");
}

// mode 0: receive only x and y.
// mode 1: receive all the coordiantes x,y,l and h
int Is_there_a_rectangle(LRect *l_rect, SCoordinates *s_coordinates, int mode)
{

  int dont_exist;

  if (mode == 0)
  {
    if (s_coordinates->x >= l_rect->x && s_coordinates->x < l_rect->x + l_rect->l && s_coordinates->y >= l_rect->y && s_coordinates->y < l_rect->y + l_rect->h)
      return EXIT_SUCCESS;
    else
      return EXIT_FAILURE;
  }

  dont_exist = (s_coordinates->x + s_coordinates->l <= l_rect->x) || (s_coordinates->x >= l_rect->x + l_rect->l) || (s_coordinates->y + s_coordinates->h <= l_rect->y) || (s_coordinates->y >= l_rect->y + l_rect->h);
  if (!dont_exist)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}

/* We have already the "Valid_action" function which checks if it's possible to move the rect
  in a certain direction. We can reuse it here, if there's a collision it means the rectangle can't
  go even left or right a single "step".
  We're only counting side collisions.
*/
void Collision_detection(LRect *all_rectangles, LRect *current_rectangle)
{
  LRect *aux;
  SCoordinates *s_coordinates_1, *s_coordinates_2;

  s_coordinates_1 = NULL;
  s_coordinates_2 = NULL;

  // Check if there's a rectangle on the right
  if (Valid_action(all_rectangles, current_rectangle->x + 1, current_rectangle->y, current_rectangle->h, current_rectangle->l, current_rectangle->id) == EXIT_FAILURE)
  {
    s_coordinates_1 = (SCoordinates *)malloc(sizeof(SCoordinates));
    if (s_coordinates_1 == NULL)
    {
      printf("Error, insuficient memory (collision_dection1)");
      return;
    }
    s_coordinates_1->h = current_rectangle->h;
    s_coordinates_1->l = current_rectangle->l;
    s_coordinates_1->x = current_rectangle->x + 1;
    s_coordinates_1->y = current_rectangle->y;
  }

  // Check if there's a rectangle on the left
  if (Valid_action(all_rectangles, current_rectangle->x - 1, current_rectangle->y, current_rectangle->h, current_rectangle->l, current_rectangle->id) == EXIT_FAILURE)
  {
    s_coordinates_2 = (SCoordinates *)malloc(sizeof(SCoordinates));
    if (s_coordinates_2 == NULL)
    {
      printf("Error, insuficient memory (collision_dection2)");
      return;
    }
    s_coordinates_2->h = current_rectangle->h;
    s_coordinates_2->l = current_rectangle->l;
    s_coordinates_2->x = current_rectangle->x - 1;
    s_coordinates_2->y = current_rectangle->y;
  }

  aux = all_rectangles;

  while (aux)
  {
    if (aux->id != current_rectangle->id)
    {
      if (s_coordinates_1 != NULL && Is_there_a_rectangle(aux, s_coordinates_1, 1) == EXIT_SUCCESS)
        printf("-> Warning, the rectangle %d is now colliding with the %d on the right\n", current_rectangle->id, aux->id);
      if (s_coordinates_2 != NULL && Is_there_a_rectangle(aux, s_coordinates_2, 1) == EXIT_SUCCESS)
        printf("-> Warning, the rectangle %d is now colliding with the %d on the left\n", current_rectangle->id, aux->id);
    }
    aux = aux->next;
  }

  if (s_coordinates_1)
    free(s_coordinates_1);
  if (s_coordinates_2)
    free(s_coordinates_2);
}

// We don't want the collision detection system to be called if the "gravity" can act on it,
// in that case, we check if the rect is already on the bottom or on the top of another.
void Collision_warning(LRect *all_rect, LRect *current_rect)
{
  SCoordinates *s_coordinates;

  s_coordinates = (SCoordinates *)malloc(sizeof(SCoordinates));
  if (s_coordinates == NULL)
  {
    printf("Error, insuficient memory (coordinate collision_warning)");
    return;
  }

  s_coordinates->h = current_rect->h;
  s_coordinates->l = current_rect->l;
  s_coordinates->x = current_rect->x;
  s_coordinates->y = current_rect->y - 1;

  if (current_rect->y == 1 || Valid_action(all_rect, current_rect->x, current_rect->y - 1, current_rect->h, current_rect->l, current_rect->id) == EXIT_FAILURE)
    Collision_detection(all_rect, current_rect);

  free(s_coordinates);
}