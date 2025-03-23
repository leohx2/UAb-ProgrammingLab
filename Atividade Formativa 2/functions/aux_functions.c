#include "../header/helper.h"
#include <ctype.h>
#include <string.h>

void To_upper_str(char *s)
{
  int i;

  for (i = 0; s[i]; i++)
    s[i] = toupper(s[i]);
}

void Remove_plus_signals(char *s)
{
  while (s[strlen(s) - 1] == '+')
    s[strlen(s) - 1] = '\0';
}

void Print_title_details(LCatalog *l_catalog)
{
  printf("\nMovie: %s\n", l_catalog->title);
  printf("Category: %s, duration: %d, PEGI: %s, views: %d\n", l_catalog->category, l_catalog->duration, l_catalog->pegi, l_catalog->views);
  printf("ID: %d\n", l_catalog->id);
}

int Get_bigger_id(LCatalog *l_catalog)
{
  int bigger = 0;

  while (l_catalog)
  {
    if (l_catalog->id > bigger)
      bigger = l_catalog->id;
    l_catalog = l_catalog->next;
  }
  return bigger;
}