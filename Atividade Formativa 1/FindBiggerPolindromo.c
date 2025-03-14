/*
##############################################################################################################################

Considere uma versão 2 do código FindPolindromo_sem_guardar.c com o seguinte enunciado:

Enunciado:
Tendo em conta que a noção de "palíndromo" pode não ser restrita a palavras, modifique o seu programa para detetar palíndromos em sentido mais lato:

Considere  que os palíndromos extravasam os limites das palavras e não têm em conta espaços, sinais de pontuação ou acentos, como o já referido "Roma me tem amor",
mas também "após a sopa", "Irene ri", "Socorram-me, subi no ônibus em Marrocos", "A inter etnia". Repare que por vezes existem palíndromos dentro de palíndromos (ex. "me tem"),
mas só se pretende obter os mais abrangentes.

No output do seu programa, deverá apresentar a expressão como ela surge no texto original, isto é "Socorram-me, subi no ônibus em Marrocos" e não
"socorrammesubinoonibusemmarrocos".

##############################################################################################################################
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXSTR 255

int IsAPalindromo(char *s, int size)
{
  int i, j = size;

  if (size < 2 || !isalnum(s[j]))
    return 0;

  // Optei por não ser key sensitive
  // Mudança na lógica, agora temos de ignorar espaços, logo a comparação será dada pela posição j
  // j decrementa quando encontra um char que é para ignorar, mas i mantém-se igual.
  for (i = 0; i < size / 2; i++, j--)
  {
    /* Em todo o ciclo j irá decrementar e i incrementar, se o char s[j] é para ignorar, por exemplo, se for um "-", então
    i decrementa 1 unidade, que irá permitir que ele fique na mesma posição no próximo ciclo, já que iremos tirar uma unidade
    e dps acrescer uma unidade, ficando igual. O mesmo acontece com j caso o char s[i] seja para ignorar.
    */
    if (!isalnum(s[j]))
      i--;
    else if (!isalnum(s[i]))
      j++;
    else if (!isalnum(s[i]) || tolower(s[i]) != tolower(s[j]) || i >= j)
      return 0;
  }

  return 1;
}

void PrintfComLimite(char *s, int inicio, int final)
{
  int j;

  for (j = inicio; j <= inicio + final; j++)
    printf("%c", s[j]);
  printf("\n");
}

// Não há mais a necessidade de controlar o tamanho da str, porém preciso encontrar o maior palíndromo.
int FindPalindromos(char *s)
{
  int str_size, i = 0, counter = 0;

  str_size = strlen(s);

  // Ve todos os chars da str que permitem formar um palíndromo, se necessário, parando em um ponto que permita encontrar um palíndromo
  // de tamanho mínimo de 3. Por isso vai té s[i + 2].
  while (s[i + 2])
  {
    // Se for para ignorar, é importante atualizar o tamanho da str e a posição do index
    if (!isalnum(s[i]))
      i++;
    else
    {
      if (IsAPalindromo(s + i, str_size - i))
      {
        // Aqui sabemos que reconheceu um palíndromo, mas preciso ver se é uma palavra, frase ou substring, já que não quero substring.
        // E para tal, vejo o caracater anterior ao primeiro e o seguinte ao último caracter daquilo que irei imprimir, se forem indicativos
        // de final de frase ou fim da str, imprime.
        if (!isalnum(s[i - 1]) && !isalnum(s[str_size + 1]))
        {
          // Esse if ao como último parametro decido se deve imprimir ou não o último caracter da str.
          PrintfComLimite(s, i, (s[str_size + 1] == '\n' ? str_size - i : str_size - i + 1));
          counter++;
        }
        // Atualiza a string s, para que seja possível verificar palíndromos a frente daquele que encontrou na mesma linha
        s += str_size + 1;

        while (*s && !isalnum(*s))
          s++;
        // Atualiza o tamanho da str e o contador.
        str_size = strlen(s);
        i = 0;
      }
      else
        str_size--;

      // Após ver char por char, se é um palíndromo, volta ao início porém com o char seguinte.
      if (str_size - i < 2)
      {
        str_size = strlen(s) - 1;
        i++;
      }
    }
  }

  return counter;
}

int main(int argc, char **argv)
{
  char str[MAXSTR], *str_total;
  int qtd_palindromos = 0;
  long int f_size;
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

  // Recebe o tamanho total do ficheiro para que consiga alocar, de maneira dinâmica, o tamanho total da string necessária
  // para analisar todo o ficheiro de uma só vez, para saber se estou a lidar com um única palíndromo.
  fseek(f, 0, SEEK_END);
  f_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  str_total = (char *)malloc(sizeof(char) * f_size);
  if (str_total == NULL)
  {
    printf("Erro ao alocar memória\n");
    return 0;
  }

  // Caso tenha chegado até aqui é pq o ficheiro existe e foi encontrado
  // Faço agora um loop de leitura do ficheiro em busca de palíndromos.
  printf("----------------------Resultado----------------------\n");
  while (!feof(f))
  {
    fgets(str, MAXSTR, f);
    strcat(str_total, str);
  }

  qtd_palindromos += FindPalindromos(str_total);
  printf("-----------------------------------------------------\n");

  if (qtd_palindromos < 1)
    printf("Não foram encontrados palíndromos no ficheiro '%s'\n", argv[1]);
  else if (qtd_palindromos == 1)
    printf("Ao todo, foi encontrado 1 palíndromo\n");
  else if (qtd_palindromos > 1)
    printf("Ao todo foram encontrados %d palíndromos\n", qtd_palindromos);

  free(str_total);
  fclose(f);
}
