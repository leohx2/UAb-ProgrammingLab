#include "../header/catalog.h"
#include "../header/helper.h"
#include <stdlib.h>

// The prototype functions bellow will be used here, and only in this file, that's why
// it's not in the header file with the remaining functions.
void Edit_menu(LCatalog *l_catalog);
void Search_menu(LCatalog *l_catalog);

LCatalog *Initial_menu(LCatalog *l_catalog, LInteractions *l_interactions, char *file_name, TFiles *t_files)
{
  int user_choice = 0, id;

  while (user_choice != -1)
  {
    printf("\nPlease choose one of the options below\n 1 - Choose a title\n 2 - Show all titles.\n 3 - Search.\n 4 - Add a new title.");
    printf("\n 5 - Remove a title.\n 6 - Edit a title.\n 7 - Save.\n 8 - See my lists\n 9 - Create a new list\n10 - StreamFlix recommendation\n-1 - Exit.\n");
    printf("Option: ");
    user_choice = Safe_answer();

    switch (user_choice)
    {
    case -1:
    {
      printf("\nThank you for using StreamFlix :)\n");
      break;
    }
    case 1:
    {
      l_interactions = Watch_a_movie(l_interactions, l_catalog, t_files->interactions);
      break;
    }
    case 2:
    {
      printf("\nHere's a list of all the titles we have:\n");
      Print_catalog(l_catalog);
      break;
    }
    case 3:
    {
      Search_menu(l_catalog);
      break;
    }
    case 4:
    {
      l_catalog = User_add_new_title(l_catalog);
      break;
    }
    case 5:
    {
      printf("\nID to remove: ");
      id = Safe_answer();
      l_catalog = User_remove_title(l_catalog, id);
      break;
    }
    case 6:
    {
      Edit_menu(l_catalog);
      break;
    }
    case 7:
    {
      t_files->movies = Save_Catalog(l_catalog, t_files->movies, file_name);
      printf("\n Changes saved!\n");
      break;
    }
    case 8:
    {
      printf("TODO... See my lists\n");
      break;
    }
    case 9:
    {
      printf("TODO... Create a new list\n");
      break;
    }
    case 10:
    {
      printf("TODO... Recommendation\n");
      break;
    }
    default:
      printf("Invalid option\n");
    }
  }
  return l_catalog;
}

void Edit_menu(LCatalog *l_catalog)
{
  int id, edit_option;

  printf("\nTitle ID to edit: ");
  id = Safe_answer();
  printf("\nPlease, choose one of the options below to edit\n");
  printf("1 - Title.\n2 - Category.\n3 - Duration.\n4 - PEGI.\n5 - Views.\n6 - Everything.\n0 - Previous menu.\n");
  printf("Option: ");
  edit_option = Safe_answer();
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
  user_choice = Safe_answer();

  if (user_choice == 1 || user_choice == 2)
    Search_title_or_category(l_catalog, user_choice);
  else if (user_choice == 3)
    Search_PEGI(l_catalog);
  else
    printf("Invalid option.\n");
}