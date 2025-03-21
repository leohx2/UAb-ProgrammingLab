#include <stdio.h>
#include "../header/catalog.h"

void Edit_menu(LCatalog *l_catalog);

LCatalog *Initial_menu(LCatalog *l_catalog, FILE *f, char *file_name)
{
  int user_choice = -1, id;

  printf("\n____________Welcome to Streamflix!____________");

  while (user_choice != 0)
  {
    printf("\nPlease choose one of the options below\n1 - Show all titles.\n2 - Add a new title.\n3 - Remove a title.\n4 - Edit a title.\n5 - Save.\n0 - Exit.\n");
    printf("Option: ");
    scanf("%d", &user_choice);

    switch (user_choice)
    {
    case 0:
    {
      printf("Thank you for using StreamFlix :)\n");
      break;
    }
    case 1:
    {
      printf("\nHere's a list of all the titles we have:\n");
      Print_catalog(l_catalog);
      break;
    }
    case 2:
    {
      l_catalog = User_add_new_title(l_catalog);
      break;
    }
    case 3:
    {
      printf("\nID to remove: ");
      scanf("%d", &id);
      l_catalog = User_remove_title(l_catalog, id);
      break;
    }
    case 4:
    {
      Edit_menu(l_catalog);
      break;
    }
    case 5:
    {
      f = Save_Catalog(l_catalog, f, file_name);
      printf("\n Changes saved!\n");
      break;
    }
    default:
    {
      printf("Invalid option\n");
      break;
    }
    }
  }
  return l_catalog;
}

void Edit_menu(LCatalog *l_catalog)
{
  int id, edit_option;

  printf("\nTitle ID to edit: ");
  scanf("%d", &id);
  printf("\nPlease, choose one of the options below to edit\n");
  printf("1 - Title.\n2 - Category.\n3 - Duration.\n4 - PEGI.\n5 - Views.\n6 - Everything.\n0 - Previous menu.\nOption: ");
  scanf("%d", &edit_option);
  if (edit_option == 0)
    return;
  if (edit_option > 6 || edit_option < 1)
    printf("Invalid option\n");
  if (Edit_item(l_catalog, edit_option, id) == 0)
    printf("ID not found\n");
}