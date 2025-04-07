#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H
#include <stdio.h>
#include "listas.h"
#include "search.h"

#define WATCH_LATER 1

LCatalog *Initial_menu(TLists *t_lists, char *file_name, TFiles *t_files, char *username);
void Print_catalog(LCatalog *list);
int Edit_item(LCatalog *l_catalog, int content_to_edit, int id);
LCatalog *User_add_new_title(LCatalog *l_catalog);
LCatalog *User_remove_title(LCatalog *l_catalog, int id);
FILE *Save_Catalog(LCatalog *l_catalog, FILE *f, char *fileName);
LInteractions *Watch_a_movie(TLists *t_lists, TFiles *t_files, char *username);
LFavorite *Load_favorites(FILE *f_favorites);
void Show_playlist_titles(TLists *t_lists, TFiles *t_files, char *username);
LFavorite *Create_new_favorite(LFavorite *l_favorite, FILE *f_favorites);
void Add_to_playlist(LFavorite *l_favorite, FILE *f_favorites, int list_id, int title_id, char *title);
int Is_favorited(LFavorite *l_favorite, int list_id, int movie_id);
void Add_to_custom_playlist(LFavorite *l_favorites, FILE *f_favorites, int title_id, char *title);
LFavorite *Remove_from_playlist(LFavorite *l_playlist, FILE *f_favorites, int playlist_id, int title_id, char *username);
int Remove_from_playlist_menu();
LInteractions *Handle_movies_choices(TLists *t_lists, TFiles *t_files, char current_status, int title_id, char *username);
void SavePlaylistFile(LFavorite *l_playlist, FILE *f_playlist, char *username);
void Delete_playlist(TLists *t_lists, TFiles *t_files, char *username);
LInteractions *Update_Interactions(LInteractions *l_interactions, int action, int movie_id, FILE *f_interactions);
void Print_Movie_name(LCatalog *l_catalog, int id);
int Valid_show_id(LCatalog *l_catalog, int id);

#endif