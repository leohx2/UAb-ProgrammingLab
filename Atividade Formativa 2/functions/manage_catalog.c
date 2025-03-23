#include <string.h>
#include <stdlib.h>
#include "../header/catalog.h"
#include "../header/helper.h"
#define MAXSTR 255

// Read all the CSV content and save it in their lists
LCatalog *Save_on_list(FILE *f, LCatalog *l_catalog)
{
  int controler = 0;
  char str[MAXSTR], *token;
  LCatalog *aux;

  fgets(str, MAXSTR, f);

  while (!feof(f))
  {
    fgets(str, MAXSTR, f);
    token = strtok(str, ",");

    /*
    Every line has 5 pieces of information: title, category, duration, PEGI, and views.
    Using a loop and a switch case, we'll iterate through each line of the CSV data and
    assign each value to the correct field. After processing each line, we save it into the list.
    The controller variable determines where to assign the current piece of information.
    */
    while (token != NULL)
    {
      if (token[strlen(token) - 1] == '\n')
        token[strlen(token) - 1] = '\0';

      switch (controler++)
      {
      case 0:
      {
        aux->title = token;
        break;
      }
      case 1:
      {
        aux->category = token;
        break;
      }
      case 2:
      {
        aux->duration = atoi(token);
        break;
      }
      case 3:
      {
        aux->pegi = token;
        break;
      }
      case 4:
      {
        aux->views = atoi(token);
        break;
      }
      default:
      {
        aux->id = atoi(token);
        controler = 0;
        break;
      }
      }
      token = strtok(NULL, ",");
    }
    l_catalog = Add_new(l_catalog, aux);
  }
  return l_catalog;
}

int Edit_item(LCatalog *l_catalog, int content_to_edit, int id)
{
  char aux[MAXSTR];
  int int_aux;

  while (l_catalog && l_catalog->id != id)
    l_catalog = l_catalog->next;

  if (l_catalog)
  {
    if (content_to_edit == 1 || content_to_edit == 6)
    {
      printf("\nCurrent title: %s\nNew title (commas are not allowed): ", l_catalog->title);
      scanf(" %[^\n]", aux);
      if (l_catalog->title)
        free(l_catalog->title);
      l_catalog->title = strdup(aux);
    }
    if (content_to_edit == 2 || content_to_edit == 6)
    {
      printf("\nCurrent category: %s\nNew category (if there is more than one category, use a backslash (\\) as a separator): ", l_catalog->category);
      scanf(" %[^\n]", aux);
      if (l_catalog->category)
        free(l_catalog->category);
      l_catalog->category = strdup(aux);
    }
    if (content_to_edit == 3 || content_to_edit == 6)
    {
      printf("\nCurrent duration: %d\nNew duration (only positive numbers are allowed): ", l_catalog->duration);
      scanf("%d", &int_aux);
      if (int_aux <= 0)
        printf("Format not supported, only numbers (greater than 0) allowed\n");
      else
        l_catalog->duration = int_aux;
    }
    if (content_to_edit == 4 || content_to_edit == 6)
    {
      printf("\nCurrent PEGI: %s\nNew PEGI: ", l_catalog->pegi);
      scanf(" %[^\n]", aux);
      if (l_catalog->pegi)
        free(l_catalog->pegi);
      l_catalog->pegi = strdup(aux);
    }
    if (content_to_edit == 5 || content_to_edit == 6)
    {
      printf("\nCurrent view count: %d\nNew view count: ", l_catalog->views);
      scanf("%d", &int_aux);
      l_catalog->views = atoi(aux);
    }
  }
  else
    return 0;
  return 1;
}

LCatalog *User_add_new_title(LCatalog *l_catalog)
{
  LCatalog *aux;
  char s[3][MAXSTR];
  int i;

  aux = (LCatalog *)malloc(sizeof(LCatalog));
  if (aux == NULL)
  {
    printf("Add new title, Error.\n");
    return l_catalog;
  }

  printf("\nTitle: ");
  scanf(" %[^\n]", s[0]);
  aux->title = strdup(s[0]);

  printf("Category (don't use commas, use '\\' instead if there's more than one): ");
  scanf(" %[^\n]", s[1]);
  aux->category = strdup(s[1]);

  printf("Duration (numbers, greater than 0, only): ");
  scanf("%d", &aux->duration);

  printf("PEGI: ");
  scanf(" %[^\n]", s[2]);
  aux->pegi = strdup(s[2]);

  printf("Current number of views: ");
  scanf("%d", &aux->views);

  aux->id = Get_bigger_id(l_catalog) + 1;
  l_catalog = Add_new(l_catalog, aux);

  aux = Free_current_list(aux);
  return l_catalog;
}

LCatalog *User_remove_title(LCatalog *l_catalog, int id)
{
  LCatalog *aux;

  if (l_catalog == NULL)
    return NULL;

  // Verify the 1st list item
  if (l_catalog->id == id)
  {
    printf("\nTitle '%s', removed successfully.\n", l_catalog->title);
    return Free_current_list(l_catalog);
  }

  // Save the current position.
  aux = l_catalog;

  // Verify all the remaining titles, until we find the id in the next title.
  while (l_catalog && l_catalog->next)
  {
    if (l_catalog->next->id == id)
    {
      printf("\nTitle '%s', removed successfully.\n", l_catalog->next->title);
      l_catalog->next = Free_current_list(l_catalog->next);
      return aux;
    }
    l_catalog = l_catalog->next;
  }

  printf("ID not found\n");
  return aux;
}

FILE *Save_Catalog(LCatalog *l_catalog, FILE *f, char *fileName)
{
  // Close the file and reopen it with write permissions to clear its contents and rewrite the data.
  fclose(f);
  f = fopen(fileName, "w");
  if (f == NULL)
  {
    printf("Error: The Save_Catalog function requires a file as a parameter.\n");
    return f;
  }

  // Writing the 1st line of csv file.
  fprintf(f, "Title,Category,Duration (min),Age Rating,Views");

  // Below we have all the remaining lines, following the pattern from the first line.
  while (l_catalog)
  {
    fprintf(f, "\n%s,%s,%d,%s,%d,%d", l_catalog->title, l_catalog->category, l_catalog->duration, l_catalog->pegi, l_catalog->views, l_catalog->id);
    l_catalog = l_catalog->next;
  }
  fclose(f);
  return fopen(fileName, "r");
}
