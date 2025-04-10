#include "../header/helper.h"
#include "../header/database.h"
#include "../header/catalog.h"
#include "../header/listas.h"
#include <stdlib.h>
#include <string.h>

FILE *Open_movies();
LCatalog *Read_movies(FILE *movies);
int Add_movies(FILE *f_movie, LCatalog *l_catalog, char titles[5][255], char categories[5][255], char age_ratings[5][100], int durations[], int views[]);
int Edit_movies(FILE *f_movie, LCatalog *l_catalog, char titles[5][255], char categories[5][255], char age_ratings[5][100], int durations[], int views[]);
int Remove_movies(FILE *f_movie, LCatalog *l_catalog, char titles[5][255], char categories[5][255], char age_ratings[5][100], int durations[], int views[]);

int main()
{
  FILE *f_movies;
  LCatalog *l_catalog;
  char titles[5][255] = {
      "Sharknator\0",
      "The Shawshank Redemption\0",
      "Parasite\0",
      "Spirited Away\0",
      "The Dark Knight\0"};

  char categories[5][255] = {
      "Comedy\0",
      "Drama\0",
      "Thriller\0",
      "Animation\0",
      "Action\0"};

  int durations[] = {148, 142, 132, 125, 152};

  char age_ratings[5][100] = {
      "16+\0",
      "16+\0",
      "16+\0",
      "7+\0",
      "13+\0"};

  int views[] = {5000000, 8000000, 6500000, 4500000, 7500000};
  l_catalog = NULL;
  f_movies = NULL;

  // Test 1
  printf("\t* Test 1/5 -> Open movies.csv:\n");
  f_movies = Open_movies();
  if (f_movies == NULL)
    return EXIT_FAILURE;

  // Test 2
  printf("\n\n\t* Test 2/5 -> Read movies.csv:\n");
  l_catalog = Read_movies(f_movies);
  if (l_catalog == NULL)
  {
    fclose(f_movies);
    return EXIT_FAILURE;
  }

  // Test 3
  printf("\n\n\t* Test 3/5 Add new movies:\n");
  if (Add_movies(f_movies, l_catalog, titles, categories, age_ratings, durations, views) == EXIT_FAILURE)
  {
    fclose(f_movies);
    l_catalog = Free_all_lists(l_catalog);
    return EXIT_FAILURE;
  }

  // Test 4
  printf("\n\n\t* Test 4/5 Editing movies:\n");
  if (Edit_movies(f_movies, l_catalog, titles, categories, age_ratings, durations, views) == EXIT_FAILURE)
  {
    fclose(f_movies);
    l_catalog = Free_all_lists(l_catalog);
    return EXIT_FAILURE;
  }

  // TEST 5
  printf("\n\n\t* Test 5/5 Deleting movies:\n");
  if (Remove_movies(f_movies, l_catalog, titles, categories, age_ratings, durations, views) == EXIT_FAILURE)
  {
    fclose(f_movies);
    l_catalog = Free_all_lists(l_catalog);
    return EXIT_FAILURE;
  }

  fclose(f_movies);
  l_catalog = Free_all_lists(l_catalog);
  printf("\n\n*All good!*\n");
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

// Test 3
int Add_movies(FILE *f_movie, LCatalog *l_catalog, char titles[5][255], char categories[5][255], char age_ratings[5][100], int durations[], int views[])
{
  LCatalog *aux = NULL;
  int i = 0;

  while (i < 5)
  {
    aux = (LCatalog *)malloc(sizeof(LCatalog));
    if (aux == NULL)
    {
      printf("Add new movies -> Error memory allocation\n");
      return EXIT_FAILURE;
    }
    aux->title = strdup(titles[i]);
    aux->category = strdup(categories[i]);
    aux->duration = durations[i];
    aux->pegi = strdup(age_ratings[i]);
    aux->views = views[i];
    aux->id = Get_bigger_id(l_catalog) + 1;

    l_catalog = Add_new(l_catalog, aux);

    aux = Free_current_list(aux);
    i++;
  }
  // Checking if we properly added it
  printf("\nChecking catalog after adding all the movies\n");
  aux = l_catalog;
  i = 0;

  while (aux && i < 5)
  {
    if (strcmp(aux->title, titles[i]) == 0 && strcmp(aux->category, categories[i]) == 0 && aux->duration == durations[i] && strcmp(aux->pegi, age_ratings[i]) == 0)
    {
      printf("\n'%s' added successfully!\n", titles[i]);
      i++;
    }
    aux = aux->next;
  }

  if (i == 5)
    printf("\nAll movies added!\n");
  else
  {
    printf("Error, %d missing movies to add\n", 5 - i);
    return EXIT_FAILURE;
  }

  // Save on CSV
  printf("\nSaving changes and updating the CSV file\n");
  f_movie = Save_Catalog(l_catalog, f_movie, "movies_test.csv");

  return EXIT_SUCCESS;
}

// 4
int Edit_movies(FILE *f_movie, LCatalog *l_catalog, char titles[5][255], char categories[5][255], char age_ratings[5][100], int durations[], int views[])
{
  int i = 0;
  LCatalog *aux;

  aux = l_catalog;
  while (aux && i < 5)
  {
    if (strcmp(aux->title, titles[i]) == 0 && strcmp(aux->category, categories[i]) == 0 && aux->duration == durations[i] && strcmp(aux->pegi, age_ratings[i]) == 0)
    {
      free(aux->title);
      strcat(titles[i], " edited\0");
      aux->title = strdup(titles[i]);

      free(aux->category);
      strcat(categories[i], " edited\0");
      aux->category = strdup(categories[i]);

      durations[i]++;
      aux->duration = durations[i];

      free(aux->pegi);
      strcat(age_ratings[i], "edited\0");
      aux->pegi = strdup(age_ratings[i]);

      views[i]++;
      aux->views = views[i];
      i++;
    }
    aux = aux->next;
  }

  // Checking if we properly added it
  printf("\nChecking catalog after Editing all the movies\n");
  aux = l_catalog;
  i = 0;

  while (aux && i < 5)
  {
    if (strcmp(aux->title, titles[i]) == 0 && strcmp(aux->category, categories[i]) == 0 && aux->duration == durations[i] && strcmp(aux->pegi, age_ratings[i]) == 0)
    {
      printf("\n'%s' edited successfully!\n", titles[i]);
      i++;
    }
    aux = aux->next;
  }

  if (i < 5)
  {
    printf("ERROR %d movies not edited\n", 5 - i);
    return EXIT_FAILURE;
  }

  // Save on CSV
  printf("\nSaving changes and updating the CSV file\n");
  f_movie = Save_Catalog(l_catalog, f_movie, "movies_test.csv");

  return EXIT_SUCCESS;
}

// 5
int Remove_movies(FILE *f_movie, LCatalog *l_catalog, char titles[5][255], char categories[5][255], char age_ratings[5][100], int durations[], int views[])
{
  int i = 0, hold_id;
  LCatalog *aux;

  aux = l_catalog;
  while (aux && i < 5)
  {
    if (strcmp(aux->title, titles[i]) == 0 && strcmp(aux->category, categories[i]) == 0 && aux->duration == durations[i] && strcmp(aux->pegi, age_ratings[i]) == 0)
    {
      hold_id = aux->id;
      aux = aux->next;
      l_catalog = User_remove_title(l_catalog, hold_id);
      i++;
    }
    else
      aux = aux->next;
  }

  printf("\nChecking if everything was deleted\n");

  aux = l_catalog;
  i = 0;
  while (aux && i < 5)
  {
    if (strcmp(aux->title, titles[i]) == 0 && strcmp(aux->category, categories[i]) == 0 && aux->duration == durations[i] && strcmp(aux->pegi, age_ratings[i]) == 0)
    {
      printf("\nERROR. The movie '%s' was not deleted\n", aux->title);
      i++;
    }
    aux = aux->next;
  }

  if (i > 0)
    return EXIT_FAILURE;

  // Save on CSV
  printf("\nMovies deleted succesfully!\n");
  printf("\nSaving changes and updating the CSV file\n");
  f_movie = Save_Catalog(l_catalog, f_movie, "movies_test.csv");

  return EXIT_SUCCESS;
}
