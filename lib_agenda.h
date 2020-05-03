#ifndef LIB_AGENDA_H_INCLUDED
#define LIB_AGENDA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "lista.h"

using namespace std;

// funçoes que manipulam as funionalidades do sistema

// definindo tipo contato
struct Contato
{
    char nome[100];
    char sobrenome[100];
    char rua[100];
    char numero[100];
    char complemento[100];
    char bairro[100];
    char cidade[100];
    char estado[100];
    char fone1[100];
    char fone2[100];
};

// remove espaços em branco do inicio e do fim da string
void trim(char str[])
{
    int len = strlen(str);

    int flag = 0;
    int i, j;
    for (i = 0, j = len - 1; j >= 0; j--)
    {
        if (str[j] == ' ' && flag == 0)
        {
            str[j] = '\0';
        }
        else
        {
            flag = 1;
        }

        if (str[i] == ' ')
        {
            i++;
        }
    }

    strcpy(str, str + i);
}

// recebe a linha completa e separa a string por um caracter definido
void split(char linha[], char sep, char resultado[8][50] /*o resultado armazena 8 strings com 50 caracteres cada, separadas pelo caracter definido*/)
{
    int index = 0;

    int i;
    for (i = 0; linha[i] != '\0'; i++)
    {
        char caracter = linha[i];

        if (i == 0)
            strcpy(resultado[index], "");

        if (caracter == sep)
        {
            index++;
            strcpy(resultado[index], "");
        }
        else
        {
            char caracter_string[2];
            sprintf(caracter_string, "%c", caracter);
            strcat(resultado[index], caracter_string);// concatena string, vai acrescentand strings no final da string
        }
    }
}


// recebe string e retorna a posição do caracter ao qual o usuario esta buscando
// caso o caracter nao for encontrado, retorna-se -1
int buscarCaracter(char str[], char caracter)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
        if (str[i] == caracter)
            return i;
    return -1;
}


// recebe o resultado da função split e retorna um novo contato representado pela linha lida
Contato *gerar_contato(char resultado[8][50])
{
    Contato *contato = (Contato *)malloc(sizeof(Contato));

    trim(resultado[0]);

    int posicao_espaco = buscarCaracter(resultado[0], ' ');
    strcpy(contato->sobrenome, resultado[0] + posicao_espaco + 1);

    resultado[0][posicao_espaco] = '\0';
    strcpy(contato->nome, resultado[0]);

    trim(resultado[1]);
    strcpy(contato->rua, resultado[1]);

    trim(resultado[2]);
    strcpy(contato->numero, resultado[2]);

    trim(resultado[3]);
    strcpy(contato->complemento, resultado[3]);

    trim(resultado[4]);
    if (buscarCaracter(resultado[4], ',') == -1)
    { // Se Bairro existir
        strcpy(contato->bairro, resultado[4]);

        trim(resultado[5]);

        int posicao_virgula = buscarCaracter(resultado[5], ',');
        strcpy(contato->estado, resultado[5] + posicao_virgula + 1);
        trim(contato->estado);

        resultado[5][posicao_virgula] = '\0';
        strcpy(contato->cidade, resultado[5]);
        trim(contato->cidade);

        trim(resultado[6]);
        strcpy(contato->fone1, resultado[6]);

        if (buscarCaracter(resultado[6], '.') == -1)
        { // Se fone 2 existir
            trim(resultado[7]);
            strcpy(contato->fone2, resultado[7]);

            contato->fone2[ strlen(contato->fone2) - 1 ] = '\0';
        }
        else
        { // Se fone 2 NÃO existir
            contato->fone1[ strlen(contato->fone1) - 1 ] = '\0';
            strcpy(contato->fone2, "");
        }
    }
    else
    { // Se bairro não existir

        strcpy(contato->bairro, "");

        trim(resultado[4]);

        int posicao_virgula = buscarCaracter(resultado[4], ',');
        strcpy(contato->estado, resultado[4] + posicao_virgula + 1);

        resultado[4][posicao_virgula] = '\0';
        strcpy(contato->cidade, resultado[4]);

        trim(resultado[5]);
        strcpy(contato->fone1, resultado[5]);

        if (buscarCaracter(resultado[5], '.') == -1)
        { // Se fone2 existir
            trim(resultado[6]);
            strcpy(contato->fone2, resultado[6]);

            contato->fone2[ strlen(contato->fone2) - 1 ] = '\0';
        }
        else
        { // Se fone2 NÃO existir
            contato->fone1[ strlen(contato->fone1) - 1 ] = '\0';
            strcpy(contato->fone2, "");
        }
    }

    return contato;
}

//ela recebe arquivo a ser lido, e ler uma linha do arquivo e retorna um contato
Contato *ler_contato(FILE *file)
{
    char linha[1000];

    if (fscanf(file, "%1000[^\n]%*c", linha) != 1)
        return NULL;

    char resultado[8][50];

    split(linha, ';', resultado);

    Contato *contato = gerar_contato(resultado);

    return contato;
}

