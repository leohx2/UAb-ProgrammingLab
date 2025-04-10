#include "../header/helper.h"
#include "../header/database.h"
#include "../header/catalog.h"
#include "../header/listas.h"
#include <stdlib.h>
#include <string.h>
#define MAXSTR 255

int main()
{
  TUser *user;
  FILE *f_user;
  int i = 0;
  char username[5][30] = {
      "Banguela",
      "Cafe",
      "Pandora",
      "Julio",
      "Teresa"};

  printf("Testing if CSV making is working properly for new users\n");

#ifdef _WIN32
  f_user = fopen(".\\database_csv\\user.csv", "r+");
#else
  f_user = fopen("./database_csv/user.csv", "r+");
#endif

  if (f_user == NULL)
  {
    printf("\nError. user database not found\n");
    return EXIT_FAILURE;
  }

  printf("Please, erase the generated content to try again\n");

  fseek(f_user, 0, SEEK_END);
  for (i = 0; i < 5; i++)
  {
    fprintf(f_user, "\n%d,%s", i + 100, username[i]);
#ifdef _WIN32
    Create_new_csv(".\\database_csv\\interaction_", username[i], 1);
    Create_new_csv(".\\database_csv\\lists_", username[i], 2);
#else
    Create_new_csv("./database_csv/interaction_", username[i], 1);
    Create_new_csv("./database_csv/lists_", username[i], 2);
#endif
    user = Parse_user(i + 100, username[i]);
    if (user)
      printf("User %s created\n", user->username);
    user = Free_user(user);
  }
  printf("\nDone, check database_csv for 5 new users, Banguela, Cafe, Pandora, Julio and Teresa\n");
  fclose(f_user);
}
