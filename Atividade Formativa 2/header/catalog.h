#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H
#include <stdio.h>
#include "listas.h"
#include "search.h"

#define WATCH_LATER 1

LCatalog *Initial_menu(LCatalog *l_catalog, LInteractions *l_interactions, LFavorite *l_favorite, char *file_name, TFiles *t_files);
void Print_catalog(LCatalog *list);
int Edit_item(LCatalog *l_catalog, int content_to_edit, int id);
LCatalog *User_add_new_title(LCatalog *l_catalog);
LCatalog *User_remove_title(LCatalog *l_catalog, int id);
FILE *Save_Catalog(LCatalog *l_catalog, FILE *f, char *fileName);
LInteractions *Watch_a_movie(LInteractions *l_interactions, LCatalog *l_catalog, LFavorite *l_favorites, FILE *f_interactions, FILE *f_favorites);
LFavorite *Load_favorites(FILE *f_favorites);
void Show_playlist_titles(LFavorite *l_favorites);
LFavorite *Create_new_favorite(LFavorite *l_favorite, FILE *f_favorites);
void Add_to_playlist(LFavorite *l_favorite, FILE *f_favorites, int list_id, int title_id, char *title);
int Is_favorited(LFavorite *l_favorite, int list_id, int movie_id);
void Add_to_custom_playlist(LFavorite *l_favorites, FILE *f_favorites, int title_id, char *title);

#endif