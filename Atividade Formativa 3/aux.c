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