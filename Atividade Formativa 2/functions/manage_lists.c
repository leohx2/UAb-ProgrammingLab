#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/listas.h"
#include "../header/helper.h"

#define MAXSTR 255

LCatalog *Add_title(LCatalog *list, LCatalog *aux)
{
  LCatalog *novo;

  novo = (LCatalog *)malloc(sizeof(LCatalog));

  if (novo != NULL)
  {
    novo->title = (char *)malloc(sizeof(char) * strlen(aux->title) + 1);
    novo->category = (char *)malloc(sizeof(char) * strlen(aux->category) + 1);
    novo->pegi = (char *)malloc(sizeof(char) * strlen(aux->pegi) + 1);

    if (novo->category == NULL || novo->title == NULL || novo->pegi == NULL)
    {
      printf("Error during memory allocation.\nLocation: manage_list.c, 'Add_new()'\n");
      return NULL;
    }

    strcpy(novo->title, aux->title);
    strcpy(novo->category, aux->category);
    strcpy(novo->pegi, aux->pegi);
    novo->duration = aux->duration;
    novo->views = aux->views;
    novo->next = list;
    novo->id = aux->id;
    return novo;
  }

  return list;
}

// Add in order
LCatalog *Add_new(LCatalog *list, LCatalog *aux)
{
  LCatalog *hold_1st_p;

  // 1st item to be added
  if (list == NULL)
  {
    list = Add_title(list, aux);
    return list;
  }

  hold_1st_p = list;

  while (list && list->next)
    list = list->next;

  list->next = Add_title(list->next, aux);
  return hold_1st_p;
}

LCatalog *Free_current_list(LCatalog *list)
{
  LCatalog *aux;

  if (list)
  {
    aux = list->next;
    free(list->category);
    free(list->title);
    free(list->pegi);
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
    Print_title_details(list);
    list = list->next;
  }
}

TUser *Free_user(TUser *t_user)
{
  if (t_user)
  {
    if (t_user->username)
      free(t_user->username);
    free(t_user);
  }
  return NULL;
}