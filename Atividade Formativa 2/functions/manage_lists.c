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

// USER
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

// INTERACTIONS
LInteractions *New_interaction(LInteractions *l_interaction, char action, int movie_id)
{
  LInteractions *new;

  new = (LInteractions *)malloc(sizeof(LInteractions));

  if (new != NULL)
  {
    // add from the newest to the oldest
    new->movie_id = movie_id;
    new->action = action;
    new->next = l_interaction;

    return new;
  }

  return l_interaction;
}

LInteractions *Free_all_interactions(LInteractions *l_interactions)
{
  LInteractions *aux;

  while (l_interactions)
  {
    aux = l_interactions->next;
    free(l_interactions);
    l_interactions = aux;
  }

  return l_interactions;
}

LFavorite *Add_new_favorite(LFavorite *l_favorite, int list_id, int title_id, char *list_name, char *movie_name)
{
  LFavorite *new;

  new = (LFavorite *)malloc(sizeof(LFavorite));
  if (new == NULL)
  {
    printf("Error memory allocation. Add_new_favorite.");
    return l_favorite;
  }
  new->id = list_id;
  new->title_id = title_id;
  new->name = (char *)malloc(sizeof(char) * strlen(list_name) + 1);
  strcpy(new->name, list_name);
  new->movie_name = (char *)malloc(sizeof(char) * strlen(movie_name) + 1);
  strcpy(new->movie_name, movie_name);
  new->next = l_favorite;

  return new;
}

// Add in order, from the oldest to the newest
LFavorite *Add_favorite_in_order(LFavorite *l_favorite, int list_id, int title_id, char *list_name, char *movie_name)
{
  LFavorite *hold_1st_p;

  // Check if there's no items
  if (l_favorite == NULL)
  {
    l_favorite = Add_new_favorite(l_favorite, list_id, title_id, list_name, movie_name);
    return l_favorite;
  }

  hold_1st_p = l_favorite;

  while (l_favorite && l_favorite->next)
    l_favorite = l_favorite->next;

  l_favorite->next = Add_new_favorite(l_favorite->next, list_id, title_id, list_name, movie_name);
  return hold_1st_p;
}

LFavorite *Free_all_favorites(LFavorite *l_favorites)
{
  LFavorite *aux;

  while (l_favorites)
  {
    aux = l_favorites->next;
    if (l_favorites->name)
      free(l_favorites->name);
    if (l_favorites->movie_name)
      free(l_favorites->movie_name);
    free(l_favorites);

    l_favorites = aux;
  }
  return l_favorites;
}