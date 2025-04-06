#ifndef DATABASE_H
#define DATABASE_H
#include "listas.h"

TUser *Login(FILE *f_user);
TUser *Parse_user(int id, char *username);
void Report(LCatalog *l_catalog);

#endif