//
// Created by Daniel de Vargas on 01/09/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#define NOMEARQUIVO 255
#define TAMANHOBLOCO 8 //em bytes

//GLOBAL VARIABLES
short int trace;
const unsigned short int TABELA_PC1[] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35,
                                         27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38,
                                         30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};


//FUNTIONS

/**
 * Faz a inversão da chave para que ela seja lida de traz para frente quando feita a decriptação
 * @param key chave a ser invertida
 */
void decriptacao(char *key);

/**
 * Faz a validação da chave adicionando caracteres caso faltem
 * @param key Chave a ser validada.
 */
void validar_chave(char *key);

/**
 * Gera um subchave com a chave passada.
 * @param key Chave principal.
 * @return A subchave gerada.
 */
char *subchave(char *key);

/**
 * Coloca a saída padrão a sequencia de bits que compõe o valor passado.
 * @param input Valor a ter os bits impressos.
 * @param size Tamanho dos dados a terem os bits impressos
 */
void printbits(char *input, int size);


int main() {
    setlocale(LC_ALL, "");
    FILE *fkey;
    FILE *fplaintext;
    FILE *fcifra;
    char nome_arquivo[NOMEARQUIVO];
    short int menu_principal;
    char key[TAMANHOBLOCO+1];//+1 para o \0 de fim de string
    char *subkey = NULL;


    printf("\n########## Cifras de Feistel ##########\n\n");

    //requisição para mostrar ou não o trace de execução
    printf("Deseja mostrar os detalhes de execução? 1-Sim 0-Não\n");
    scanf("%hu", &trace);
    if (trace != 0) {
        trace = 1;
    } else {
        trace = 0;
    }

    do {
        printf("\n1-Encriptação  2-Decriptação  0-Sair\nEscolha: ");
        scanf("%hu", &menu_principal);
        switch (menu_principal) {
            case 2:
                printf("\nArquivo de cifra: ");
                scanf("%s", nome_arquivo);
                fplaintext = fopen(strcat(nome_arquivo, ".txt"), "r");
                if (fplaintext == NULL) {
                    printf("Erro ao abrir arquivo de chave, certifique-se de que seja um .txt e digite apenas o nome\n");
                    exit(-1);
                }

                printf("\nDefina o nome do arquivo de plaintext: ");
                scanf("%s", nome_arquivo);
                fcifra = fopen(strcat(nome_arquivo, ".txt"), "w");
                if (fcifra == NULL) {
                    printf("Erro ao criar arquivo de plaintext\n");
                    exit(-1);
                }
                break;
            case 1:
                printf("\nArquivo de plaintext: ");
                scanf("%s", nome_arquivo);
                fplaintext = fopen(strcat(nome_arquivo, ".txt"), "r");
                if (fplaintext == NULL) {
                    printf("Erro ao abrir arquivo de plaintext, certifique-se de que seja um .txt e digite apenas o nome\n");
                    exit(-1);
                }

                printf("Defina o nome do arquivo de cifra: ");
                scanf("%s", nome_arquivo);
                fcifra = fopen(strcat(nome_arquivo, ".txt"), "w");
                if (fcifra == NULL) {
                    printf("Erro ao criar arquivo de cifra\n");
                    exit(-1);
                }
                break;
            case 0:
                printf("\nTchau\n");
                exit(0);
            default:
                printf("Opção inválida\n");
        }


        printf("Arquivo de chave: ");
        scanf("%s", nome_arquivo);
        fkey = fopen(strcat(nome_arquivo, ".txt"), "r");
        if (fkey == NULL) {
            printf("Erro ao abrir arquivo de chave, certifique-se de que seja um .txt e digite apenas o nome");
            exit(-1);
        }

        //LEITURA E VALIDAÇÕES DA CHAVE
        if (trace)
            printf("INFO - Lendo e validando chave\n");
        if (fgets(key, TAMANHOBLOCO+1, fkey) == NULL) {
            printf("ERRO - Erro na leitura da chave\n");
            exit(-1);
        }
        validar_chave(key);
        if (trace) {
            printf("Chave lida: (%s) ", key);
            printbits(&key, TAMANHOBLOCO);
            printf("\n");
        }

        if (menu_principal == 2)
            decriptacao(key);

        //gerar subchave
        if (trace)
            printf("INFO - Gerando subchave\n");
        subkey = (char) malloc(sizeof(char)*TAMANHOBLOCO);
        strcpy(subkey, subchave(key));
        if (trace) {
            printf("Subchave gerada: ");
            printbits(&subkey, TAMANHOBLOCO);
        }
    } while (menu_principal != 0);

}

void validar_chave(char *key) {
    int tamanho_leitura = TAMANHOBLOCO + 1;
    for (int i = 0; i < TAMANHOBLOCO; i++) {
        if (key[i] == '\0') //pegar a primeira referencia de \0 para saber até onde leu
            tamanho_leitura = i;

        if (i > tamanho_leitura) //da leitura em diante preencher com 0s
            key[i] = '\0';
    }
}

char *subchave(char *key) {
    if (key == NULL) {
        return NULL;
    }
    char *retorno = NULL;
    char *aux = NULL;
    retorno = (char *) realloc(retorno, TAMANHOBLOCO);
    aux = (char *) realloc(aux, sizeof(char) * TAMANHOBLOCO);

    for (int i = 0; i < 56; i++) {
        *aux = *key << (TABELA_PC1[i] - 1);//fazer o shift para o bit que se quer que fique no bit mais significativo
        *aux = *aux >> ((TAMANHOBLOCO * 8) - 1); //coloca o bit que se quer no bit menos significativo
        *aux = *aux << ((TAMANHOBLOCO * 8) - 1 - i); //coloca o bit que se quer na respectiva posição do retorno
        *retorno = *aux | *retorno; //concatenando o bit que se quer com os resultados anteriores
    }

//    retorno = realloc(retorno, TAMANHOBLOCO + 1);
//    retorno[TAMANHOBLOCO + 1] = '\0';
    return retorno;
}

void printbits(char *input, int size) {
    char aux = '\000';

    for (int j = 0; j < size; j++) {
        for (int i = size; i > 0; i--) {
            aux = input[j] << (size-i);
            aux = aux >> (size - 1);

            if (aux) {
                printf("1");
            } else {
                printf("0");
            }
        }
        printf(" ");
    }
}

void decriptacao(char *key) {
    //TODO: implementar função de reversão de bits da chave
}