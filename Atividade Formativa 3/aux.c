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

int Is_there_a_rectangle(LRect *l_rect, SCoordinates *s_coordinates)
{
  if (s_coordinates->x >= l_rect->x && s_coordinates->x < l_rect->x + l_rect->l && s_coordinates->y >= l_rect->y && s_coordinates->y < l_rect->y + l_rect->h)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}