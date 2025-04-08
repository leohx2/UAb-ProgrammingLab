#include "../header/catalog.h"
#include "../header/helper.h"
#include "../header/database.h"
#include <stdlib.h>

// The prototype functions bellow will be used here, and only in this file, that's why
// it's not in the header file with the remaining functions.
void Edit_menu(LCatalog *l_catalog);
void Search_menu(LCatalog *l_catalog);

LCatalog *Initial_menu(TLists *t_listas, char *file_name, TFiles *t_files, char *username)
{
  int user_choice = 0, id;

  while (user_choice != -1)
  {
    printf("\nPlease choose one of the options below\n 1 - Choose a show\n 2 - Display all shows.\n 3 - Search.\n 4 - Add a new show.");
    printf("\n 5 - Remove a show.\n 6 - Edit a show.\n 7 - Save.\n 8 - See my playlists\n 9 - Create a new playlist\n10 - Delete a playlist\n11 - StreamFlix recommendation\n-1 - Exit.\n");
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
      t_listas->l_interactions = Watch_a_movie(t_listas, t_files, username);
      break;
    }
    case 2:
    {
      printf("\nHere's a list of all the titles we have:\n");
      Print_catalog(t_listas->l_catalog);
      break;
    }
    case 3:
    {
      Search_menu(t_listas->l_catalog);
      break;
    }
    case 4:
    {
      t_listas->l_catalog = User_add_new_title(t_listas->l_catalog);
      break;
    }
    case 5:
    {
      printf("\nID to remove: ");
      id = Safe_answer();
      t_listas->l_catalog = User_remove_title(t_listas->l_catalog, id);
      break;
    }
    case 6:
    {
      Edit_menu(t_listas->l_catalog);
      break;
    }
    case 7:
    {
      t_files->movies = Save_Catalog(t_listas->l_catalog, t_files->movies, file_name);
      printf("\n Changes saved!\n");
      break;
    }
    case 8:
    {
      Show_playlist_titles(t_listas, t_files, username);
      break;
    }
    case 9:
    {
      t_listas->l_playlist = Create_new_favorite(t_listas->l_playlist, t_files->favorites);
      break;
    }
    case 10:
    {
      Delete_playlist(t_listas, t_files, username);
      break;
    }
    case 11:
    {
      // To make sure the interactions file is updated we'll close and reopen it.
      fclose(t_files->interactions);
      Recommendation(t_listas->l_catalog, username);
      t_files->interactions = Open_interaction_csv("./database csv/interaction_", username, "r+");
      break;
    }
    default:
      printf("Invalid option\n");
    }
  }
  return t_listas->l_catalog;
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

int Remove_from_playlist_menu()
{
  int user_choice = -2;

  while (user_choice < -1 || user_choice == 0)
  {
    printf("\nIf you want to remove a title from a playlist, type the title ID, otherwise, -1 to return to main menu\n");
    printf("Option: ");
    user_choice = Safe_answer();

    if (user_choice == 0)
      printf("\nInvalid option.\n");
  }

  return user_choice;
}