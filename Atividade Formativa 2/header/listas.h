#ifndef LISTA_H
#define LISTA_H

typedef struct LCatalog
{
  char *category;
  char *pegi;
  int duration;
  char *title;
  int views;
  int id;
  struct LCatalog *next;
} LCatalog;

LCatalog *Save_on_list(FILE *file, LCatalog *l_catalog);
LCatalog *Add_new(LCatalog *list, LCatalog *aux);
LCatalog *Free_current_list(LCatalog *list);
LCatalog *Free_all_lists(LCatalog *list);
void Print_catalog(LCatalog *list);
int Edit_item(LCatalog *l_catalog, int content_to_edit, int id);

#endif