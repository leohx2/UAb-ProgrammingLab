#include <stdio.h>
// Include the lists to use and it functions as well during the program.
#include "header/listas.h"
// Include the auxiliar functions to use in main
#include "header/catalog.h"
#include "header/helper.h"
#include "header/database.h"

#define FILENAME "movies_data.csv"

int main()
{
  FILE *f, *f_user, *f_interactions;
  LCatalog *catalog;
  LInteractions *l_interactions;
  TUser *t_user;

  catalog = NULL;
  f_user = NULL;

  // To work properly, the CSV file must have the follwing sintax:
  // Title, Category, Duration (min), Recommended classification age, View.
  f = fopen("movies_data.csv", "r");
  f_user = fopen("./database csv/user.csv", "r+");

  if (f == NULL)
  {
    printf("Error. File 'movies_data.csv' not found.\n");
    return 0;
  }
  if (f_user == NULL)
  {
    printf("Error. user.csv not found.\n");
    return 0;
  }

  catalog = Save_on_list(f, catalog);

  t_user = Login(f_user);
  fclose(f_user);
  printf("\n____________Welcome to Streamflix, %s!____________\n", t_user->username);

  // Load the user interactions in the list
  f_interactions = Open_interaction_csv("./database csv/interaction_", t_user->username);
  if (f_interactions == NULL)
  {
    printf("Error, user database not found.\n");
    fclose(f);
  }

  l_interactions = Load_Interactions(f_interactions, t_user);

  catalog = Initial_menu(catalog, f, FILENAME);

  f = Save_Catalog(catalog, f, FILENAME);

  catalog = Free_all_lists(catalog);
  t_user = Free_user(t_user);

  fclose(f);
  fclose(f_interactions);
}