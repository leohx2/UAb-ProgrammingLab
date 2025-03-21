#include <stdio.h>
// Include the lists to use and it functions as well during the program.
#include "header/listas.h"
// Include the auxiliar functions to use in main
#include "header/catalog.h"

#define FILENAME "movies_data.csv"

int main()
{
  FILE *f;
  LCatalog *catalog;

  catalog = NULL;

  // To work properly, the CSV file must have the follwing sintax:
  // Title, Category, Duration (min), Recommended classification age, View.
  f = fopen("movies_data.csv", "r");
  if (f == NULL)
  {
    printf("Error. File 'movies_data.csv' not found.\n");
    return 0;
  }

  catalog = Save_on_list(f, catalog);
  catalog = Initial_menu(catalog, f, FILENAME);

  // Saving.
  f = Save_Catalog(catalog, f, FILENAME);

  catalog = Free_all_lists(catalog);
  fclose(f);
}