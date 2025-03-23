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
typedef struct
{
  char *username;
  int id;
} TUser;

LCatalog *Save_on_list(FILE *file, LCatalog *l_catalog);
LCatalog *Add_new(LCatalog *list, LCatalog *aux);
LCatalog *Free_current_list(LCatalog *list);
LCatalog *Free_all_lists(LCatalog *list);
TUser *Free_user(TUser *t_user);

#endif