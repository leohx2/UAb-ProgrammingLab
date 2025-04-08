#ifndef HELPER_H
#define HELPER_H
#include "listas.h"

void To_upper_str(char *s);
void Remove_plus_signals(char *s);
void Print_title_details(LCatalog *l_catalog);
int Get_bigger_id(LCatalog *l_catalog);
void Create_new_csv(char *path, char *username, int mode);
FILE *Open_interaction_csv(char *path, char *username, char *mode);
int Safe_answer();
int Pegi_atoi(char *pegi);
char *Get_show_info(LCatalog *l_catalog, int id, int mode);

#endif