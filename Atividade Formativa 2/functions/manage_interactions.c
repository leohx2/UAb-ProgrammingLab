#include "../header/listas.h"
#include "../header/helper.h"
#include "../header/catalog.h"
#include <stdlib.h>
#include <string.h>

#define MAXSTR 255

LInteractions *Load_Interactions(FILE *f_interactions)
{
  LInteractions *l_interactions = NULL;
  char str[MAXSTR], *token, action;
  int movie_id;

  // Ignore the files first line
  fgets(str, MAXSTR, f_interactions);

  // interaction.csv is splitted in two parts, action and title_id
  while (!feof(f_interactions))
  {
    fgets(str, MAXSTR, f_interactions);

    // Get the action
    token = strtok(str, ",");
    action = token[0];

    // Get the movie id;
    token = strtok(NULL, ",");
    movie_id = atoi(token);
    l_interactions = New_interaction(l_interactions, action, movie_id);
  }

  return l_interactions;
}

// Returns n if there's no recorded data about the current id.
char Get_movie_current_status(LInteractions *l_interactions, int id)
{
  // We need to ignore the Add and remove from playlist
  while (l_interactions)
  {
    if (l_interactions->movie_id == id && l_interactions->action != 'A' && l_interactions->action != 'R')
      return l_interactions->action;
    l_interactions = l_interactions->next;
  }

  return 'n';
}

void Save_Interactions(FILE *f_interactions, char action, int movie_id)
{
  fseek(f_interactions, 0, SEEK_END);
  fprintf(f_interactions, "\n%c,%d", action, movie_id);
}

LInteractions *Update_Interactions(LInteractions *l_interactions, int action, int movie_id, FILE *f_interactions)
{
  if (action == 1 || action == 's')
  {
    l_interactions = New_interaction(l_interactions, 's', movie_id);
    Save_Interactions(f_interactions, 's', movie_id);
  }
  else if (action == 4 || action == 'r')
  {
    l_interactions = New_interaction(l_interactions, 'r', movie_id);
    Save_Interactions(f_interactions, 'r', movie_id);
  }
  else if (action == 5 || action == 'w')
  {
    l_interactions = New_interaction(l_interactions, 'w', movie_id);
    Save_Interactions(f_interactions, 'w', movie_id);
  }
  else if (action == 'p')
  {
    l_interactions = New_interaction(l_interactions, 'p', movie_id);
    Save_Interactions(f_interactions, 'p', movie_id);
  }
  else if (action == 'A')
  {
    l_interactions = New_interaction(l_interactions, 'A', movie_id);
    Save_Interactions(f_interactions, 'A', movie_id);
  }
  else if (action == 'R')
  {
    l_interactions = New_interaction(l_interactions, 'R', movie_id);
    Save_Interactions(f_interactions, 'R', movie_id);
  }

  return l_interactions;
}

LInteractions *Watching(LInteractions *l_interactions, FILE *f_interactions, LCatalog *l_catalog, int movie_id)
{
  int user_choice = 0, paused = 0;
  printf("\n-----------------------------------------\n");
  printf("You're watching '%s'\n", l_catalog->title);

  // Options -> 1-> Pause, 1-> Resume (if it's paused), 2-> mark as done, 3-> exit to main menu
  while (user_choice != 3)
  {
    // If paused, display "Resume", otherwise "Pause"
    printf("\n1 - %s\t2 - Mark as done\t3 - Exit to main menu\nOption: ", paused ? "Resume" : "Pause");
    user_choice = Safe_answer();

    if (user_choice != 1 && user_choice != 2 && user_choice != 3)
      printf("\nInvalid Option.\n");
    else if (user_choice == 1)
    {
      // if the movie is paused (paused == 1) and user choose the option 1, he's choosing resume, otherwise he's choosing pause
      l_interactions = Update_Interactions(l_interactions, paused ? 'r' : 'p', movie_id, f_interactions);
      paused = !paused;
    }
    else if (user_choice == 2)
    {
      printf("\nThank you for watching '%s' :)\n", l_catalog->title);
      printf("\n-----------------------------------------\n");
      l_catalog->views++;
      l_interactions = Update_Interactions(l_interactions, 'w', movie_id, f_interactions);
      return l_interactions;
    }
  }

  // If we get here the user asked to go back to main menu, we need to "pause", save and then exit.
  l_interactions = Update_Interactions(l_interactions, 'p', movie_id, f_interactions);
  printf("\n-----------------------------------------\n");
  return l_interactions;
}

