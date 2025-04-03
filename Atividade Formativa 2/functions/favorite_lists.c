#include "../header/listas.h"
#include "../header/helper.h"
#include "../header/catalog.h"
#include <stdlib.h>
#include <string.h>
#define MAXSTR 255

int Valid_Favorit_id(LFavorite *l_favorites, int id)
{
  if (l_favorites == NULL)
    return 0;

  while (l_favorites)
  {
    if (l_favorites->id == id)
      return 1;
    l_favorites = l_favorites->next;
  }

  return 0;
}

int Get_biggest_id(LFavorite *l_favorites)
{
  int biggest_id = 0;

  while (l_favorites)
  {
    if (l_favorites->id > biggest_id)
      biggest_id = l_favorites->id;
    l_favorites = l_favorites->next;
  }

  return biggest_id;
}

void Show_all_favorites(LFavorite *l_favorites)
{
  int user_choice = 0;
  LFavorite *aux;

  printf("\nPlease choose one of the lists below\n");

  aux = l_favorites;
  while (aux)
  {
    printf("\n%s\nList ID: %d\n", aux->name, aux->id);
    aux = aux->next;
  }
  printf("\nYour choice (ID): ");
  user_choice = Safe_answer();

  if (user_choice == -1)
    return;
  else
    Show_favorite_titles(l_favorites, user_choice);
}

// When the list is created it's first mention in the CSV file contains a title_id == 0.
void Show_favorite_titles(LFavorite *l_favorites, int list_id)
{
  int counter = 0;
  LFavorite *aux;

  printf("\n-----------------------------\n");

  if (!Valid_Favorit_id(l_favorites, list_id))
  {
    printf("\nList not found\n");
    return;
  }

  aux = l_favorites;
  while (aux && aux->id != list_id)
    aux = aux->next;

  printf("List: %s\n", aux->name);

  while (l_favorites)
  {
    if (l_favorites->id == list_id && l_favorites->title_id != 0)
    {
      printf("\nTile: %s\nID: %d\n", l_favorites->movie_name, l_favorites->title_id);
      counter++;
    }
    l_favorites = l_favorites->next;
  }
  if (counter == 0)
    printf("\nThis list is currently empty :)");
  printf("\n-----------------------------\n");
}

LFavorite *Load_favorites(FILE *f_favorites)
{
  char str[MAXSTR], *token, *movie_name;
  int list_id, title_id;
  LFavorite *l_favorites = NULL;

  // Ignore CSV first line
  fgets(str, MAXSTR, f_favorites);

  // CSV splited in 4 parts, list_id, title_id, list_name, movie name
  // Title_id can be 0 if there is no movie added to the list.
  while (!feof(f_favorites))
  {
    fgets(str, MAXSTR, f_favorites);

    // Get the list_id
    token = strtok(str, ",");
    list_id = atoi(token);

    // Get the title_id
    token = strtok(NULL, ",");
    title_id = atoi(token);

    // the list name
    token = strtok(NULL, ",");

    // movie name
    movie_name = strtok(NULL, ",");

    l_favorites = Add_favorite_in_order(l_favorites, list_id, title_id, token, movie_name);
  }

  return l_favorites;
}

// CSV splited in 4 parts, list_id, title_id, list_name, movie name
LFavorite *Create_new_favorite(LFavorite *l_favorite, FILE *f_favorites)
{
  int new_id;
  char name[MAXSTR];

  printf("\n-----------------------------\n");
  printf("New list name (please, do not use commas): ");
  scanf(" %[^\n]", name);

  if (strstr(name, ","))
  {
    printf("\nInvalid name, please, do not use commas.\n");
    printf("\n-----------------------------\n");
    return l_favorite;
  }

  new_id = Get_biggest_id(l_favorite) + 1;

  // When a new list is created, there's no movies added to it, so we place the title_id as 0 and the movie name as "Title name"
  l_favorite = Add_favorite_in_order(l_favorite, new_id, 0, name, "Title name");

  fseek(f_favorites, 0, SEEK_END);
  fprintf(f_favorites, "\n%d,%d,%s,Title name", new_id, 0, name);

  printf("\n-----------------------------\n");
  return l_favorite;
}

LFavorite *Add_movie_favorite(LFavorite *l_favorite, FILE *f_favorites, int list_id, int title_id, char *title)
{
  LFavorite *aux;

  aux = l_favorite;

  while (aux && aux->id != list_id)
    aux = aux->next;

  l_favorite = Add_favorite_in_order(l_favorite, list_id, title_id, aux->name, title);

  fseek(f_favorites, 0, SEEK_END);
  fprintf(f_favorites, "%d,%d,%s,%s", list_id, title_id, aux->name, title);

  return l_favorite;
}