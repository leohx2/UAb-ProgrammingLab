#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H
#include <stdio.h>
#include "listas.h"
#include "search.h"

LCatalog *Initial_menu(LCatalog *l_catalog, LInteractions *l_interactions, char *file_name, TFiles *files);
void Print_catalog(LCatalog *list);
int Edit_item(LCatalog *l_catalog, int content_to_edit, int id);
LCatalog *User_add_new_title(LCatalog *l_catalog);
LCatalog *User_remove_title(LCatalog *l_catalog, int id);
FILE *Save_Catalog(LCatalog *l_catalog, FILE *f, char *fileName);
LInteractions *Watch_a_movie(LInteractions *l_interactions, LCatalog *l_catalog, FILE *f_interactions);

#endif