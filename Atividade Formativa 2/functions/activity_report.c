#include "../header/database.h"
#include "../header/catalog.h"
#include "../header/listas.h"
#include "../header/helper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSTR 120
#define MOVIES_REPORT 5 // How many movies to show during the report
#define CATEGORIES_REPORT 3

LViews *Update_views(LViews *l_views, int title_id);
LViews *Sort_views(LViews *l_views);
void Print_report(LViews *l_views, LCatalog *l_catalog, LCategory *l_category, char *username, int h_interaction);
LCategory *Most_popular_categories(LCategory *l_category, LViews *l_views, LCatalog *l_catalog);
LCategory *Update_Categories_popularity(LCategory *l_category, char *c_name, int views);
LCategory *Sort_categories(LCategory *l_category);

/* This report shows:
 * 1. The top 5 most-streamed shows by users, not couting it's currently number of views
 * 2. The 3 most popular categories
 * 3. Most active users
  To do so, we need to loop through all users.
*/
void Report(LCatalog *l_catalog)
{
  FILE *f_users;
  LViews *l_views;
  LCategory *l_category;
  char s[MAXSTR], *username, *h_engagement_user;
  int interaction_counter, h_interaction = 0;

  l_views = NULL;
  l_category = NULL;
  h_engagement_user = NULL;
#ifdef _WIN32
  f_users = fopen(".\\database_csv\\user.csv", "r");
#else
  f_users = fopen("./database_csv/user.csv", "r");
#endif

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

    // This functions also keep track of how many interactions the user had with the system
    interaction_counter = 0;
    l_views = Streamed_shows(username, l_views, l_catalog, &interaction_counter, 1);

    // Keep tracking the highest-engagement user
    if (interaction_counter > h_interaction)
    {
      h_interaction = interaction_counter;
      if (h_engagement_user)
        free(h_engagement_user);

      h_engagement_user = strdup(username);
    }
  }

  l_views = Sort_views(l_views);
  l_category = Most_popular_categories(l_category, l_views, l_catalog);
  Print_report(l_views, l_catalog, l_category, h_engagement_user, h_interaction);

  l_views = Free_all_views(l_views);
  l_category = Free_all_categories(l_category);
  free(h_engagement_user);
  fclose(f_users);
}

void Print_report(LViews *l_views, LCatalog *l_catalog, LCategory *l_category, char *username, int h_interaction)
{
  // These will allow us to not lose the first postion from the parameters.
  LViews *views;
  LCategory *category;
  int i = 1;

  views = l_views;
  printf("\n---------------------------------------------------\n");
  printf("\t\tStreamFlix report!\n");

  printf("\n- Check out our top 5 most-streamed shows:\n");
  while (views && i <= MOVIES_REPORT)
  {
    printf("\n%2d: ", i);
    Print_Movie_name(l_catalog, views->show_id);
    printf("\nTotal views: %2d\n", views->views);
    views = views->next;
    i++;
  }

  printf("\n\n- Our top 3 most popular categories:\n");
  i = 1;
  category = l_category;
  while (category && i <= CATEGORIES_REPORT)
  {
    printf("\n%2d: %s\nTotal views: %2d\n", i, category->category, category->counter);
    category = category->next;
    i++;
  }

  printf("\n\n- Highest-engagement user:\n");
  printf("\nCongratulations to %s, our top user with %d interactions!\n", username, h_interaction);
  printf("\n---------------------------------------------------\n");
}

