#include "headers/helper.h"
#include <ctype.h>

void Str_to_lower(char *s)
{
  int i;

  for (i = 0; s[i]; i++)
    s[i] = tolower(s[i]);
}

// Get user input without the '\n' in the end
void Get_str_input(char *input)
{
  scanf(" %[^\n]s", input);

  if (input[strlen(input) - 1] == '\n')
    input[strlen(input) - 1] = '\0';
}