#include "header/database.h"
#include "header/helper.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXSTR 255

TUser *Parse_user(int id, char *username)
{
  TUser *user;

  user = (TUser *)malloc(sizeof(TUser));
  if (user == NULL)
    return NULL;

  user->username = (char *)malloc(sizeof(char) * strlen(username) + 1);
  if (user->username == NULL)
  {
    free(user);
    return NULL;
  }
  strcpy(user->username, username);
  user->id = id;
  return user;
}

// mode: 'f' to login, 'c' to create
TUser *Find_or_create_user(FILE *f_user, char *username, int mode)
{
  char s[MAXSTR], *token;
  int bigger_id = 0, user_id;
  TUser *user;

  // Ignore the fist line, that will tell us the file order, in that case "user_id,username"
  fgets(s, MAXSTR, f_user);

  while (!feof(f_user))
  {
    fgets(s, MAXSTR, f_user);
    token = strtok(s, ",");

    // Keep track of the bigger ID in case we need to create a new user
    user_id = atoi(token);
    if (user_id > bigger_id)
      bigger_id = user_id;

    token = strtok(NULL, ",");
    if (token[strlen(token) - 1] == '\n')
      token[strlen(token) - 1] = '\0';

    if (strcmp(username, token) == 0)
    {
      // If we're using the creation mode, we don't want a user that already exist.
      if (mode == 'c')
      {
        printf("\nUser Already exist.\n");
        rewind(f_user);
        return Login(f_user);
      }
      user = Parse_user(user_id, username);
      if (user == NULL)
        printf("Error. Memory allocation, Find_user.\n");
      return user;
    }
  }

  if (mode == 'f')
  {
    rewind(f_user);
    printf("\nUser not found.\n");
    return Login(f_user);
  }
  // Create a new user and csv file for that user
  fprintf(f_user, "\n%d,%s", ++bigger_id, username);
#ifdef _WIN32
  Create_new_csv(".\\database_csv\\interaction_", username, 1);
  Create_new_csv(".\\database_csv\\lists_", username, 2);
#else
  Create_new_csv("./database_csv/interaction_", username, 1);
  Create_new_csv("./database_csv/lists_", username, 2);
#endif
  user = Parse_user(bigger_id, username);

  return user;
}

// returns the user file.csv
TUser *Login(FILE *f_user)
{
  TUser *t_user;
  char username[MAXSTR];
  int user_choice = 0;

  while (user_choice < 1 || user_choice > 2)
  {
    printf("\n1 - Login (no password required)\n2 - Create a new profile\nOption: ");
    user_choice = Safe_answer();
    if (user_choice < 1 || user_choice > 2)
      printf("Invalid option");
  }

  // Ask for a username or a new one based on the user_choice
  printf("\n%s", user_choice == 1 ? "Username: " : "New username: ");

  scanf(" %[^\n]", username);

  if (user_choice == 1)
    t_user = Find_or_create_user(f_user, username, 'f');
  else
    t_user = Find_or_create_user(f_user, username, 'c');

  return t_user;
}