#include <stdio.h>
#include <stdlib.h>
// Include the lists to use and it functions as well during the program.
#include "header/listas.h"
// Include the auxiliar functions to use in main
#include "header/catalog.h"
#include "header/helper.h"
#include "header/database.h"

#define FILENAME "movies_data.csv"

int main()
{
  FILE *f_user;
  TFiles *t_files;
  TLists *t_lists;
  TUser *t_user;
  int user_choice = 0;

  t_lists = (TLists *)malloc(sizeof(TLists));
  t_files = (TFiles *)malloc(sizeof(TFiles));
  if (t_lists == NULL || t_files == NULL)
  {
    printf("ERROR, memory allocation.\n");
    return EXIT_FAILURE;
  }

  t_lists->l_catalog = NULL;
  f_user = NULL;

  // To work properly, the CSV file must have the follwing sintax:
  // Title, Category, Duration (min), Recommended classification age, View.
  t_files->movies = fopen("movies_data.csv", "r");
#ifdef _WIN32
  f_user = fopen(".\\database_csv\\user.csv", "r+");
#else
  f_user = fopen("./database_csv/user.csv", "r+");
#endif

  if (t_files->movies == NULL)
  {
    printf("Error. File 'movies_data.csv' not found.\n");
    return EXIT_FAILURE;
  }
  if (f_user == NULL)
  {
    printf("Error. user.csv not found.\n");
    return EXIT_FAILURE;
  }

  t_lists->l_catalog = Save_on_list(t_files->movies, t_lists->l_catalog);

  while (user_choice != 2)
  {
    printf("\n------------- Please choose an option -------------\n");
    printf("1: StreamFlix report\n2: Login\n");
    printf("option: ");
    user_choice = Safe_answer();
    if (user_choice == 1)
      Report(t_lists->l_catalog);
    else if (user_choice == 2)
      t_user = Login(f_user);
    else
      printf("\nInvalid option\n");
  }

  fclose(f_user);
  printf("\n____________Welcome to Streamflix, %s!____________\n", t_user->username);

// Load the user interactions in the list
#ifdef _WIN32
  t_files->interactions = Open_interaction_csv(".\\database_csv\\interaction_", t_user->username, "r+");
#else
  t_files->interactions = Open_interaction_csv("./database_csv/interaction_", t_user->username, "r+");
#endif
  if (t_files->interactions == NULL)
  {
    printf("Error, user database not found.\n");
    fclose(t_files->movies);
    return EXIT_FAILURE;
  }

#ifdef _WIN32
  t_files->favorites = Open_interaction_csv(".\\database_csv\\lists_", t_user->username, "r+");
#else
  t_files->favorites = Open_interaction_csv("./database_csv/lists_", t_user->username, "r+");
#endif
  if (t_files->favorites == NULL)
  {
    printf("Error, user database not found.\n");
    fclose(t_files->movies);
    fclose(t_files->interactions);
    return EXIT_FAILURE;
  }

  t_lists->l_playlist = Load_favorites(t_files->favorites);

  t_lists->l_interactions = Load_Interactions(t_files->interactions);

  t_lists->l_catalog = Initial_menu(t_lists, FILENAME, t_files, t_user->username);

  t_files->movies = Save_Catalog(t_lists->l_catalog, t_files->movies, FILENAME);

  t_lists->l_catalog = Free_all_lists(t_lists->l_catalog);
  t_user = Free_user(t_user);
  t_lists->l_interactions = Free_all_interactions(t_lists->l_interactions);
  t_lists->l_playlist = Free_all_playlists(t_lists->l_playlist);

  fclose(t_files->movies);
  fclose(t_files->interactions);
  fclose(t_files->favorites);

  free(t_files);
  free(t_lists);
}