LInteractions *Watch_a_movie(TLists *t_lists, TFiles *t_files, char *username)
{
  int title_id;
  char current_status;
  LCatalog *hold_1st_p;

  printf("\nInsert the title id or -1 to return to main menu\n");
  printf("Title id: ");
  title_id = Safe_answer();

  if (title_id == -1)
    return t_lists->l_interactions;

  hold_1st_p = t_lists->l_catalog;
  while (t_lists->l_catalog && t_lists->l_catalog->id != title_id)
    t_lists->l_catalog = t_lists->l_catalog->next;

  if (t_lists->l_catalog == NULL)
  {
    printf("\nTitle not found.\n");
    t_lists->l_catalog = hold_1st_p;
    return t_lists->l_interactions;
  }

  current_status = Get_movie_current_status(t_lists->l_interactions, title_id);

  t_lists->l_interactions = Handle_movies_choices(t_lists, t_files, current_status, title_id, username);
  t_lists->l_catalog = hold_1st_p;
  return t_lists->l_interactions;
}

LInteractions *Handle_movies_choices(TLists *t_lists, TFiles *t_files, char current_status, int title_id, char *username)
{
  int user_choice = 0, is_favorited;

  while (user_choice != 1 && user_choice != 4 && user_choice != 5 && user_choice != -1)
  {
    is_favorited = Is_favorited(t_lists->l_playlist, WATCH_LATER, title_id);
    printf("\n-----------------------------------------\n");
    printf("You selected '%s'\n", t_lists->l_catalog->title);
    printf("Would you like to:\n");
    // If we get here there's a valid user choice and we'll get the movie current status.

    // n stands for none and w for watched
    if (current_status == 'n' || current_status == 'w')
      printf(" 1 - Start\n 2 - %s watch later\n 3 - Add to a playlist\n-1 - Main menu\n", is_favorited ? "Remove from" : "Save on");
    else if (current_status == 'p')
      printf(" 1 - Start from the beginning\n 2 - %s watch later\n 3 - Add to a playlist\n 4 - Resume\n 5 - Mark as done\n-1 - Main menu\n", is_favorited ? "Remove from" : "Save on");

    printf("Option: ");
    user_choice = Safe_answer();

    printf("\n-----------------------------------------\n");
    if (user_choice < -1 || user_choice > 5 || user_choice == 0)
      printf("Invalid Option\n");
    else if (user_choice == -1)
      return t_lists->l_interactions;
    else if (user_choice == 2)
    {
      // If it's already saved on watch latter, removes, otherwise add.
      t_lists->l_interactions = Update_Interactions(t_lists->l_interactions, is_favorited ? 'R' : 'A', title_id, t_files->interactions);
      if (is_favorited == 0)
        Add_to_playlist(t_lists->l_playlist, t_files->favorites, WATCH_LATER, title_id, t_lists->l_catalog->title);
      else if (is_favorited == 1)
        t_lists->l_playlist = Remove_from_playlist(t_lists->l_playlist, t_files->favorites, WATCH_LATER, title_id, username);
    }
    else if (user_choice == 3)
    {
      t_lists->l_interactions = Update_Interactions(t_lists->l_interactions, 'A', title_id, t_files->interactions);
      Add_to_custom_playlist(t_lists->l_playlist, t_files->favorites, title_id, t_lists->l_catalog->title);
    }
    else
      t_lists->l_interactions = Update_Interactions(t_lists->l_interactions, user_choice, title_id, t_files->interactions);
  }
  if (user_choice == 1 || user_choice == 4)
    t_lists->l_interactions = Watching(t_lists->l_interactions, t_files->interactions, t_lists->l_catalog, title_id);

  // Increase de views numbers when mark as done.
  if (user_choice == 5)
    t_lists->l_catalog->views++;
  return t_lists->l_interactions;
}