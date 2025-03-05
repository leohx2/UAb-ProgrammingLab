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
#include <stdlib.h>
#include <ctype.h>

#define MAXSTR 255
#define MAXPALINDROMO 20

/*  Escolho Linked lists para armazenar os resultados encontrados de modo a ter controle sobre os mesmos
	para caso seja necessário uma utilização futura, desta forma é também mais rápido do que um vetor de strings.
*/
// Estrutura de dados:
typedef struct TPalindromos
{
	char *pali;
	struct TPalindromos *next;
} SPalindromos;

SPalindromos *AdicionaNovo(SPalindromos *list, int str_size, char *s)
{
	SPalindromos *novo;

	novo = (SPalindromos *)malloc(sizeof(SPalindromos));
	if (novo != NULL)
	{
		novo->pali = (char *)malloc(sizeof(char) * str_size);
		if (novo->pali == NULL)
			return list;

		novo->pali = strcpy(novo->pali, s);
		novo->next = list;
		return novo;
	}

	return list;
}

// Evita duplicados
SPalindromos *AdicionaEmOrdem(SPalindromos *list, int str_size, char *s)
{
	SPalindromos *aux;
	// Caso seja o primeiro, não faz a verificação, apenas adiciona
	if (list == NULL)
		list = AdicionaNovo(list, str_size, s);
	else
	{
		aux = list;
		if (strcmp(s, aux->pali) == 0)
			return list;

		while (aux && aux->next)
		{
			// Se a string s já estiver presente na lista, n adiciona.
			if (strcmp(s, aux->next->pali) == 0)
				return list;
			aux = aux->next;
		}
		// adiciona ao fim da lista a nova str.
		aux->next = AdicionaNovo(aux->next, str_size, s);
	}
	return list;
}

int TamanhoLista(SPalindromos *list)
{
	int tamanho = 0;
	SPalindromos *aux = list;

	while (aux)
	{
		aux = aux->next;
		tamanho++;
	}

	return tamanho;
}

SPalindromos *FreePalindromos(SPalindromos *list)
{
	SPalindromos *aux;

	if (list)
	{
		aux = list->next;
		if (list->pali)
			free(list->pali);
		free(list);
		list = aux;
	}
	return list;
}

void MostrarPalindromos(SPalindromos *list)
{
	int i = 1;
	while (list)
	{
		printf("%3.d: %s\n", i, list->pali);
		list = list->next;
		i++;
	}
}

int IsAPalindromo(char *s, int size)
{
	int i;

	// Optei por não ser key sensitive
	for (i = 0; i < size / 2; i++)
		if (!isalnum(s[i]) || tolower(s[i]) != tolower(s[size - i - 1]))
			return 0;

	return 1;
}

SPalindromos *FindPalindromos(SPalindromos *s_pali, char *s)
{
	// guarda_pali irá armazenar o palindromo que será adicionado à lista.
	int str_size;

	str_size = strlen(s);

	if (str_size < 3 || str_size > 10)
		return s_pali;

	if (IsAPalindromo(s, str_size))
		s_pali = AdicionaEmOrdem(s_pali, str_size, s);
	return s_pali;
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
	SPalindromos *palindromos;

	palindromos = NULL;
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
			palindromos = FindPalindromos(palindromos, aux);
	}

	qtd_palindromos = TamanhoLista(palindromos);
	if (qtd_palindromos < 1)
		printf("Não foram encontrados palíndromos no ficheiro '%s'\n", argv[1]);
	else if (qtd_palindromos == 1)
		printf("Foi encontrado 1 palíndromo entre 3 e 10 caracteres, que será listado abaixo\n");
	else if (qtd_palindromos > 1)
		printf("Foram encontrados %d palíndromos entre 3 e 10 caracteres, que serão listados abaixo\n", qtd_palindromos);

	MostrarPalindromos(palindromos);

	fclose(f);
	while (palindromos)
		palindromos = FreePalindromos(palindromos);
}
