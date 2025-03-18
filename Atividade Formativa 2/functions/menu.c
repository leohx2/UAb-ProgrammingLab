#include <stdio.h>
#include "../header/listas.h"

void Initial_menu(LCatalog *l_catalog)
{
  int user_choice = -1;

  printf("\n____________Welcome to Streamflix!____________");

  while (user_choice != 0)
  {
    printf("\nPlease choose one of the options bellow\n1 - Show all title.\n2 - Add a new title.\n3 - Remove a title.\n4 - Edit a title.\n0 - Exit\n");
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
      printf("\nHere's a list of all title we have:\n");
      Print_catalog(l_catalog);
      break;
    }
    case 2:
    {
      // TODO add a new movie;
      break;
    }
    case 3:
    {
      // TODO remove a movie;
      break;
    }
    case 4:
    {
      // TODO edit a movie;
      break;
    }
    default:
    {
      printf("Invalid option\n");
      break;
    }
    }
  }
}