#include "../header/listas.h"
#include "../header/helper.h"
#include "../header/catalog.h"
#include <stdlib.h>
#include <string.h>
#define MAXSTR 255

int Valid_Playlist_id(LFavorite *l_playlist, int id)
{
  if (l_playlist == NULL)
    return 0;

  while (l_playlist)
  {
    if (l_playlist->id == id)
      return 1;
    l_playlist = l_playlist->next;
  }

  return 0;
}

int Get_biggest_id(LFavorite *l_playlist)
{
  int biggest_id = 0;

  while (l_playlist)
  {
    if (l_playlist->id > biggest_id)
      biggest_id = l_playlist->id;
    l_playlist = l_playlist->next;
  }

  return biggest_id;
}

// mode: 1 allow the user to choose a playlist, mode: 2 just display the lists
void Show_all_playlist(LFavorite *l_playlist)
{
  LFavorite *aux;

  aux = l_playlist;
  while (aux)
  {
    if (aux->title_id == 0)
      printf("\nName: '%s'\nID: %d\n", aux->name, aux->id);
    aux = aux->next;
  }
}

// When the list is created it's first mention in the CSV file contains a title_id == 0.
void Show_playlist_titles(TLists *t_lists, TFiles *t_files, char *username)
{
  int counter = 0, list_id, user_choice;
  LFavorite *aux;

  printf("\n-----------------------------\n");
  printf("Please choose one of the playlists below\n");

  Show_all_playlist(t_lists->l_playlist);
  printf("Your choice (ID):");
  list_id = Safe_answer();

  if (!Valid_Playlist_id(t_lists->l_playlist, list_id))
  {
    printf("\nList not found\n");
    return;
  }

  aux = t_lists->l_playlist;
  while (aux && aux->id != list_id)
    aux = aux->next;

  printf("\nHere's all titles added to '%s'\n", aux->name);

  aux = t_lists->l_playlist;
  while (aux)
  {
    if (aux->id == list_id && aux->title_id != 0)
    {
      printf("\n'%s'\nID: %d\n", aux->movie_name, aux->title_id);
      counter++;
    }
    aux = aux->next;
  }

  if (counter == 0)
    printf("\nThis playlist is currently empty :)");
  else
  {
    user_choice = Remove_from_playlist_menu();
    if (user_choice > 0)
    {
      t_lists->l_playlist = Remove_from_playlist(t_lists->l_playlist, t_files->favorites, list_id, user_choice, username);
      t_lists->l_interactions = Update_Interactions(t_lists->l_interactions, 'R', user_choice, t_files->interactions);
    }
  }
  printf("\n-----------------------------\n");
}

LFavorite *Load_favorites(FILE *f_favorites)
{
  char str[MAXSTR], *token, *movie_name;
  int list_id, title_id;
  LFavorite *l_playlist = NULL;

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

    l_playlist = Add_favorite_in_order(l_playlist, list_id, title_id, token, movie_name);
  }

  return l_playlist;
}

// CSV splited in 4 parts, list_id, title_id, list_name, movie name
LFavorite *Create_new_favorite(LFavorite *l_playlist, FILE *f_favorites)
{
  int new_id;
  char name[MAXSTR];

  printf("\n-----------------------------\n");
  printf("New playlist name (please, do not use commas): ");
  scanf(" %[^\n]", name);

  if (strstr(name, ","))
  {
    printf("\nInvalid name, please, do not use commas.\n");
    printf("\n-----------------------------\n");
    return l_playlist;
  }

  new_id = Get_biggest_id(l_playlist) + 1;

  // When a new list is created, there's no movies added to it, so we place the title_id as 0 and the movie name as "Title name"
  l_playlist = Add_favorite_in_order(l_playlist, new_id, 0, name, "Title name");

  fseek(f_favorites, 0, SEEK_END);
  fprintf(f_favorites, "\n%d,%d,%s,Title name", new_id, 0, name);

  printf("\n-----------------------------\n");
  return l_playlist;
}

