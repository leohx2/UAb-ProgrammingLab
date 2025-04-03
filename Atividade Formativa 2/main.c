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
  LCatalog *l_catalog;
  LInteractions *l_interactions;
  LFavorite *l_favorites;
  TUser *t_user;

  l_catalog = NULL;
  f_user = NULL;
  t_files = (TFiles *)malloc(sizeof(TFiles));

  // To work properly, the CSV file must have the follwing sintax:
  // Title, Category, Duration (min), Recommended classification age, View.
  t_files->movies = fopen("movies_data.csv", "r");
  f_user = fopen("./database csv/user.csv", "r+");

  if (t_files->movies == NULL)
  {
    printf("Error. File 'movies_data.csv' not found.\n");
    return 0;
  }
  if (f_user == NULL)
  {
    printf("Error. user.csv not found.\n");
    return 0;
  }

  l_catalog = Save_on_list(t_files->movies, l_catalog);

  t_user = Login(f_user);
  fclose(f_user);
  printf("\n____________Welcome to Streamflix, %s!____________\n", t_user->username);

  // Load the user interactions in the list
  t_files->interactions = Open_interaction_csv("./database csv/interaction_", t_user->username);
  if (t_files->interactions == NULL)
  {
    printf("Error, user database not found.\n");
    fclose(t_files->movies);
  }

  t_files->favorites = Open_interaction_csv("./database csv/lists_", t_user->username);
  if (t_files->favorites == NULL)
  {
    printf("Error, user database not found.\n");
    fclose(t_files->movies);
    fclose(t_files->interactions);
  }

  l_favorites = Load_favorites(t_files->favorites);

  l_interactions = Load_Interactions(t_files->interactions);

  l_catalog = Initial_menu(l_catalog, l_interactions, l_favorites, FILENAME, t_files);

  t_files->movies = Save_Catalog(l_catalog, t_files->movies, FILENAME);

  l_catalog = Free_all_lists(l_catalog);
  t_user = Free_user(t_user);
  l_interactions = Free_all_interactions(l_interactions);
  l_favorites = Free_all_favorites(l_favorites);

  fclose(t_files->movies);
  fclose(t_files->interactions);
  fclose(t_files->favorites);

  free(t_files);
}