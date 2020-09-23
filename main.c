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

const unsigned short int TABELA_PC2[] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
                                         26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
                                         51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

const unsigned short int EBITSELECTION[] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11,
                                            12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
                                            22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};

const unsigned short int SBOX1[4][16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                                         0, 15, 7, 3, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                                         4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 13, 10, 5, 0,
                                         15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13};

const unsigned short int SBOX2[4][16] = {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                                         3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                                         0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                                         13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9};

const unsigned short int SBOX3[4][16] = {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                                         13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                                         13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                                         1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12};

const unsigned short int SBOX4[4][16] = {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                                         13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                                         10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                                         3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14};

const unsigned short int SBOX5[4][16] = {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                                         14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                                         4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                                         11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3};

const unsigned short int SBOX6[4][16] = {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                                         10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                                         9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                                         4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13};

const unsigned short int SBOX7[4][16] = {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                                         13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                                         1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                                         6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12};

const unsigned short int SBOX8[4][16] = {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                                         1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                                         7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                                         2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};

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

char *funcao_feistel(char *RRodada, char *round_key);

char *SBOXES(char *expansao);

void PBox_feistel(char *resultado);


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
    char LN[4]; //parte da esqerda do bloco a ser encriptado
    char RN[4]; //parte da direita do bloco a ser encriptado
    char LRodada[4]; //parte da esquerda do bloco sendo encriptado, sendo usado no momento para encriptação nas rodadas
    char RRodada[4]; //parte da direita do bloco sendo encriptado, sendo usado no momento para encriptação nas rodadas
    char LNPRodada[4]; //guarda os valores calculados a serem usados na próxima rodada para a parte esquerda do bloco sendo encriptado
    char RNPRodada[4]; //guarda os valores calculados a serem usados na próxima rodada para a parte direita do bloco sendo encriptado
    char *resultado_f; //guarda o resultado da função F


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
            LN[3] = permuted_plaintext[2];
            LN[4] = permuted_plaintext[3];
            RN[0] = permuted_plaintext[4];
            RN[1] = permuted_plaintext[5];
            RN[2] = permuted_plaintext[6];
            RN[3] = permuted_plaintext[7];


            for (int i = 0; i < NUM_RODADAS; i++) {
                //gerar subchave de rodada
                roundkey = round_key(subchave1, subchave2, i);

                if (i == 0) {
                    for (int j = 0; j < 4; ++j) {
                        LRodada[j] = LN[j];
                        RRodada[j] = RN[j];
                    }
                } else {
                    for (int j = 0; j < 4; ++j) {
                        LRodada[j] = LNPRodada[j];
                        RRodada[j] = RNPRodada[j];
                    }
                }
                LNPRodada[0] = RRodada[0];
                LNPRodada[1] = RRodada[1];
                resultado_f = funcao_feistel(RRodada, roundkey);
//                RNPRodada = resltado_f ^ LRodada;
                free(roundkey);
            }

        }
    } while (menu_principal != 0);

}

char *funcao_feistel(char *RRodada, char *round_key) {
    if (RRodada == NULL || round_key == NULL) {
        return NULL;
    }

    char *resultado;
    char *expansao;
    int block_count = 0; //contador do bloco da subchave
    char block_result = '\000'; //guarda o resultado do bloco que está sendo construído
    int position; //guarda qual será o caractere acessado da key para extrair o bit requirido
    char aux;
    int shift; //
    int bit_position_block = 7; //contador para a posição para por o bit requirido dentro do bloco
    char mask = 0b00000001; //mascara para isolar o bit após o shift de 7 bits a direita onde é colocado 1 a frente

    expansao = (char *) malloc(sizeof(char) * 6);
    //fazer a expansão de RRodada
    for (int i = 0; i < 48; ++i) {
        //descobre qual o bloco/caractere do RRodada será acessado
        position = (int) (EBITSELECTION[i] - 1) / 8;
        //calcula a quantidade de shifts a esquerda será necessário para isolar o bit desejado
        shift = EBITSELECTION[i] - (position * 8) - 1;
        aux = (char) RRodada[position] << shift;
        aux = aux >> 7;
        aux = aux & mask;
        //até aqui isolei o bit que quero usar
        aux = aux << bit_position_block;
        block_result = block_result | aux;

        bit_position_block--;
        if (bit_position_block < 0) {
            expansao[block_count] = block_result;
            block_count++;
            bit_position_block = 7;
            block_result = '\000';
        }

    }
    if (trace) {
        printf("RRodada expandido: ");
        printbits(expansao, 6);
    }

    //XOR entre chave e RRodada expandido
    for (int i = 0; i < 6; ++i) {
        expansao[i] = expansao[i] ^ round_key[i];
    }

    resultado = SBOXES(expansao);
    if (trace) {
        printf("Resultado SBOXES: ");
        printbits(resultado, 4);
    }

    PBox_feistel(resultado);
    if (trace) {
        printf("Resultado após PBOX: ");
        printbits(resultado, 4);
    }


}

