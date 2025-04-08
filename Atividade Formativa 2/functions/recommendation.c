#include "../header/database.h"
#include "../header/catalog.h"
#include "../header/listas.h"
#include "../header/helper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_HISTORY 5
#define MAX_RECOMMENDATION 3

LRecommendation *Recommendation_algorithm(LViews *l_views, LCatalog *l_catalog, int avg_duration, int avg_pegi);
LRecommendation *Update_recommendations(LRecommendation *l_recommendation, LCatalog *l_catalog, int points);
void Avarage_show(LViews *l_views, LCatalog *l_catalog, int *avg_duration, int *avg_pegi);
int Category_Points(char *show_category, char *catalog_category);
int Avg_points(int avg_valeu, int value, int mode);
int Is_valid_recommendation(LViews *l_views, int id);
LRecommendation *Sort_recommendations(LRecommendation *l_recommendation);

void Recommendation(LCatalog *l_catalog, char *username)
{
  LViews *l_views;
  LRecommendation *l_recommendation, *display_recommendation;
  int avg_durantion, avg_pegi, i = 1;

  printf("\n-----------------------------------------\n");
  printf("\nHey, %s! Here's our recommendations for you: \n", username);
  l_views = NULL;
  l_recommendation = NULL;

  // Recommendation based on the last 5 watched shows.
  l_views = Streamed_shows(username, l_views, l_catalog, NULL, 2);
  Avarage_show(l_views, l_catalog, &avg_durantion, &avg_pegi);
  l_recommendation = Recommendation_algorithm(l_views, l_catalog, avg_durantion, avg_pegi);
  l_recommendation = Sort_recommendations(l_recommendation);

  display_recommendation = l_recommendation;
  while (display_recommendation && i <= MAX_RECOMMENDATION)
  {
    printf("\n%d:\nShow: %s\nCategory: %s\nID: %d\n", i, display_recommendation->show_name, display_recommendation->categories, display_recommendation->show_id);
    display_recommendation = display_recommendation->next;
    i++;
  }
  if (i == 1)
    printf("\nWe'll give better recommendations after you watch a few shows! Come back later, you won't regret it! :)\n");
  printf("\n-----------------------------------------\n");
  l_views = Free_all_views(l_views);
  l_recommendation = Free_all_recommendations(l_recommendation);
}

// Just look for the last 5 watched shows
void Avarage_show(LViews *l_views, LCatalog *l_catalog, int *avg_duration, int *avg_pegi)
{
  LViews *views;
  LCatalog *catalog;
  int sum_duration = 0, sum_pegi = 0, counter = 0, i = 0;

  views = l_views;

  while (views && i < MAX_HISTORY)
  {
    catalog = l_catalog;
    while (catalog && catalog->id != views->show_id)
      catalog = catalog->next;

    if (catalog)
    {
      sum_duration += catalog->duration;
      sum_pegi += Pegi_atoi(catalog->pegi);
      counter++;
    }
    views = views->next;
    i++;
  }

  *avg_duration = sum_duration / counter;
  *avg_pegi = sum_pegi / counter;
}

/* POINTS System:
100 points: Same category
10 points: Same sub-category
10/[abs(show_pegi - avg_pegi) + 1] points
30/[abs(show_duration - avg_duration) + 1] points
*/
LRecommendation *Recommendation_algorithm(LViews *l_views, LCatalog *l_catalog, int avg_duration, int avg_pegi)
{
  LRecommendation *l_recommendation;
  LViews *views;
  LCatalog *catalog;
  int points, i = 0;
  char *categories, holder[255];

  l_recommendation = NULL;
  views = l_views;

  while (views && i < MAX_HISTORY)
  {
    catalog = l_catalog;
    categories = Get_show_info(l_catalog, views->show_id, 2);
    To_upper_str(categories);

    while (catalog)
    {
      points = 0;
      if (Is_valid_recommendation(l_views, catalog->id))
      {
        strcpy(holder, catalog->category);
        To_upper_str(holder);
        points += Category_Points(categories, holder);
        points += Avg_points(avg_duration, catalog->duration, 1);
        points += Avg_points(avg_pegi, Pegi_atoi(catalog->pegi), 2);
      }
      if (points)
        l_recommendation = Update_recommendations(l_recommendation, catalog, points);

      catalog = catalog->next;
    }

    if (categories)
      free(categories);

    views = views->next;
    i++;
  }

  return l_recommendation;
}

