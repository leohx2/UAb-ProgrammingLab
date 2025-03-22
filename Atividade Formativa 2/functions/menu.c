#include "../header/catalog.h"

// The prototype functions bellow will be used here, and only in this file, that's why
// it's not in the header file with the remaining functions.
void Edit_menu(LCatalog *l_catalog);
void Search_menu(LCatalog *l_catalog);

LCatalog *Initial_menu(LCatalog *l_catalog, FILE *f, char *file_name)
{
  int user_choice = -1, id;

  printf("\n____________Welcome to Streamflix!____________");

  while (user_choice != 0)
  {
    printf("\nPlease choose one of the options below\n1 - Show all titles.\n2 - Search.\n3 - Add a new title.\n4 - Remove a title.\n5 - Edit a title.\n6 - Save.\n0 - Exit.\n");
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
      Search_menu(l_catalog);
      break;
    }
    case 3:
    {
      l_catalog = User_add_new_title(l_catalog);
      break;
    }
    case 4:
    {
      printf("\nID to remove: ");
      scanf("%d", &id);
      l_catalog = User_remove_title(l_catalog, id);
      break;
    }
    case 5:
    {
      Edit_menu(l_catalog);
      break;
    }
    case 6:
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
  printf("1 - Title.\n2 - Category.\n3 - Duration.\n4 - PEGI.\n5 - Views.\n6 - Everything.\n0 - Previous menu.\n");
  printf("Option: ");
  scanf("%d", &edit_option);
  if (edit_option == 0)
    return;
  if (edit_option > 6 || edit_option < 1)
    printf("Invalid option\n");
  if (Edit_item(l_catalog, edit_option, id) == 0)
    printf("ID not found\n");
}

void Search_menu(LCatalog *l_catalog)
{
  int user_choice;

  printf("\nChoose one of the options bellow:\n");
  printf("Search by\n1 - Title\n2 - Category\n3 - Age Rating\n");
  printf("option:");
  scanf("%d", &user_choice);

  if (user_choice == 1 || user_choice == 2)
    Search_title_or_category(l_catalog, user_choice);
  else if (user_choice == 3)
    Search_PEGI(l_catalog);
  else
    printf("Invalid option.\n");
}