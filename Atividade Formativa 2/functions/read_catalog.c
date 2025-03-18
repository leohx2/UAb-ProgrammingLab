#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../header/listas.h"
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
      default:
      {
        aux->views = atoi(token);
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