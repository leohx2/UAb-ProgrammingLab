#ifndef LISTA_H
#define LISTA_H
#include <stdio.h>
typedef struct SCatalog
{
  char *category;
  char *pegi;
  int duration;
  char *title;
  int views;
  int id;
  struct SCatalog *next;
} LCatalog;

typedef struct SInteractions
{
  int movie_id;
  char action; // Action('p'/'r'/'s'/'w' for pause/resume/stream/watched)
  struct SInteractions *next;
} LInteractions;

typedef struct SFavorite
{
  char *name;
  char *movie_name;
  int id;
  int title_id;
  struct SFavorite *next;
} LFavorite;
typedef struct
{
  char *username;
  int id;
} TUser;

typedef struct
{
  FILE *interactions;
  FILE *favorites;
  FILE *movies;
} TFiles;

typedef struct
{
  LCatalog *l_catalog;
  LInteractions *l_interactions;
  LFavorite *l_playlist;
} TLists;

typedef struct SViews
{
  int show_id;
  int views;
  struct SViews *next;
} TViews;

LCatalog *Save_on_list(FILE *file, LCatalog *l_catalog);
LCatalog *Add_new(LCatalog *list, LCatalog *aux);
LCatalog *Free_current_list(LCatalog *list);
LCatalog *Free_all_lists(LCatalog *list);
TUser *Free_user(TUser *t_user);
LInteractions *Load_Interactions(FILE *f_interactions);
LInteractions *New_interaction(LInteractions *l_interaction, char action, int movie_id);
LInteractions *Free_all_interactions(LInteractions *l_interactions);
LFavorite *Add_favorite_in_order(LFavorite *l_favorite, int list_id, int title_id, char *list_name, char *movie_name);
LFavorite *Free_playlist(LFavorite *l_favorites);
LFavorite *Free_all_playlists(LFavorite *l_favorites);
TViews *Free_all_views(TViews *t_views);
TViews *Add_new_view(TViews *t_views, int title_id, int views);

#endif