// mode 1 to organize and update the shows and it's views, mode 2 to just list all the watched shows interactions from newest to oldest
LViews *Streamed_shows(char *username, LViews *l_views, LCatalog *l_catalog, int *interactions_counter, int mode)
{
  FILE *f_interactions;
  char str[MAXSTR], *token;

#ifdef _WIN32
  f_interactions = Open_interaction_csv(".\\database_csv\\interaction_", username, "r");
#else
  f_interactions = Open_interaction_csv("./database_csv/interaction_", username, "r");
#endif
  if (f_interactions == NULL)
  {
    printf("Error. File not found (report system).\n");
    return NULL;
  }

  // this file has 2 values, the action and the title ID, we're looking for 'w' actions
  // that sinalizes the show was streamed until the end.
  // Ignore the csv 1st line.
  fgets(str, MAXSTR, f_interactions);

  while (!feof(f_interactions))
  {
    fgets(str, MAXSTR, f_interactions);

    // Get the action
    token = strtok(str, ",");

    if (interactions_counter)
      *interactions_counter += 1;
    if (token[0] == 'w')
    {
      // Get the title ID
      token = strtok(NULL, ",");

      if (Valid_show_id(l_catalog, atoi(token)))
      {
        if (mode == 1)
          l_views = Update_views(l_views, atoi(token));
        else
          l_views = Add_new_view(l_views, atoi(token), 1);
      }
    }
  }
  fclose(f_interactions);

  return l_views;
}

LViews *Update_views(LViews *l_views, int title_id)
{
  LViews *aux;

  if (l_views == NULL)
  {
    l_views = Add_new_view(l_views, title_id, 1);
    return l_views;
  }

  // Check the 1st postion
  if (l_views->show_id == title_id)
  {
    l_views->views++;
    return l_views;
  }

  // Check the remaining positions, always checking the next to be able to keep track of the
  // current position, otherwise it could turn into a NULL at the end and we would lost the track of it
  aux = l_views;
  while (aux && aux->next && aux->next->show_id != title_id)
    aux = aux->next;

  if (aux->next)
    aux->next->views++;
  else
    aux->next = Add_new_view(aux->next, title_id, 1);

  return l_views;
}

// We're not using bigdata or anything close to it, due that I will use bubble sorte
LViews *Sort_views(LViews *l_views)
{
  int counter = 0;
  LViews *aux, holder;

  aux = l_views;
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
    l_views = Sort_views(l_views);

  return l_views;
}

LCategory *Most_popular_categories(LCategory *l_category, LViews *l_views, LCatalog *l_catalog)
{
  LViews *views;
  LCatalog *catalog;

  views = l_views;
  while (views)
  {
    catalog = l_catalog;
    while (catalog && catalog->id != views->show_id)
      catalog = catalog->next;
    l_category = Update_Categories_popularity(l_category, catalog->category, views->views);
    views = views->next;
  }

  l_category = Sort_categories(l_category);
  return l_category;
}

LCategory *Update_Categories_popularity(LCategory *l_category, char *c_name, int views)
{
  LCategory *category_aux;

  // If the list is empty, add a new item.
  if (l_category == NULL)
  {
    l_category = Add_new_category(l_category, c_name, views);
    return l_category;
  }

  // check if the 1st position, if there's the same category, update the views
  if (strcmp(l_category->category, c_name) == 0)
  {
    l_category->counter += views;
    return l_category;
  }

  // Verify the other positions
  category_aux = l_category;

  while (category_aux && category_aux->next && strcmp(category_aux->next->category, c_name) != 0)
    category_aux = category_aux->next;

  if (category_aux->next)
    category_aux->next->counter += views;
  else
    category_aux->next = Add_new_category(category_aux->next, c_name, views);

  return l_category;
}

LCategory *Sort_categories(LCategory *l_category)
{
  int counter = 0, views_holder;
  char cat_holder[255];
  LCategory *aux;

  aux = l_category;
  while (aux && aux->next)
  {
    if (aux->counter < aux->next->counter)
    {
      strcpy(cat_holder, aux->category);
      views_holder = aux->counter;
      free(aux->category);
      aux->category = strdup(aux->next->category);
      aux->counter = aux->next->counter;
      free(aux->next->category);
      aux->next->category = strdup(cat_holder);
      aux->next->counter = views_holder;
      counter++;
    }
    aux = aux->next;
  }

  if (counter > 0)
    l_category = Sort_categories(l_category);

  return l_category;
}
