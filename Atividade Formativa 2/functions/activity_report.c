#include "../header/database.h"
#include "../header/catalog.h"
#include "../header/listas.h"
#include "../header/helper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSTR 70
#define MOVIES_REPORT 5 // How many movies to show during the report

TViews *Streamed_shows(char *username, TViews *t_views);
TViews *Update_views(TViews *t_views, int title_id);
TViews *Sort_views(TViews *t_views);
void Print_report(TViews *t_views, LCatalog *l_catalog);

/* This report shows:
 * 1. The top 5 most-streamed shows by users, not couting it's currently number of views
 * 2. The 3 most popular categories
 * 3. Most active users
  To do so, we need to loop through all users.
*/
void Report(LCatalog *l_catalog)
{
  FILE *f_users;
  TViews *t_views;
  char s[MAXSTR], *username;

  t_views = NULL;
  f_users = fopen("./database csv/user.csv", "r");

  if (f_users == NULL)
  {
    printf("Users not found\n");
    return;
  }

  // Ignore the first csv line
  fgets(s, MAXSTR, f_users);

  while (!feof(f_users))
  {
    fgets(s, MAXSTR, f_users);

    // the fist value is the ID and the second is the username.
    // We don't need the ID here, just ignore it.
    username = strtok(s, ",");

    // get username
    username = strtok(NULL, ",");
    if (username[strlen(username) - 1] == '\n')
      username[strlen(username) - 1] = '\0';
    t_views = Streamed_shows(username, t_views);
  }

  t_views = Sort_views(t_views);

  Print_report(t_views, l_catalog);

  t_views = Free_all_views(t_views);
  fclose(f_users);
}

void Print_report(TViews *t_views, LCatalog *l_catalog)
{
  TViews *aux;
  int i = 1;

  aux = t_views;
  printf("\n--------------------------------------\n");
  printf("Welcome to streamFlix report!\n");

  printf("\nHere's our top 5 most streamed movies:\n");
  while (aux && i <= MOVIES_REPORT)
  {
    printf("\n%2d -> ", i);
    Print_Movie_name(l_catalog, aux->show_id);
    printf("\nViews: %3d\n", aux->views);
    aux = aux->next;
    i++;
  }
  printf("\n--------------------------------------\n");
}

TViews *Streamed_shows(char *username, TViews *t_views)
{
  FILE *f_interactions;
  char str[10], *token;

  f_interactions = Open_interaction_csv("./database csv/interaction_", username, "r");
  if (f_interactions == NULL)
  {
    printf("Error. File not found (report system).\n");
    return NULL;
  }

  // this file has 2 values, the action and the title ID, we're looking for 'w' actions
  // that sinalizes the show was streamed until the end.
  // Ignore the csv 1st line.
  fgets(str, 10, f_interactions);

  while (!feof(f_interactions))
  {
    fgets(str, 10, f_interactions);

    // Get the action
    token = strtok(str, ",");
    printf("");
    if (token[0] == 'w')
    {
      // Get the title ID
      token = strtok(NULL, ",");
      t_views = Update_views(t_views, atoi(token));
    }
  }

  fclose(f_interactions);
  return t_views;
}

TViews *Update_views(TViews *t_views, int title_id)
{
  TViews *aux;

  if (t_views == NULL)
  {
    t_views = Add_new_view(t_views, title_id, 1);
    return t_views;
  }

  // Check the 1st postion
  if (t_views->show_id == title_id)
  {
    t_views->views++;
    return t_views;
  }

  // Check the remaining positions, always checking the next to be able to keep track of the
  // current position, otherwise it could turn into a NULL at the end and we would lost the track of it
  aux = t_views;
  while (aux && aux->next && aux->next->show_id != title_id)
    aux = aux->next;

  if (aux->next)
    aux->next->views++;
  else
    aux->next = Add_new_view(aux->next, title_id, 1);

  return t_views;
}

// We're not using bigdata or anything close to it, due that I will use bubble sorte
TViews *Sort_views(TViews *t_views)
{
  int counter = 0;
  TViews *aux, holder;

  aux = t_views;
  while (aux && aux->next)
  {
    if (aux->views < aux->next->views || (aux->views == aux->next->views && aux->show_id < aux->next->show_id))
    {
      holder.views = aux->views;
      holder.show_id = aux->show_id;
      aux->views = aux->next->views;
      aux->show_id = aux->next->show_id;
      aux->next->views = holder.views;
      aux->next->show_id = holder.show_id;
      counter++;
    }
    aux = aux->next;
  }

  if (counter > 0)
    t_views = Sort_views(t_views);

  return t_views;
}