//
// Created by Daniel de Vargas on 01/09/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

#define NOMEARQUIVO 255
#define TAMANHOBLOCO 8 //em bytes
#define NUM_RODADAS 16

//GLOBAL VARIABLES
short int trace;
const unsigned short int TABELA_PC1[] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2,
                                         59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39,
                                         31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37,
                                         29, 21, 13, 5, 28, 20, 12, 4};

const unsigned short int TABELA_IP[] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
                                        62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
                                        57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
                                        61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};

const unsigned short int LEFT_SHIFT[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

const unsigned short int TABELA_PC2[] = {14, 17, 11, 24, 1, 5, 3, 28,  15, 6, 21, 10, 23, 19, 12, 4,
                                         26, 8, 16, 7, 27, 20, 13, 2,  41, 52, 31, 37, 47, 55, 30, 40,
                                         51, 45, 33, 48, 44, 49, 39, 56,  34, 53, 46, 42, 50, 36, 29, 32};


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

/**
 * Faz a leitura e validação de caracteres do arquivo, colocando os caracteres válidos no buffer.
 * @param file Arquivo que se quer ler.
 * @param buffer Variável para armazenamento dos caracteres válidos.
 * @param buffer_size Tamanho, em caracteres, do buffer.
 * @return True se foram lidos caracteres do arquivo ou false se chegou ao fim do arquivo.
 */
bool read_file(FILE *file, char *buffer, int buffer_size);

/**
 * Faz a permutação dos bits do bloco passado, de acordo com a tabela IP.
 * @param bloco endereço do bloco lido.
 * @param block_size Numero de caracteres do bloco.
 * @return O endereço do bloco permutado.
 */
char *PBox(char *bloco, int block_size);

/**
 * Gera a subchave de rodada. Realizando a junção, shifts circulares e permutação da chave.
 * @param subkey_part1 Parte 1 da chave.
 * @param subkey_part2 Parte 2 da chave.
 * @param key_round Onde a chave gerada será armazenada.
 * @param round Número da rodada que será gerada a subchave.
 * @return Endereço da subchave gerada.
 */
char *round_key(char *subkey_part1, char *subkey_part2, int round);

/**
 * Realiza a união dos bits das partes da subkey.
 * @param part1 Endereço do vetor com a parte 1 da subkey.
 * @param part2 Endereço do vetor com a parte 2 da subkey.
 * @param subkey Endereço para salvar a subkey unida.
 */
void union_subkey(char *part1, char *part2, char *subkey);


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
    char *roundkey;
    char plaintext[TAMANHOBLOCO]; //armazena um fragmento de texto a ser encriptado
    char *permuted_plaintext;
    char LN[2], RN[2];


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
            printf("Quebrando a chave em duas partes\n");
            printf("Parte 1: ");
            printbits(subchave1, 4);
            printf("Parte 2: ");
            printbits(subchave2, 4);
        }

        //LER ARQUIVO A SER ENCRIPTADO
        while (read_file(fplaintext, plaintext, TAMANHOBLOCO)) {
            //TODO: encriptar
            permuted_plaintext = PBox(plaintext, TAMANHOBLOCO);
            if (trace) {
                printf("Plaintext: (%s) ", plaintext);
                printbits(plaintext, 8);
                printf("PermutedPlaintext: ");
                printbits(permuted_plaintext, 8);
            }
            //quebrar o bloco em 2
            LN[0] = permuted_plaintext[0];
            LN[1] = permuted_plaintext[1];
            RN[0] = permuted_plaintext[2];
            RN[1] = permuted_plaintext[3];


            for (int i = 0; i < NUM_RODADAS; i++) {
                //gerar subchave de rodada
                roundkey = round_key(subchave1, subchave2, i);
                printf("Subchave recebida no main: ");
                printbits(roundkey, 6);

                free(roundkey);
            }

        }
    } while (menu_principal != 0);

}

