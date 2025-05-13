#include "../headers/commands.h"

LRect *Add_rect(LRect *l_rect, SCoordinates *s_condinates)
{
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
  new->id = Random_id(l_rect);

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

LRect *Free_rect(LRect *l_rect)
{
  LRect *aux;

  aux = l_rect->next;
  free(l_rect);
  l_rect = aux;

  return l_rect;
}

LRect *Free_all_rect(LRect *l_rect)
{
  while (l_rect)
    l_rect = Free_rect(l_rect);

  return l_rect;
}
