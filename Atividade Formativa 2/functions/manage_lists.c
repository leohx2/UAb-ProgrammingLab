#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/listas.h"

LCatalog *Add_new(LCatalog *list, LCatalog *aux)
{
  LCatalog *novo;
  static int id = 1;

  novo = (LCatalog *)malloc(sizeof(LCatalog));

  if (novo != NULL)
  {
    novo->category = (char *)malloc(sizeof(char) * strlen(aux->category));
    novo->title = (char *)malloc(sizeof(char) * strlen(aux->title));
    novo->pegi = (char *)malloc(sizeof(char) * strlen(aux->pegi));

    if (novo->category == NULL || novo->title == NULL || novo->pegi == NULL)
    {
      printf("Error during memory allocation.\nLocation: manage_list.c, 'Add_new()'\n");
      return NULL;
    }

    strcpy(novo->category, aux->category);
    strcpy(novo->title, aux->title);
    strcpy(novo->pegi, aux->pegi);
    novo->duration = aux->duration;
    novo->views = aux->views;
    novo->next = list;
    novo->id = id++;

    return novo;
  }

  return list;
}

LCatalog *Free_current_list(LCatalog *list)
{
  LCatalog *aux;

  if (list)
  {
    aux = list->next;
    free(list);

    return aux;
  }

  return list;
}

LCatalog *Free_all_lists(LCatalog *list)
{
  while (list)
    list = Free_current_list(list);

  return list;
}

void Print_catalog(LCatalog *list)
{
  while (list)
  {
    printf("\nMovie: %s\n", list->title);
    printf("Category: %s, duration: %d, PEGI: %s, views: %d\n", list->category, list->duration, list->pegi, list->views);
    printf("ID: %d\n", list->id);
    list = list->next;
  }
}