//
// Created by Daniel de Vargas on 01/09/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NOMEARQUIVO 255
#define TAMANHOBLOCO 8 //em bytes

short int trace;

int main(){
    FILE *fkey;
    FILE *fplaintext;
    FILE *fcifra;
    char nome_arquivo[NOMEARQUIVO];
    short int menu_principal;
    char key[TAMANHOBLOCO];

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
        printf("\n1-Encriptação  2-Decriptação\nEscolha: ");
        scanf("%hu", &menu_principal);


        printf("\nArquivo de chave: ");
        scanf("%s", nome_arquivo);
        fkey = fopen(strcat(nome_arquivo, ".txt"), "r");
        if (fkey == NULL) {
            printf("Erro ao abrir arquivo de chave, certifique-se de que seja um .txt e digite apenas o nome");
            exit(-1);
        }


        switch (menu_principal) {
            case 2:
                printf("\nArquivo de cifra: ");
                scanf("%s", nome_arquivo);
                fplaintext = fopen(strcat(nome_arquivo, ".txt"), "r");
                if (fplaintext == NULL) {
                    printf("Erro ao abrir arquivo de chave, certifique-se de que seja um .txt e digite apenas o nome\n");
                    exit(-1);
                }

                printf("Defina o nome do arquivo de plaintext: ");
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
                break;
            default:
                printf("Opção inválida\n");
        }
    }while (menu_principal != 0);


    if (trace){
        printf("INFO - Lendo e validando chave");
    }



}