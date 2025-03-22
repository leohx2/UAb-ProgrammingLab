#include <string.h>
#include <stdlib.h>
#include "../header/catalog.h"
#include "../header/helper.h"
#define MAXSTR 255

// mode 1: exact match, mode 2: similar
// This function searches for titles or categories based on the specified mode.
int Print_matchs(LCatalog *aux, char *user_input, char holder[MAXSTR], int mode, int title_or_category)
{
  int counter = 0;
  while (aux)
  {
    if (title_or_category == 1)
      strcpy(holder, aux->title);
    else
      strcpy(holder, aux->category);
    To_upper_str(holder);

    if ((mode == 1 && strcmp(holder, user_input) == 0) ||
        (mode == 2 && strstr(holder, user_input) != NULL && strcmp(holder, user_input) != 0))
    {
      Print_title_details(aux);
      counter++;
    }
    aux = aux->next;
  }

  return counter;
}

// Divide in two parts, exactly match and containing the string
// mode 1 for title, 2 for category
void Search_title_or_category(LCatalog *l_catalog, int mode)
{
  char user_input[MAXSTR], holder[MAXSTR];
  LCatalog *aux;

  if (mode == 1)
    printf("\nTitle: ");
  else
    printf("\nCategory: ");
  scanf(" %[^\n]", user_input);

  To_upper_str(user_input);

  // Exactly match
  printf("\n------------------------------------------------------------");
  printf("\nThe search results show the following exactly match:\n");
  aux = l_catalog;

  if (Print_matchs(aux, user_input, holder, 1, mode) == 0)
    printf("The exactly match was not found.\n");
  printf("\n------------------------------------------------------------");

  printf("\n------------------------------------------------------------");
  // Contaning the user input
  printf("\nHere are some similar titles:\n");
  aux = l_catalog; // Reset position.

  if (Print_matchs(aux, user_input, holder, 2, mode) == 0)
    printf("Nothing found.\n");
  printf("\n------------------------------------------------------------");
}

int Print_pegi_matchs(LCatalog *aux, int user_input, int mode)
{
  char holder_str[10];
  int counter = 0, holder_int;

  while (aux)
  {
    strcpy(holder_str, aux->pegi);
    Remove_plus_signals(holder_str);
    holder_int = atoi(holder_str);

    if ((mode == 1 && holder_int == user_input) ||
        (mode == 2 && holder_int < user_input))
    {
      Print_title_details(aux);
      counter++;
    }
    aux = aux->next;
  }

  return counter;
}

// This function supports user inputs with numbers only or numbers followed by a plus sign.
// Example: 13, 13+, or even 13++++
void Search_PEGI(LCatalog *l_catalog)
{
  char user_input[10], holder[10], answer[10];
  int input_pegi, title_pegi;
  LCatalog *aux;

  printf("\nAge rating (numbers only, i.e '13+', '13', '0+', '0'): ");
  scanf(" %[^\n]", user_input);

  // clean the input
  Remove_plus_signals(user_input);
  input_pegi = atoi(user_input);

  // Exactly match
  printf("\n------------------------------------------------------------");
  printf("\nThe search results show the following exactly age rating match:\n");

  aux = l_catalog;
  if (Print_pegi_matchs(aux, input_pegi, 1) == 0)
    printf("The exactly match was not found.\n");
  printf("\n------------------------------------------------------------");

  printf("\nWould you like to have a list of movies with a lower age rating as well?\nAnswer(y/n): ");
  scanf(" %9[^\n]", answer);

  To_upper_str(answer);
  if (strcmp("YES", answer) == 0 || strcmp("Y", answer) == 0)
  {
    printf("\n------------------------------------------------------------");
    // Contaning the user input
    printf("\nHere are some similar titles, with a lower age rating:\n");

    aux = l_catalog; // Reset position.
    if (Print_pegi_matchs(aux, input_pegi, 2) == 0)
      printf("Nothing found.\n");
    printf("\n------------------------------------------------------------");
  }
}