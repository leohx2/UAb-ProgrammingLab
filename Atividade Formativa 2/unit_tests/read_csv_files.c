/* This file will test if we can read all the csv porpely,
if there is no error while trying to acess a user file or an inexistent file
*/
#include "../header/helper.h"
#include "../header/catalog.h"
#include "../header/listas.h"
#include <stdlib.h>
#include <string.h>
#define MAXSTR 255

int Open_and_read_movies();
FILE *Open_user_file();
int Reading_interactions_and_lists(FILE *users);

int main()
{
  FILE *user;

  // test 1 and 2
  if (Open_and_read_movies() == EXIT_FAILURE)
    return EXIT_FAILURE;

  // Test 3
  user = Open_user_file();
  if (user == NULL)
    return EXIT_FAILURE;

  // Test 4, reading interactions, and lists from users
  if (Reading_interactions_and_lists(user) == EXIT_FAILURE)
  {
    fclose(user);
    return EXIT_FAILURE;
  }

  fclose(user);
  printf("\nAll tests passed!\n");
  return EXIT_SUCCESS;
}

int Open_and_read_movies()
{
  FILE *movies;
  LCatalog *l_catalog;

  l_catalog = NULL;

  printf("Test 1/4 -> Open movies.csv\n");
  movies = fopen("movies_test.csv", "r");
  if (movies == NULL)
  {
    printf("Open movies -> ERROR, can't open it\n");
    return EXIT_FAILURE;
  }
  else
    printf("Open movies -> Working!\n");

  printf("\nTest 2/4 -> Read movies.csv\n");

  l_catalog = Save_on_list(movies, l_catalog);
  fclose(movies);

  if (l_catalog == NULL)
  {
    printf("Open and read movies -> ERROR, can't read it\n");
    return EXIT_FAILURE;
  }
  else
    printf("Read movies -> Working!\n");

  l_catalog = Free_all_lists(l_catalog);
  return EXIT_SUCCESS;
}

FILE *Open_user_file()
{
  FILE *user;

  printf("\nTest 3/4 -> Open user.csv\n");
#ifdef _WIN32
  user = fopen(".\\database_csv\\user.csv", "r");
#else
  user = fopen("./database_csv/user.csv", "r");
#endif

  if (user == NULL)
    printf("Open user file -> ERROR\n");
  else
    printf("Open user file -> Working!\n");
  return user;
}

int Reading_interactions_and_lists(FILE *users)
{
  char s[MAXSTR], *token;
  FILE *interaction, *list;

  printf("\nTest 4/4 -> Reading interactions and lists csv files\n");

  // Ignore csv first line
  fgets(s, MAXSTR, users);

  while (!feof(users))
  {
    fgets(s, MAXSTR, users);

    // first part is an ID, the second is the username;
    token = strtok(s, ",");
    token = strtok(NULL, ",");
    if (token[strlen(token) - 1] == '\n')
      token[strlen(token) - 1] = '\0';
#ifdef _WIN32
    interaction = Open_interaction_csv(".\\database_csv\\interaction_", token, "r");
    list = Open_interaction_csv(".\\database_csv\\lists_", token, "r");
#else
    interaction = Open_interaction_csv("./database_csv/interaction_", token, "r");
    list = Open_interaction_csv("./database_csv/lists_", token, "r");
#endif
    if (interaction == NULL)
    {
      printf("Reading interactions and lists csv files -> ERROR, interaction not found\n");
      if (list)
        fclose(list);
      return EXIT_FAILURE;
    }
    else if (list == NULL)
    {
      printf("Reading interactions and lists csv files -> ERROR, list not found\n");
      if (interaction)
        fclose(interaction);
      return EXIT_FAILURE;
    }
    printf("Reading user '%s' intereactions and lists -> Working!\n", token);
    fclose(list);
    fclose(interaction);
  }
  printf("Reading interactions and lists csv files -> working!\n");
  return EXIT_SUCCESS;
}
