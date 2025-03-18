#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/listas.h"

#define MAXSTR 255

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

/*
Content_to_edit will be one of values bellow:
0 - to edit title
1 - Category
2 - duration
3 - PEGI
4 - views
5 - Everything
*/
int Edit_item(LCatalog *l_catalog, int content_to_edit, int id)
{
  char aux[MAXSTR];
  int int_aux;

  while (l_catalog && l_catalog->id != id)
    l_catalog = l_catalog->next;

  if (l_catalog)
  {
    if (content_to_edit == 1 || content_to_edit == 6)
    {
      printf("\nCurrent title: %s\nNew title (don't use commas): ", l_catalog->title);
      scanf(" %s", aux);
      if (l_catalog->title)
        free(l_catalog->title);
      l_catalog->title = strdup(aux);
    }
    if (content_to_edit == 2 || content_to_edit == 6)
    {
      printf("\nCurrent category: %s\nNew category (if it has more than 1 category, don't use commas use '\\' instead): ", l_catalog->category);
      scanf(" %s", aux);
      if (l_catalog->category)
        free(l_catalog->category);
      l_catalog->category = strdup(aux);
    }
    if (content_to_edit == 3 || content_to_edit == 6)
    {
      printf("\nCurrent duration %d\nNew duration (only numbers, greater than 0, allowed): ", l_catalog->duration);
      scanf("%d", &int_aux);
      if (int_aux == 0)
        printf("Format not suported, only numbers (greater than 0) allowed\n");
      else
        l_catalog->duration = int_aux;
    }
    if (content_to_edit == 4 || content_to_edit == 6)
    {
      printf("\nCurrent PEGI: %s\nNew PEGI: ", l_catalog->pegi);
      scanf(" %s", aux);
      if (l_catalog->pegi)
        free(l_catalog->pegi);
      l_catalog->pegi = strdup(aux);
    }
    if (content_to_edit == 5 || content_to_edit == 6)
    {
      printf("\nCurrent number of views %d\nNew number of views: ", l_catalog->duration);
      scanf("%d", &int_aux);
      l_catalog->duration = atoi(aux);
    }
  }
  else
    return 0;
  return 1;
}