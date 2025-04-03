#include "../header/helper.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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

// Create a new user file. mode: 1 for interactions, 2 for lists
void Create_new_csv(char *path, char *username, int mode)
{
  char *holder;
  FILE *new_user;
  holder = (char *)malloc(sizeof(char) * strlen(path) + strlen(username) + 4);
  strcat(holder, path);
  strcat(holder, username);
  strcat(holder, ".csv");
  new_user = fopen(holder, "w+");
  if (mode == 1)
    fprintf(new_user, "Action('p'/'s'/'c' for pause/stream/conclusion),title_id");
  else if (mode == 2)
    fprintf(new_user, "list ID, title ID, List name, Title name\n1,0,Watch Later,Title name");
  fclose(new_user);
  free(holder);
}

FILE *Open_interaction_csv(char *path, char *username)
{
  char *holder;
  FILE *new_user;
  holder = (char *)malloc(sizeof(char) * strlen(path) + strlen(username) + 4);
  strcat(holder, path);
  strcat(holder, username);
  strcat(holder, ".csv");
  new_user = fopen(holder, "r+");
  free(holder);

  return (new_user);
}

int Safe_answer()
{
  char *str;

  scanf(" %[^\n]", str);
  return atoi(str);
}