LRecommendation *Update_recommendations(LRecommendation *l_recommendation, LCatalog *l_catalog, int points)
{
  LRecommendation *recommendation;

  // Checkt if the list is empty
  if (l_recommendation == NULL)
  {
    l_recommendation = Add_new_recommendation(l_recommendation, points, l_catalog->id, l_catalog->title, l_catalog->category);
    return l_recommendation;
  }

  // Check the 1st postion, if the show is already there to update the points
  if (l_recommendation->show_id == l_catalog->id)
  {
    l_recommendation->points += points;
    return l_recommendation;
  }

  // Check all the remaining list items
  recommendation = l_recommendation;

  while (recommendation && recommendation->next)
  {
    if (recommendation->next->show_id == l_catalog->id)
    {
      recommendation->next->points += points;
      return l_recommendation;
    }
    recommendation = recommendation->next;
  }

  recommendation->next = Add_new_recommendation(recommendation->next, points, l_catalog->id, l_catalog->title, l_catalog->category);

  return l_recommendation;
}

// Imagine the a show has two categories, ADVENTURE/DRAMA, it's category is "ADVENTURE/DRAMA" but there's two
// sub-categories, "ADVENTURE" and "DRAMA"
int Category_Points(char *show_category, char *catalog_category)
{
  int points = 0;
  char *catalog_sub_category;

  if (strcmp(show_category, catalog_category) == 0)
    return 100;
  else
  {
    catalog_sub_category = strtok(catalog_category, "/");
    while (catalog_sub_category != NULL)
    {
      if (strstr(show_category, catalog_sub_category) != NULL)
        points += 25;
      catalog_sub_category = strtok(NULL, "/");
    }
  }

  // Controll to avoid someone adding a title with all categories existens, it would always have 100 points
  // and the only way to have those 100 points is matching the category, not by having a lot sub categories in commun.
  if (points > 75)
    points = 75;
  return points;
}

// points by avg_duration and avg_pegi
// mode: 1 for duration, 2 for pegi
int Avg_points(int avg_valeu, int value, int mode)
{
  int diff, result;

  // this + 1 will have an essential role, to prevent a division by 0.
  diff = abs(avg_valeu - value) + 1;
  if (mode == 1)
    result = 60 / diff; // 60 points is the maximum here. Keeped it as an int to round it to the lower value.
  else
  {
    // 50 points is the maximum, and I don't want give a show points if the PEGI diff
    // is greater than 5.
    if (diff > 5)
      result = 0;
    else
      result = 50 / diff;
  }

  return result;
}

// We don't want to recommend a movie watched recently
int Is_valid_recommendation(LViews *l_views, int id)
{
  LViews *aux;
  int i = 0;

  aux = l_views;
  while (aux && i < MAX_HISTORY)
  {
    if (id == aux->show_id)
      return 0;
    aux = aux->next;
    i++;
  }

  return 1;
}

LRecommendation *Sort_recommendations(LRecommendation *l_recommendation)
{
  int counter = 0;
  LRecommendation *recommendation, *aux;

  recommendation = l_recommendation;

  while (recommendation && recommendation->next)
  {
    if (recommendation->points < recommendation->next->points)
    {
      aux = Add_new_recommendation(aux, recommendation->points, recommendation->show_id, recommendation->show_name, recommendation->categories);
      recommendation->points = recommendation->next->points;
      recommendation->show_id = recommendation->next->show_id;
      free(recommendation->show_name);
      free(recommendation->categories);
      recommendation->show_name = strdup(recommendation->next->show_name);
      recommendation->categories = strdup(recommendation->next->categories);

      recommendation->next->points = aux->points;
      recommendation->next->show_id = aux->show_id;
      free(recommendation->next->show_name);
      free(recommendation->next->categories);
      recommendation->next->show_name = strdup(aux->show_name);
      recommendation->next->categories = strdup(aux->categories);

      counter++;
    }

    recommendation = recommendation->next;
  }

  if (counter > 0)
    l_recommendation = Sort_recommendations(l_recommendation);

  return l_recommendation;
}
