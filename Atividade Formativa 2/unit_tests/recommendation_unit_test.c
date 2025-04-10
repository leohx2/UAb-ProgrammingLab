#include "../header/helper.h"
#include "../header/database.h"
#include "../header/catalog.h"
#include "../header/listas.h"
#include <stdlib.h>
#include <string.h>

#define MOVIE_1 8
#define MOVIE_2 11
#define MOVIE_3 13
#define MOVIE_4 18
#define MOVIE_5 20

FILE *Open_movies();
LCatalog *Read_movies(FILE *movies);
FILE *Open_interactions(char *username);

// Create or re-create an user with no interactions, ask for a recommendation, add an interaction and as for another recommendation.
// This file will test the recommendation syste and the interactions as well.
int main()
{
  FILE *f_movies, *f_interaction;
  LCatalog *l_catalog;
  char *s = "user_tester";

  l_catalog = NULL;
  f_movies = NULL;
  f_interaction = NULL;

  // Test 1
  printf("*Test 1/5 -> Open movies.csv:\n");
  f_movies = Open_movies();
  if (f_movies == NULL)
    return EXIT_FAILURE;

  // Test 2
  printf("\n\n*Test 2/5 -> Read movies.csv:\n");
  l_catalog = Read_movies(f_movies);
  if (l_catalog == NULL)
  {
    fclose(f_movies);
    return EXIT_FAILURE;
  }

// This user wont be registered at our user.csv but we need to use the main database for this test
#ifdef _WIN32
  Create_new_csv(".\\database_csv\\interaction_", s, 1);
#else
  Create_new_csv("./database_csv/interaction_", s, 1);
#endif

  // Test 3
  printf("\n\n*Test 3/5 -> Read interactions.csv:\n");
  f_interaction = Open_interactions(s);
  if (f_interaction == NULL)
  {
    fclose(f_movies);
    l_catalog = Free_all_lists(l_catalog);
    return EXIT_FAILURE;
  }

  printf("\n\n*Test 4/5 -> Recommendation for a new user, with no movie history:\n");
  Recommendation(l_catalog, s);

  // To change the wathched movies, change these macros, they represent the movie ID
  printf("\n\n*Adding interactions\n");
  Save_Interactions(f_interaction, 'w', MOVIE_1);
  Save_Interactions(f_interaction, 'w', MOVIE_2);
  Save_Interactions(f_interaction, 'w', MOVIE_3);
  Save_Interactions(f_interaction, 'w', MOVIE_4);
  Save_Interactions(f_interaction, 'w', MOVIE_5);
  // update the interactions_file
  // fclose(f_interaction);
  // f_interaction = Open_interactions(s);

  printf("\n\n*Test 5/5 -> Recommendation for a new user, with movie history:\n");
  Recommendation(l_catalog, s);

  fclose(f_movies);
  l_catalog = Free_all_lists(l_catalog);
  printf("\n\n*This tester won't work as a right or wrong test, it's more interpretative to find out if there are good recommendations or not\n");
  return EXIT_SUCCESS;
}

// 1
FILE *Open_movies()
{
  FILE *movies;
  movies = fopen("movies_test.csv", "r");
  if (movies == NULL)
  {
    printf("Open movies -> ERROR, can't open it\n");
    return NULL;
  }
  else
    printf("Open movies -> Working!\n");

  return movies;
}

// 2
LCatalog *Read_movies(FILE *movies)
{
  LCatalog *l_catalog;

  l_catalog = NULL;

  l_catalog = Save_on_list(movies, l_catalog);

  if (l_catalog == NULL)
  {
    printf("Open and read movies -> ERROR, can't read it\n");
    return NULL;
  }
  else
    printf("Read movies -> Working!\n");

  return l_catalog;
}

// 3
FILE *Open_interactions(char *username)
{
  FILE *interactions;

#ifdef _WIN32
  interactions = Open_interaction_csv(".\\database_csv\\interaction_", username, "r+");
#else
  interactions = Open_interaction_csv("./database_csv/interaction_", username, "r+");
#endif
  if (interactions == NULL)
  {
    printf("Open interactions -> ERROR, can't open it\n");
    return NULL;
  }
  else
    printf("Open interactions -> Working!\n");

  return interactions;
}