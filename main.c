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
const unsigned short int TABELA_PC1[] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2,
                                         59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39,
                                         31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37,
                                         29, 21, 13, 5, 28, 20, 12, 4};


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
 * Gera um subkey com a chave passada.
 * @param key Chave principal.
 * @param subchave Bloco de memória onde será copiada a subchave gerada.
 */
void subkey(char *key, char *subchave);

/**
 * Coloca a saída padrão a sequencia de bits que compõe o valor passado.
 * @param input Valor a ter os bits impressos.
 * @param num_caracteres Número de caracteres a terem os bits impressos
 */
void printbits(char *input, int num_caracteres);

/**
 * Divide a chave em duas partes, colocando as nos endereços recebidos como parâmetro.
 * @param key Chave a ser dividida em duas.
 * @param part1 Endereço para armazenar a primeira parte da chave.
 * @param part2 Endereço para armazenar a segunda parte da chave.
 */
void split_key(char *key, char *part1, char *part2);


int main() {
    setlocale(LC_ALL, "");
    FILE *fkey;
    FILE *fplaintext;
    FILE *fcifra;
    char nome_arquivo[NOMEARQUIVO];
    short int menu_principal;
    char key[TAMANHOBLOCO + 1];//+1 para o \0 de fim de string
    char *subchave = NULL;
    char *subchave1 = NULL, *subchave2 = NULL;


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
                fflush(stdin);
                fplaintext = fopen(strcat(nome_arquivo, ".txt"), "r");
                if (fplaintext == NULL) {
                    printf("Erro ao abrir arquivo de chave, certifique-se de que seja um .txt e digite apenas o nome\n");
                    exit(-1);
                }

                printf("\nDefina o nome do arquivo de plaintext: ");
                scanf("%s", nome_arquivo);
                fflush(stdin);
                fcifra = fopen(strcat(nome_arquivo, ".txt"), "w");
                if (fcifra == NULL) {
                    printf("Erro ao criar arquivo de plaintext\n");
                    exit(-1);
                }
                break;
            case 1:
                printf("\nArquivo de plaintext: ");
                scanf("%s", nome_arquivo);
                fflush(stdin);
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
        if (fgets(key, TAMANHOBLOCO + 1, fkey) == NULL) {
            printf("ERRO - Erro na leitura da chave\n");
            exit(-1);
        }
        validar_chave(key);
        if (trace) {
            printf("Chave lida: (%s) ", key);
            printbits(key, TAMANHOBLOCO);
            printf("\n");
        }

        if (menu_principal == 2)
            decriptacao(key);

        //gerar subkey
        if (trace)
            printf("INFO - Gerando subkey\n");
        subchave = (char *) malloc(sizeof(char) * TAMANHOBLOCO);
        subchave1 = (char *) malloc(sizeof(char) * 4);
        subchave2 = (char *) malloc(sizeof(char) * 4);
        subkey(key, subchave);
        split_key(subchave, subchave1, subchave2);
        if (trace) {
            printf("Subchave gerada: ");
            printbits(subchave, 7);
            printf("\nQuebrando a chave em duas partes");
            printf("\nParte 1: ");
            printbits(subchave1, 4);
            printf("\nParte 2: ");
            printbits(subchave2, 4);
        }

    } while (menu_principal != 0);

}

void split_key(char *key, char *part1, char *part2) {
    if (key == NULL || part1 == NULL || part2 == NULL) {
        return;
    }
    char mask1 = 15 << 4; //máscara 11110000
    char mask2 = 127 >> 3; //máscara 00001111
    char aux;

    for (int i = 0, j = 3; i < 8; i++) {
        if (i < 4) {
            //primeira parte
            part1[i] = key[i];
        } else {
            //segunda parte da chave
            part2[i-4] = key[j] & mask2;
            part2[i-4] = part2[i-4] << 4;
            if (j == 6)
                aux = ((0 & mask1) >> 4) & mask2;
            else
                aux = ((key[j + 1] & mask1) >> 4) & mask2;
            part2[i-4] = part2[i-4] | aux;
            j++;
        }
    }
    //ajustar para colocar 0s nos 4 últimos bits do ultimo byte
    part1[3] = part1[3] & mask1;

//    printf("Parte1: ");
//    printbits(part1, 4);
//    printf("\nParte2: ");
//    printbits(part2, 4);
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

void subkey(char *key, char *subchave) {
    if (key != NULL) {
        int block_count = 0; //contador do bloco da subchave
        char block_result = '\000'; //guarda o resultado do bloco que está sendo construído
        int position; //guarda qual será o caractere acessado da key para extrair o bit requirido
        char aux;
        int shift; //
        int bit_position_block = 7; //contador para a posição para por o bit requirido dentro do bloco
        char mask = 0x0001; //mascara para isolar o bit após o shift de 7 bits a direita onde é colocado 1 a frente

        for (int i = 0; i < 56; i++) {
            //descobre qual o bloco/caractere da key será acessado
            position = (int) (TABELA_PC1[i] - 1) / 8;
            //calcula a quantidade de shifts a esquerda será necessário para isolar o bit desejado
            shift = TABELA_PC1[i] - (position * 8) - 1;
            aux = (char) key[position] << shift; //faz o shift
            aux = aux >> 7; //coloca o bit desejado no bit menos significativo do byte
            aux = aux & mask;
            //até aqui isolei o bit que quero usar
            aux = aux << bit_position_block; //coloca o bit que se quer na respectiva posição do retorno
            block_result = block_result | aux; //incorpora o bit isolado ao bloco
//
//            printf("Block_result: ");
//            printbits(&block_result, 1);
//            printf("\n");

            bit_position_block--;
            if (bit_position_block < 0) {
                subchave[block_count] = block_result;
                block_count++;
                bit_position_block = 7;
                block_result = '\000';
            }
        }
    }
}

void printbits(char *input, int num_caracteres) {
    char aux = '\000';

    for (int j = 0; j < num_caracteres; j++) {
        for (int i = 8; i > 0; i--) {
            aux = input[j] << (8 - i);
            aux = aux >> 7;

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