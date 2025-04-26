#include "../headers/drawing.h"

void Update_rect(LRect *l_rect)
{
  LRect *aux;

  aux = l_rect;
  while (aux)
  {
    while (aux->y > 1 && Valid_action(l_rect, aux->x, aux->y - 1, aux->h, aux->l, aux->id) == EXIT_SUCCESS)
      aux->y -= 1;
    aux = aux->next;
  }
}

void Update_matrix(int m[MAX_LINES][MAX_COL], LRect *l_rect)
{
  LRect *aux;
  int i, c;

  aux = l_rect;
  while (aux)
  {
    for (i = 0; i < aux->l; i++)
    {
      for (c = 0; c < aux->h; c++)
      {
        if (i == 0 || aux->x + i == aux->x + aux->l - 1 || c == 0 || aux->y + c == aux->y + aux->h - 1)
          m[aux->y + c - 1][aux->x + i - 1] = -1;
        else
          m[aux->y + c - 1][aux->x + i - 1] = aux->id;
      }
    }
    aux = aux->next;
  }
}

void Draw_matrix(int m[MAX_LINES][MAX_COL])
{
  int l, c;

  for (l = MAX_LINES - 1; l >= 0; l--)
  {
    printf("\n");
    for (c = 0; c < MAX_COL; c++)
    {
      if (m[l][c] == -1)
        printf("X ");
      else
        printf(". ");
    }
  }
}

// When creating id must be 0.
int Valid_action(LRect *l_rect, int x, int y, int h, int l, int id)
{
  LRect *aux;

  aux = l_rect;
  while (aux)
  {
    if (aux->id == id || (x >= aux->x + aux->l || x + l <= aux->x) ||
        (y >= aux->y + aux->h || y + h <= aux->h))
      aux = aux->next;
    else
    {
      if (id == 0)
        printf("\nInvalid coordinates, theres a rectangle in the desired area\n");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}