void Add_to_playlist(LFavorite *l_playlist, FILE *f_favorites, int list_id, int title_id, char *title)
{
  LFavorite *aux;

  aux = l_playlist;

  while (aux && aux->id != list_id)
    aux = aux->next;

  l_playlist = Add_favorite_in_order(l_playlist, list_id, title_id, aux->name, title);

  fseek(f_favorites, 0, SEEK_END);
  fprintf(f_favorites, "\n%d,%d,%s,%s", list_id, title_id, aux->name, title);
}

int Is_favorited(LFavorite *l_playlist, int list_id, int movie_id)
{
  while (l_playlist)
  {
    if (l_playlist->id == list_id && l_playlist->title_id == movie_id)
      return 1;
    l_playlist = l_playlist->next;
  }

  return 0;
}

void Add_to_custom_playlist(LFavorite *l_playlist, FILE *f_favorites, int title_id, char *title)
{
  int user_choice = 0;

  Show_all_playlist(l_playlist);

  printf("Your choice (Playlist ID): ");
  user_choice = Safe_answer();

  if (!Valid_Playlist_id(l_playlist, user_choice))
  {
    printf("\nList not found.\n");
    return;
  }

  if (!Is_favorited(l_playlist, user_choice, title_id))
    Add_to_playlist(l_playlist, f_favorites, user_choice, title_id, title);
}

// We don't need to worry about the first option, beacuse it's the declaration of the watch later playlist
LFavorite *Remove_from_playlist(LFavorite *l_playlist, FILE *f_playlist, int playlist_id, int title_id, char *username)
{
  LFavorite *aux;

  aux = l_playlist;

  while (aux && aux->next)
  {
    if (aux->next->id == playlist_id && aux->next->title_id == title_id)
    {
      if (aux->next->title_id != 0)
        printf("\nTitle '%s', has been removed successfully from playlist.\n", aux->next->movie_name);
      aux->next = Free_playlist(aux->next);
      SavePlaylistFile(l_playlist, f_playlist, username);
      return l_playlist;
    }
    aux = aux->next;
  }

  printf("\n Title not found.\n");
  return l_playlist;
}

void SavePlaylistFile(LFavorite *l_playlist, FILE *f_playlist, char *username)
{
  LFavorite *aux;

  fclose(f_playlist);
  f_playlist = Open_interaction_csv("./database csv/lists_", username, "w");
  if (f_playlist == NULL)
  {
    printf("ERROR, file can't be re-opened\n");
    return;
  }

  // Rewriting the first and second lines
  fprintf(f_playlist, "list ID, title ID, List name, Title name");

  aux = l_playlist;
  while (aux)
  {
    fprintf(f_playlist, "\n%d,%d,%s,%s", aux->id, aux->title_id, aux->name, aux->movie_name);
    aux = aux->next;
  }
}

void Delete_playlist(TLists *t_lists, TFiles *t_files, char *username)
{
  int list_id, id_to_delete;
  LFavorite *aux;

  printf("\n-----------------------------\n");
  printf("Select which playlist you'd like to delete below.\nWARNING: This cannot be reversed\n");

  aux = t_lists->l_playlist;
  while (aux)
  {
    // Watch later shouldn't be deleted by the user.
    if (aux->title_id == 0 && aux->id != WATCH_LATER)
      printf("\nName: '%s'\nID: %d\n", aux->name, aux->id);
    aux = aux->next;
  }

  printf("Your choice (ID):");
  list_id = Safe_answer();

  if (!Valid_Playlist_id(t_lists->l_playlist, list_id) || list_id == WATCH_LATER)
  {
    printf("\nList not found\n");
    return;
  }

  aux = t_lists->l_playlist;
  while (aux)
  {
    if (aux->id == list_id)
    {
      id_to_delete = aux->title_id;
      aux = aux->next;
      t_lists->l_playlist = Remove_from_playlist(t_lists->l_playlist, t_files->favorites, list_id, id_to_delete, username);
      if (id_to_delete != 0)
        Update_Interactions(t_lists->l_interactions, 'R', id_to_delete, t_files->interactions);
    }
    else
      aux = aux->next;
  }

  printf("\nThe playlist has been deleted successfully.\n");
  printf("\n-----------------------------\n");
}
