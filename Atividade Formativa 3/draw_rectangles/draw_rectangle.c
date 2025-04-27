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
  LRect *aux = l_rect;
  int valid;

  while (aux)
  {
    if (id != aux->id)
    {
      valid = (x + l <= aux->x) || (x >= aux->x + aux->l) || (y + h <= aux->y) || (y >= aux->y + aux->h);

      if (!valid)
      {
        if (id == 0)
          printf("\nInvalid coordinates, there's a rectangle in the desired area\n");
        return EXIT_FAILURE;
      }
    }
    aux = aux->next;
  }
  return EXIT_SUCCESS;
}

void Move_rectangle(LRect *l_rect, SCoordinates *s_coordinates)
{
  LRect *aux;

  aux = l_rect;

  while (aux)
  {
    // Finding the rect that contains the right position.
    if (s_coordinates->x >= aux->x && s_coordinates->x < aux->x + aux->l && s_coordinates->y >= aux->y && s_coordinates->y < aux->y + aux->h)
    {
      if (s_coordinates->command == 'r')
      {
        if (aux->x + aux->l + s_coordinates->p - 1 <= MAX_COL && Valid_action(l_rect, aux->x + s_coordinates->p, aux->y, aux->h, aux->y, aux->id) == EXIT_SUCCESS)
          aux->x += s_coordinates->p;
        else
          printf("\nIt's not possible to move the rectangle %d, %d positions to the right\n", aux->id, s_coordinates->p);
      }
      else if (s_coordinates->command == 'l')
      {
        if (aux->x - s_coordinates->p > 0 && Valid_action(l_rect, aux->x - s_coordinates->p, aux->y, aux->h, aux->l, aux->id) == EXIT_SUCCESS)
          aux->x -= s_coordinates->p;
        else
          printf("\nIt's not possible to move the rectangle %d, %d positions to the left\n", aux->id, s_coordinates->p);
      }
      return;
    }

    aux = aux->next;
  }

  printf("\nRectangle not found\n");
}