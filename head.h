#ifndef HEAD_H //evita chamar duas vezes as funcoes

#define HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dirent.h" //biblioteca manipulação de diretórios POSIX (adap p windows)

//structs
typedef struct{
    char *linhas; //vai q ele escreve um pneumo n sei q das qnts
} Texto;

typedef struct{
    char name[256];
    char s_extensao[256]; //garante q n vão ser comparados arq especiais . e ..
    char ext[3];
    char *conteudo;
    long int size; //tam alocado p conteudo
    int qntd_linhas;
    Texto *texto;
} Arquivo;

//funcoes
void write_csv(char *tipo, char *arq1, char *arq2, float similaridade);
void get_name(Arquivo *file);
Arquivo *all_files(DIR *dir, Arquivo *files, int *size);
void comp_name(Arquivo *files, int size); //lista de arq com msm nome
void find_size(FILE *arq, long int *size);
FILE *open_archieve(char *nome);
char *get_content(char *dir_name, char *arq_name, char *conteudo, long int *size);
Texto *get_line(char *conteudo, long int size_conteudo, Texto *texto, int *qntd_palavras);
int comp_byte(char *conteudo1, char *conteudo2, long int size);
int mini(int a, int b, int c);
int edit_distance(char *word1, char *word2);
float line_line(Texto *texto1, Texto *texto2, int qntd1, int qntd2);
void comp_content(Arquivo *files, int size); //lista de arq e porcentagem de igualdade

#endif