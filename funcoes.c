#include "head.h"
#include <stdbool.h>

//perguntas
//1. Preciso abrir um diretorio p checar tds os arq ou poderia ser um vetor de arq?
//2. structs precisam ser definidas na parte do head e da de funcoes p funcionar?
//3. Por nome, tem problema se mostrar a extensão tambem?
//4. Arq especiais . e .. devem ser considerados?
//5. Preciso incluir uma pasta q está dentro da pesquisada? exemplo: codigo c e dentro de dele tem pastas como lab 1, etc
//6. Pq bool printed[size]={false} n funciona?
//7. Preciso tirar os arq repetidos? na coisa dos temas n pede mas é literalmente o nome do tema

void get_name(Arquivo *file)
{
    int s_name = strlen(file->name);

    for(int i=0; i<s_name; i++){
        if (file->name[i] != '.'){
            file->s_extensao[i] = file->name[i];  
        }
        else{
            file->s_extensao[i] = '\0';
            break;
        }

    }

    file->s_extensao[s_name] = '\0'; //se for uma pasta e n um arq, pastas n tem extensão
}

Arquivo *all_files(DIR *dir, Arquivo *files, int *size)
{
    struct dirent *entrada; //estrutura para cada entrada (arquivo/pasta)
    Arquivo *temp; //garante q eu n perca os q foram alocados corretamente

    printf("Lista de arquivos no diretorio:\n");

    while ((entrada = readdir(dir)) != NULL) { //readdir lê
        printf("%s\n", entrada->d_name); 
        (*size)++;

        //coloca tds os arq do dir num vetor alc dinam
        temp = (Arquivo*) realloc(files, (*size)*sizeof(Arquivo)); 
        if (temp == NULL){
            printf("Erro de alocacao\n");
            free(files);
            closedir(dir);
            exit(1);
        }
        files = temp;
        strcpy((files+(*size)-1)->name, entrada->d_name);
        get_name(files+(*size)-1);
    }

    return files;
}

void comp_name(Arquivo *files, int size)
{ //lista de arq com msm nome
    bool printed[size];

    for (int i = 0; i < size; i++){ //inicializa
        printed[i] = false;
    }

    for(int i=0; i<size-1; i++){
        for(int j=i+1; j<size; j++){ //strcasecmp em linux
            if (stricmp((files+i)->s_extensao, (files+j)->s_extensao) == 0 && strlen((files+i)->s_extensao) > 0){ //strlen p garantir q tem algo na s_extensao
                if (!(printed[i])){
                    printf("%s\n", (files+i)->name);
                    printed[i] = true;
                }
                if (!(printed[j])){
                    printf("%s\n", (files+j)->name);
                    printed[j] = true;
                }
            }        
        }
    }
}

// void open(arq)
// {   
//     char filename[20];
//     FILE * arq;
//     arq = fopen (filename,"r");
//     if (arq == NULL){
//         printf ("Erro de abertura");
//     }
// }

// void close(arq)
// {
//     fclose(arq);
// }

// void comp_content(Arquivo *files, int size)
// { //lista de arq e porcentagem de igualdade
//     //poderia comparar usando strcmp e vendo a diferença entre str q eu iria pegar usando fread de cada um dos arq
// }

// fazer funcao get strings p usar dentro de for p acessar td conteudo do arq p comparar usando algoritmo de levenstein