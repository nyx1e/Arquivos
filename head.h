#ifndef HEAD_H
#define HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> //biblioteca manipulação de diretórios POSIX

//chamo as funções que crio

typedef struct{
    char name[256];
    char s_extensao[256]; //garante q n vão ser comparados arq especiais . e ..
}Arquivo;

void get_name(Arquivo *file);
Arquivo *all_files(DIR *dir, Arquivo *files, int *size);
void comp_name(Arquivo *files, int size); //lista de arq com msm nome

// void open(arq);
// void close(arq);
// void comp_content(Arquivo *files, int size); //lista de arq e porcentagem de igualdade

#endif