// busca pelo sobrenome
Lista *buscarPorNomeSobrenome(Lista *lista, const char *valorBuscado)
{
    Lista *resultados = cria_lista();

    Elemento *it = lista->cabeca->proximo;
    while (it != NULL)
    {
        if (strlen(valorBuscado) == 0 || strncmp(it->valor->nome, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->sobrenome, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }

        it = it->proximo;
    }

    return resultados;
}

// busca por detalhes
Lista *buscarPorDetalhes(Lista *lista, const char *valorBuscado)
{
    Lista *resultados = cria_lista();

    Elemento *it = lista->cabeca->proximo;
    while (it != NULL)
    {
        if (strlen(valorBuscado) == 0 || strncmp(it->valor->nome, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->sobrenome, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->rua, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->numero, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->complemento, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->bairro, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->cidade, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->estado, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->fone1, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }
        else if (strncmp(it->valor->fone2, valorBuscado, strlen(valorBuscado)) == 0)
        {
            adicionar(resultados, it->valor);
        }

        it = it->proximo;
    }

    return resultados;
}


// recebe o contato e salva no arquivo
void salvarContatoNoArquivo(Contato *contato, FILE *arquivo)
{
    fprintf(arquivo, "%s %s; %s; %s; %s; %s; %s, %s; ", contato->nome, contato->sobrenome, contato->rua,
            contato->numero, contato->complemento, contato->bairro, contato->cidade, contato->estado);
    if (strcmp(contato->fone2, "") == 0)
    {
        fprintf(arquivo, "%s\n", contato->fone1);
    }
    else
    {
        fprintf(arquivo, "%s; %s\n", contato->fone1, contato->fone2);
    }
}

// salva lista no arquivo
void salvarListaNoArquivo(Lista *lista, FILE *arquivo)
{
    Elemento *it = lista->cabeca->proximo;
    while (it != NULL)
    {
        salvarContatoNoArquivo(it->valor, arquivo);
        it = it->proximo;
    }
}

// tem haver com a ordenação
void troca(Elemento *a, Elemento *b)
{
    if (a == NULL || b == NULL)
        return;
    Contato *valor_a = a->valor;
    a->valor = b->valor;
    b->valor = valor_a;
}

// ordenando
void ordena(Lista *lista)
{
    for (int count = 0; count < lista->tamanho; count++)
    {
        for (Elemento *it = lista->cabeca->proximo; it != NULL && it->proximo != NULL; it = it->proximo)
        {
            int cmpNome = strcmp(it->valor->nome, it->proximo->valor->nome);
            int cmpSobrenome = strcmp(it->valor->sobrenome, it->proximo->valor->sobrenome);
            if (cmpNome > 0 || (cmpNome == 0 && cmpSobrenome > 0))
            {
                troca(it, it->proximo);
            }
        }
    }
}

// mostra contato no menu
void mostrarContato(Contato *contato)
{
    printf("Nome: %s\n", contato->nome);
    printf("Sobrenome: %s\n", contato->sobrenome);
    printf("Rua: %s\n", contato->rua);
    printf("Numero: %s\n", contato->numero);
    printf("Bairro: %s\n", contato->bairro);
    printf("Complemento: %s\n", contato->complemento);
    printf("Cidade: %s\n", contato->cidade);
    printf("Estado: %s\n", contato->estado);
    printf("Telefone1: %s\n", contato->fone1);
    printf("Telefone2: %s\n\n", contato->fone2);
}

// alterando contato
void alterarContato(Contato *contato)
{
    char valor[100];

    printf("Nome (%s): ", contato->nome);
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "") != 0)
        strcpy(contato->nome, valor);

    printf("\nSobrenome (%s): ", contato->sobrenome);
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "") != 0)
        strcpy(contato->sobrenome, valor);

    printf("\nRua (%s): ", contato->rua);
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "") != 0)
        strcpy(contato->rua, valor);

    printf("\nNumero (%s): ", contato->numero);
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "") != 0)
        strcpy(contato->numero, valor);

    printf("\nBairro (%s): ", contato->bairro);
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "") != 0)
        strcpy(contato->bairro, valor);

    printf("\nComplemento (%s): ", contato->complemento);
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "") != 0)
        strcpy(contato->complemento, valor);

    printf("\nCidade (%s): ", contato->cidade);
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "") != 0)
        strcpy(contato->cidade, valor);

    printf("\nEstado (%s): ", contato->estado);
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "") != 0)
        strcpy(contato->estado, valor);

    printf("\nTelefone 1 (%s): ", contato->fone1);
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "") != 0)
        strcpy(contato->fone1, valor);

    printf("\nTelefone 2 (%s) ", contato->fone2);
    printf("[Digite 0 (zero) para apagar o fone2]: ");
    fgets(valor, 100, stdin);
    valor[strlen(valor) - 1] = '\0';

    if (strcmp(valor, "0") == 0)
        strcpy(contato->fone2, "");
    else if (strcmp(valor, "") != 0)
        strcpy(contato->fone2, valor);
}

// adicionar contato exibindo formulario para usuario
Contato *obterContatoDeUsuario()
{
    Contato *contato = (Contato *)malloc(sizeof(Contato));

    printf("Nome: ");
    scanf(" %[^\n]%*c", contato->nome);

    printf("\nSobrenome: ");
    scanf(" %[^\n]%*c", contato->sobrenome);

    printf("\nRua: ");
    scanf(" %[^\n]%*c", contato->rua);

    printf("\nNumero: ");
    scanf(" %[^\n]%*c", contato->numero);

    printf("\nBairro: ");
    scanf(" %[^\n]%*c", contato->bairro);

    printf("\nComplemento: ");
    scanf(" %[^\n]%*c", contato->complemento);

    printf("\nCidade: ");
    scanf(" %[^\n]%*c", contato->cidade);

    printf("\nEstado: ");
    scanf(" %[^\n]%*c", contato->estado);

    printf("\nTelefone 1: ");
    scanf(" %[^\n]%*c", contato->fone1);

    printf("\nTelefone 2: ");
    fgets(contato->fone2, 100, stdin);
    contato->fone2[strlen(contato->fone2) - 1] = '\0';

    return contato;
}

#endif // LIB_AGENDA_H_INCLUDED
