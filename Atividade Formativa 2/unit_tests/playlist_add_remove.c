#include "../header/helper.h"
#include "../header/database.h"
#include "../header/catalog.h"
#include "../header/listas.h"
#include <stdlib.h>
#include <string.h>

#define MOVIE_1 8
#define MOVIE_2 11
#define MOVIE_3 13
#define MOVIE_4 18
#define MOVIE_5 20

FILE *Open_playlists(char *username);
int Movies_counter(LFavorite *playlist, int id);
int Playlist_size(LFavorite *playlist);

// Create or re-create an user with no interactions, ask for a recommendation, add an interaction and as for another recommendation.
// This file will test the recommendation syste and the interactions as well.
int main()
{
  FILE *f_lists;
  LFavorite *l_playlists;
  char *s = "user_tester";
  int before, after;

  f_lists = NULL;
  l_playlists = NULL;

// This user wont be registered at our user.csv but we need to use the main database for this test
#ifdef _WIN32
  Create_new_csv(".\\database_csv\\lists_", s, 2);
#else
  Create_new_csv("./database_csv/lists_", s, 2);
#endif

  // Test 1
  printf("\n\n*Test 1/6 -> Read list.csv:\n");
  f_lists = Open_playlists(s);
  if (f_lists == NULL)
    return EXIT_FAILURE;

  // Test 2
  printf("\n\n*Test 2/6 -> Load playlist info: \n");
  l_playlists = Load_favorites(f_lists);
  if (l_playlists == NULL)
  {
    printf("Error while loading playlist information\n");
    fclose(f_lists);
    return EXIT_FAILURE;
  }
  printf("Load playlist info -> Working!\n");

  // Test 3
  printf("\n\n*Test 3/6 -> Create a new playlist\n");
  before = Playlist_size(l_playlists);
  l_playlists = Create_new_favorite(l_playlists, f_lists);
  after = Playlist_size(l_playlists);
  if (before >= after)
  {
    printf("Error, new playlist was not created\n");
    fclose(f_lists);
    l_playlists = Free_all_playlists(l_playlists);
    return EXIT_FAILURE;
  }
  printf("Create a new playlist -> Working!\n");

  printf("\n*Displaying all playlists available:\n");
  Show_all_playlist(l_playlists);

  // Test 4
  printf("\n\n*Test 4/6 -> Adding movies to watch later\n");
  before = Movies_counter(l_playlists, 1);
  Add_to_playlist(l_playlists, f_lists, 1, MOVIE_1, "filme 1");
  Add_to_playlist(l_playlists, f_lists, 1, MOVIE_2, "filme 2");
  Add_to_playlist(l_playlists, f_lists, 1, MOVIE_3, "filme 3");
  after = Movies_counter(l_playlists, 1);
  if (before >= after)
  {
    printf("Error, didn't add movies to watch later\n");
    fclose(f_lists);
    l_playlists = Free_all_playlists(l_playlists);
    return EXIT_FAILURE;
  }
  printf("Adding movies to watch later-> Working!\n");

  // Test 5
  printf("\n\n*Test 5/6 -> Adding movies to new playlist\n");
  before = Movies_counter(l_playlists, 2);
  Add_to_playlist(l_playlists, f_lists, 2, MOVIE_4, "filme 4");
  Add_to_playlist(l_playlists, f_lists, 2, MOVIE_5, "filme 5");
  after = Movies_counter(l_playlists, 2);
  if (before >= after)
  {
    printf("Error, didn't add movies to the new playlist\n");
    fclose(f_lists);
    l_playlists = Free_all_playlists(l_playlists);
    return EXIT_FAILURE;
  }
  printf("Adding movies to new playlist -> Working!\n");

  // Test 6
  printf("\n\n*Test 6/6 -> Deleting movies from playlists\n");
  printf("Watch later:\n");
  before = Movies_counter(l_playlists, 1);
  l_playlists = Remove_from_playlist(l_playlists, f_lists, 1, MOVIE_1, s);
  l_playlists = Remove_from_playlist(l_playlists, f_lists, 1, MOVIE_2, s);
  l_playlists = Remove_from_playlist(l_playlists, f_lists, 1, MOVIE_3, s);
  after = Movies_counter(l_playlists, 1);
  if (before <= after)
  {
    printf("Error, while deleting from watch later\n");
    fclose(f_lists);
    l_playlists = Free_all_playlists(l_playlists);
    return EXIT_FAILURE;
  }
  printf("\nNew playlist:\n");
  before = Movies_counter(l_playlists, 2);
  l_playlists = Remove_from_playlist(l_playlists, f_lists, 2, MOVIE_4, s);
  l_playlists = Remove_from_playlist(l_playlists, f_lists, 2, MOVIE_5, s);
  after = Movies_counter(l_playlists, 2);
  if (before <= after)
  {
    printf("Error, while deleting from new playlist\n");
    fclose(f_lists);
    l_playlists = Free_all_playlists(l_playlists);
    return EXIT_FAILURE;
  }

  fclose(f_lists);
  l_playlists = Free_all_playlists(l_playlists);
  printf("\n\n*Everything is working here :)*\n\n");
  return EXIT_SUCCESS;
}

// 1
FILE *Open_playlists(char *username)
{
  FILE *lists;

#ifdef _WIN32
  lists = Open_interaction_csv(".\\database_csv\\lists_", username, "r+");
#else
  lists = Open_interaction_csv("./database_csv/lists_", username, "r+");
#endif
  if (lists == NULL)
  {
    printf("Open lists -> ERROR, can't open it\n");
    return NULL;
  }
  else
    printf("Open lists -> Working!\n");

  return lists;
}

int Playlist_size(LFavorite *playlist)
{
  int i = 0;

  while (playlist)
  {
    i++;
    playlist = playlist->next;
  }

  return i;
}

int Movies_counter(LFavorite *playlist, int id)
{
  int i = 0;
  while (playlist)
  {
    if (playlist->id == id)
      i++;
    playlist = playlist->next;
  }

  return i;
}
