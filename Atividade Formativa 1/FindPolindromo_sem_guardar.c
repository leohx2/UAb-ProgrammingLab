/*
##############################################################################################################################
Enunciado:
Escreva um programa para encontrar palíndromos num ficheiro de texto.
Um palíndromo é uma sequência de caracteres que se lê da mesma forma, tanto da esquerda para a direita como da direita para a esquerda.
Exemplos de palíndromos: sacas, rapar, rodador, anilina. Considere apenas palíndromos de comprimentos entre 3 e 10 caracteres.

##############################################################################################################################
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSTR 255
#define MAXPALINDROMO 20

/*
Retiro as opções de lista para não guardar em memória o palíndromo analisado
*/

int IsAPalindromo(char *s, int size)
{
  int i;

  // Optei por não ser key sensitive
  for (i = 0; i < size / 2; i++)
    if (!isalnum(s[i]) || tolower(s[i]) != tolower(s[size - i - 1]))
      return 0;

  return 1;
}

int FindPalindromos(char *s)
{
  int str_size;

  str_size = strlen(s);

  if (IsAPalindromo(s, str_size) && (str_size >= 3 && str_size <= 10))
  {
    printf("%s\n", s);
    return 1;
  }
  else
    return 0;
}

int LocalizaDaFraseOuIndevido(char c)
{
  int i;
  char s[7] = "\n\t ,.;";

  if (c == '\0')
    return 1;
  for (i = 0; s[i]; i++)
    if (s[i] == c)
      return 1;
  return 0;
}

// Irei considerar palavras tudo o que estiver entre espaços, ponto final, vírgula, ponto e vírgula ou fim da str.
int SeparaPorPalavras(char *str, char *aux)
{
  static int i = 0;
  int j = 0;

  while (str[i])
  {
    if (LocalizaDaFraseOuIndevido(str[i]) == 0)
      aux[j++] = str[i];

    i++;
    if (LocalizaDaFraseOuIndevido(str[i]) == 1)
    {
      aux[j] = '\0';
      return 1;
    }
  }

  i = 0;
  return 0;
}

int main(int argc, char **argv)
{
  char str[MAXSTR], aux[MAXSTR];
  int qtd_palindromos = 0;
  FILE *f;

  if (argc != 2)
  {
    printf("Por favor, indique apenas o nome do ficheio. Ex: \"./FindPalindromo.c file_name.txt\"\n");
    return 0;
  }

  f = fopen(argv[1], "rt");
  if (f == NULL)
  {
    printf("Arquivo de texto não encontrado\n");
    return 0;
  }

  // Caso tenha chegado até aqui é pq o ficheiro existe e foi encontrado
  // Faço agora um loop de leitura do ficheiro em busca de palíndromos.
  while (!feof(f))
  {
    fgets(str, MAXSTR, f);

    // remove o quebra linha do final da string, que só está presente nas str anteriores à última
    if (str[strlen(str) - 1] == '\n')
      str[strlen(str) - 1] = '\0';

    while (SeparaPorPalavras(str, aux))
      qtd_palindromos += FindPalindromos(aux);
  }

  if (qtd_palindromos < 1)
    printf("Não foram encontrados palíndromos no ficheiro '%s'\n", argv[1]);
  else if (qtd_palindromos == 1)
    printf("Ao todo, foi encontrado 1 palíndromo entre 3 e 10 caractere\n");
  else if (qtd_palindromos > 1)
    printf("Ao todo foram encontrados %d palíndromos entre 3 e 10 caracteres\n", qtd_palindromos);

  fclose(f);
}
