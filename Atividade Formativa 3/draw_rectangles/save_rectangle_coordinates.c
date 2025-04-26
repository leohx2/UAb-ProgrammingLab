#include "../headers/drawing.h"

LRect *Add_rect(LRect *l_rect, SCoordinates *s_condinates)
{
  static int id = 1;
  LRect *new, *aux;

  new = (LRect *)malloc(sizeof(LRect));

  if (new == NULL)
  {
    printf("\nError memory allocation, rectangle\n");
    return NULL;
  }

  new->h = s_condinates->h;
  new->l = s_condinates->l;
  new->x = s_condinates->x;
  new->y = s_condinates->y;
  new->id = id++;

  if (l_rect == NULL)
  {
    new->next = l_rect;
    return new;
  }

  aux = l_rect;
  while (aux && aux->next)
    aux = aux->next;
  new->next = NULL;
  aux->next = new;
  return l_rect;
}

LRect *Free_all_rect(LRect *l_rect)
{
  LRect *aux;

  while (l_rect)
  {
    aux = l_rect->next;
    free(l_rect);
    l_rect = aux;
  }

  return l_rect;
}
