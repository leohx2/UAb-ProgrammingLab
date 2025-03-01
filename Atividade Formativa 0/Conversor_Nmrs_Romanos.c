/*
############################################################################################################
Enúnciado:
Conversor de numerais romanos.

Escreva um programa para converter um número em numeração romana para a notação decimal.
Ex. MCMLXXIX = 1979.
Implemente um adequado tratamento de erros, que informe o utilizador, de forma clara, sobre o erro que
cometeu na escrita do numeral romano (ex. usou uma letra não permitida como "P").

Informações úteis:
Nesse sistema, são utilizadas as letras I, X, L, C, D e M para formar os números.
I = 1
V = 5
X = 10
L = 50
C = 100
D = 500
M = 1000

IV = 4
IX = 9
XL = 40
XC = 90
CD = 400
CM = 900
M = 1000
############################################################################################################
*/
#include <stdio.h>
#include <ctype.h>
#define MAXSTR 255

int Contem_o_char(char c, char *str)
{
  int i;

  for (i = 0; str[i]; i++)
    if (str[i] == c)
      return 1;
  return 0;
}

// Retorna o valor do número romano a ser analisado ou 0 se for inválido
int Verifica_romano(char *romano, int *index_atual)
{
  switch (romano[*index_atual])
  {
  case 'I':
  {
    if (romano[*index_atual + 1] == 'V')
    {
      *index_atual += 1;
      return 4;
    }
    else if (romano[*index_atual + 1] == 'X')
    {
      *index_atual += 1;
      return 9;
    }
    else if (Contem_o_char(romano[*index_atual + 1], "LCDM"))
      return -1;
    else
      return 1;
  }
  case 'V':
  {
    if (Contem_o_char(romano[*index_atual + 1], "XLCDM"))
      return -1;
    return 5;
  }
  case 'X':
  {
    if (romano[*index_atual + 1] == 'L')
    {
      *index_atual += 1;
      return 40;
    }
    else if (romano[*index_atual + 1] == 'C')
    {
      *index_atual += 1;
      return 90;
    }
    else if (Contem_o_char(romano[*index_atual + 1], "DM"))
      return -1;
    else
      return 10;
  }
  case 'L':
  {
    if (Contem_o_char(romano[*index_atual + 1], "CDM"))
      return -1;
    return 50;
  }
  case 'C':
  {
    if (romano[*index_atual + 1] == 'D')
    {
      *index_atual += 1;
      return 400;
    }
    else if (romano[*index_atual + 1] == 'M')
    {
      *index_atual += 1;
      return 900;
    }
    else
      return 100;
  }
  case 'D':
  {
    if (Contem_o_char(romano[*index_atual + 1], "M"))
      return -1;
    return 500;
  }
  case 'M':
    return 1000;
  default:
    return 0;
  }
}

void Torna_maiusculo(char *str)
{
  int i;

  for (i = 0; str[i]; i++)
    str[i] = toupper(str[i]);
}

// Verifica se há uma sequência inválida, como por exemplo IIII ou VV, XXXX, etc.
int Sequencia_invalida(char *str)
{
  int i, iguais = 1;
  for (i = 1; str[i]; i++)
  {
    if (str[i] == str[i - 1])
      iguais++;
    else
      iguais = 1;

    if (str[i] == 'I' && iguais >= 4 || str[i] == 'V' && iguais >= 2 || str[i] == 'X' && iguais >= 4 ||
        str[i] == 'L' && iguais >= 2 || str[i] == 'C' && iguais >= 4 || str[i] == 'D' && iguais >= 2 ||
        str[i] == 'M' && iguais >= 4)
      return str[i];
  }

  return 0;
}

int main()
{
  char nmro_romano[MAXSTR];
  int i, decimal = 0, aux;

  printf("Insira o número romano: ");
  scanf(" %s", nmro_romano);

  // Torna os números dados em maiúsculo
  Torna_maiusculo(nmro_romano);

  // Verifica se há uma sequência inválida
  aux = Sequencia_invalida(nmro_romano);
  if (aux > 0)
  {
    printf("Sequência inválida, há pelo menos uma repetição inválida de caracteres \" %c \"\n", aux);
    return 0;
  }

  for (i = 0; nmro_romano[i]; i++)
  {
    // Aux recebe 0 se for um nmro inválido ou o valor em decimal relativo ao número romano a ser analisado
    aux = Verifica_romano(nmro_romano, &i);

    if (aux == 0)
    {
      printf("Caracter \"%c\" inválido, por favor insira apenas caracteres romanos\nSão eles: I, X, L, C, D e M\n", nmro_romano[i]);
      return 0;
    }
    else if (aux == -1)
    {
      printf("Há pelo menos uma subtração inválida detectada, sendo a \" %c%c \"", nmro_romano[i], nmro_romano[i + 1]);
      return 0;
    }
    decimal += aux;
  }
  printf("Número decimal, após a conversão: %d\n", decimal);
}