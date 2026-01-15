#ifndef HEAD_H //evita chamar duas vezes as funcoes

#define HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dirent.h" //biblioteca manipulação de diretórios POSIX (adap p windows)

//structs
typedef struct{
    char *palavra;
    int qntd; //qnts vezes aparece no texto n sei se é necessario
} Texto;
typedef struct{
    char name[256];
    char s_extensao[256]; //garante q n vão ser comparados arq especiais . e ..
    char *conteudo;
    long int size; //tam alocado p conteudo
    Texto *texto;
} Arquivo;

//funcoes
void get_name(Arquivo *file);
Arquivo *all_files(DIR *dir, Arquivo *files, int *size);
void comp_name(Arquivo *files, int size); //lista de arq com msm nome
void find_size(FILE *arq, long int *size);
FILE *abre_arquivo(char *nome);
Arquivo *get_conteudo(char *dir_name, Arquivo *files, int size);
int comp_byte(char *conteudo1, char *conteudo2, long int size);
// void comp_content(Arquivo *files, int size); //lista de arq e porcentagem de igualdade

#endif