void PBox_feistel(char *resultado) {
    //TODO: implementar a permitação do resultado da saida das SBoxes
}

char *SBOXES(char *expansao) {
    if (expansao == NULL) {
        return NULL;
    }

    char *resultado; //guarda o resultado das sboxes a ser retornado
    int linha, coluna;
    char block_in; //guarda o bloco que irá passar pelas sboxes
    char block_out, maskAux;
    char mask1 = 0b00111111;
    char maskLin = 0b00000001;
    char maskCol = 0b00001111;

    resultado = (char *) malloc(sizeof(char) * 4);

    //SB1
    block_in = ((expansao[0] >> 2) & mask1);
    coluna = (block_in >> 1) & maskCol;
    linha = (((block_in >> 5) & maskLin) << 1) | (block_in & maskLin);
    resultado[0] = SBOX1[linha][coluna];
    resultado[0] <<= 4;


    //SB2
    maskAux = 0b00000011;
    block_in = (expansao[0] & maskAux) << 4;
    maskAux = 0b00001111;
    block_in = ((expansao[1] >> 4) & maskAux) | block_in;
    coluna = (block_in >> 1) & maskCol;
    linha = (((block_in >> 5) & maskLin) << 1) | (block_in & maskLin);
    block_out = SBOX2[linha][coluna];
    resultado[0] |= block_out;

    //SB3
    block_in = (expansao[1] & maskAux) << 2;
    maskAux = 0b11000000;
    block_in = (((expansao[2] & maskAux) >> 6) & 0b00000011) | block_in;
    coluna = (block_in >> 1) & maskCol;
    linha = (((block_in >> 5) & maskLin) << 1) | (block_in & maskLin);
    resultado[1] = SBOX3[linha][coluna];
    resultado[1] <<= 4;

    //SB4
    maskAux = 0b00111111;
    block_in = (expansao[2] & maskAux);
    coluna = (block_in >> 1) & maskCol;
    linha = (((block_in >> 5) & maskLin) << 1) | (block_in & maskLin);
    block_out = SBOX4[linha][coluna];
    resultado[1] = resultado[1] | block_out;

    //SB5
    block_in = (expansao[3] >> 2) & maskAux;
    coluna = (block_in >> 1) & maskCol;
    linha = (((block_in >> 5) & maskLin) << 1) | (block_in & maskLin);
    resultado[2] = SBOX5[linha][coluna];
    resultado[2] <<= 4;

    //SB6
    maskAux = 0b00000011;
    block_in = ((expansao[3] & maskAux)) << 4;
    maskAux = 0b11110000;
    block_in = (((expansao[4] & maskAux) >> 4) & 0b00001111) | block_in;
    coluna = (block_in >> 1) & maskCol;
    linha = (((block_in >> 5) & maskLin) << 1) | (block_in & maskLin);
    block_out = SBOX6[linha][coluna];
    resultado[2] = resultado[2] | block_out;

    //SB7
    maskAux = 0b00001111;
    block_in = (expansao[4] & maskAux) << 2;
    maskAux = 0b11000000;
    block_in = (((expansao[5] & maskAux) >> 6) & 0b00000011) | block_in;
    coluna = (block_in >> 1) & maskCol;
    linha = (((block_in >> 5) & maskLin) << 1) | (block_in & maskLin);
    resultado[3] = SBOX7[linha][coluna];
    resultado[3] <<= 4;

    //SB8
    maskAux = 0b00111111;
    block_in = expansao[5] & maskAux;
    coluna = (block_in >> 1) & maskCol;
    linha = (((block_in >> 5) & maskLin) << 1) | (block_in & maskLin);
    block_out = SBOX8[linha][coluna];
    resultado[3] |= block_out;

    return resultado;
}

char *round_key(char *subkey_part1, char *subkey_part2, int round) {
    if (subkey_part1 == NULL || subkey_part2 == NULL || round < 0 || round > NUM_RODADAS) {
        return NULL;
    }
    char key_aux[7];
    char *key_round = (char *) malloc(sizeof(char) * 6);
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