char *round_key(char *subkey_part1, char *subkey_part2, int round) {
    if (subkey_part1 == NULL || subkey_part2 == NULL || round < 0 || round > NUM_RODADAS) {
        return NULL;
    }
    char key_aux[7];
    char *key_round = (char*) malloc(sizeof(char) * 6);
    int position;//descobre qual o caractere do bloco será acessado
    int shift; //calcula a quantidade de shifts a esquerda será necessário para isolar o bit desejado
    char aux;
    char block_result = '\000'; //guarda o resultado do byte que está sendo construído
    int bit_position_block = 7; //contador para a posição para por o bit requirido dentro do bloco
    int byte_count = 0;
    char mask; //máscara para isolar os bits que seriam perdidos com o shift à esquerda
    char mask_d;//mascara para tirar as replicações de 1s quando shift à direita
    char ultimo; //guarda os bits iniciais perdidos após o shift à esquerda

    mask = LEFT_SHIFT[round] == 2 ? 0b11000000 : 0b10000000; //formar a máscara
    mask_d = LEFT_SHIFT[round] == 2 ? 0b00000011 : 0b00000001; //formar a máscara
    shift = LEFT_SHIFT[round] == 2 ? 6 : 7;

    //shift circular na parte 1
    for (int i = 0; i < 4; ++i) {
        if (i == 0)
            ultimo = subkey_part1[0] & mask;
        if (i != 3) {
            aux = ((subkey_part1[i + 1] & mask) >> shift) & mask_d;
        } else {
            aux = (ultimo >> (shift - 4)) & mask_d;
        }
        subkey_part1[i] = subkey_part1[i] << LEFT_SHIFT[round];
        subkey_part1[i] = subkey_part1[i] | aux;
    }
    //shift circular na parte 2
    for (int i = 0; i < 4; ++i) {
        if (i == 0)
            ultimo = subkey_part2[0] & mask;
        if (i != 3) {
            aux = ((subkey_part2[i + 1] & mask) >> shift) & mask_d;
        } else {
            aux = (ultimo >> (shift - 4)) & mask_d;
        }
        subkey_part2[i] = subkey_part2[i] << LEFT_SHIFT[round];
        subkey_part2[i] = subkey_part2[i] | aux;
    }

    //unir as duas partes
    union_subkey(subkey_part1, subkey_part2, key_aux);
    //fazer permutação com tabela PC 2
    mask = 0b00000001;
    for (int i = 0; i < 56; i++) {
        position = (int) (TABELA_PC2[i] - 1) / 8;
        //calcula a quantidade de shifts a esquersa será necessário para isolar o bit desejado
        shift = TABELA_PC2[i] - (position * 8) - 1;
        aux = (char) key_aux[position] << shift;
        aux >>= 7;
        aux = aux & mask;
        //até aqui isolei o bit que quero usar
        aux = aux << bit_position_block;
        block_result = block_result | aux;

        bit_position_block--;
        if (bit_position_block < 0) {
            key_round[byte_count] = block_result;
            byte_count++;
            bit_position_block = 7;
            block_result = '\000';
        }
    }
    if (trace) {
        printf("\n\nCircular shift nas partes da subchave (rodada %d)\n", round);
        printf("Parte 1 da subchave após o shift circular (ignorar últimos 4 bits): ");
        printbits(subkey_part1, 4);
        printf("Parte 2 da subchave após o shift circular (ignorar os últimos 4 bits): ");
        printbits(subkey_part2, 4);
        printf("Subchave de rodada (%d): ", round);
        printbits(key_round, 6);
    }
    return key_round;
}

void union_subkey(char *part1, char *part2, char *subkey) {
    if (part1 != NULL || part2 != NULL || subkey != NULL) {
        char mask = 0b11110000;
        char mask2 = 0b00001111;

        subkey[0] = part1[0];
        subkey[1] = part1[1];
        subkey[2] = part1[2];
        subkey[3] = part1[3] | ((part2[0] >> 4) & mask2);
        subkey[4] = (((part2[1] & mask) >> 4) & mask2) | (part2[0] << 4);
        subkey[5] = (((part2[2] & mask) >> 4) & mask2) | (part2[1] << 4);
        subkey[6] = ((part2[3] >> 4) & mask2) | (part2[2] << 4);
    }
}

char *PBox(char *bloco, int block_size) {
    if (bloco == NULL || block_size < 0) {
        return NULL;
    }

    char *bloco_permutado = malloc(sizeof(char) * block_size);
    int position;//descobre qual o caractere do bloco será acessado
    int shift; //calcula a quantidade de shifts a esquerda será necessário para isolar o bit desejado
    char aux;
    char block_result = '\000'; //guarda o resultado do byte que está sendo construído
    char mask = 0x0001; //máscara para isolar o bit menos significativo do byte, pois após shift a direita, se tiver 1 ele é replicado
    int bit_position_block = 7; //contador para a posição para por o bit requirido dentro do bloco
    int byte_count = 0;

    for (int i = 0; i < 64; i++) {
        position = (int) (TABELA_IP[i] - 1) / 8;
        shift = TABELA_IP[i] - (position * 8) - 1;
        aux = bloco[position] << shift;
        aux = aux >> 7; //coloca o bit desejado na posição menos significativa do byte
        aux = aux & mask;

        //ate aqui, isolei o bit que quero usar
        aux = aux << bit_position_block;
        block_result = block_result | aux;

//        printf("Block_result: ");
//        printbits(&block_result, 1);

        bit_position_block--;
        if (bit_position_block < 0) {
            bloco_permutado[byte_count] = block_result;
            byte_count++;
            bit_position_block = 7;
            block_result = '\000';
        }
    }
    return bloco_permutado;
}

bool read_file(FILE *file, char *buffer, int buffer_size) {
    if (file == NULL || buffer == NULL || buffer_size < 1) {
        return false;
    }

    if (!feof(file)) {
        char c;

        for (int i = 0; i < buffer_size; ++i) {
            c = fgetc(file);
            if (c > 0) {
                buffer[i] = c;
            } else {
                buffer[i] = NULL;
            }
        }
        return true;
    } else {
        return false;
    }
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
            part2[i - 4] = key[j] & mask2;
            part2[i - 4] = part2[i - 4] << 4;
            if (j == 6)
                aux = ((0 & mask1) >> 4) & mask2;
            else
                aux = ((key[j + 1] & mask1) >> 4) & mask2;
            part2[i - 4] = part2[i - 4] | aux;
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
    printf("\n");
}

void decriptacao(char *key) {
    //TODO: implementar função de reversão de bits da chave
}