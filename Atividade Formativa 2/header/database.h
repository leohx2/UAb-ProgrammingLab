#ifndef DATABASE_H
#define DATABASE_H
#include "listas.h"

TUser *Login(FILE *f_user);
TUser *Parse_user(int id, char *username);
void Save_Interactions(FILE *f_interactions, char action, int movie_id);
void Report(LCatalog *l_catalog);
LViews *Streamed_shows(char *username, LViews *l_views, LCatalog *l_catalog, int *interactions_counter, int max);
void Recommendation(LCatalog *l_catalog, char *username);

#endif