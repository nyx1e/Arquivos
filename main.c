#include "head.h"
//outros includes em head...

int main() {
    //variaveis
    int option;
    int size = 0; //tam do vetor aloc dinam
    DIR *dir; 
    char dir_name[256] = "/Users/Sousa/OneDrive/Documentos/codigo c/pasta_teste"; //path
    Arquivo *files = NULL;

    //mostra tds os arq no dir
    dir = opendir(dir_name); //ele abre funciona como fopen p dir

    if (dir == NULL) { //checagem
        printf("Erro de abertura do dir\n");
        closedir(dir);
        return 1;
    }
    else{
        files = all_files(dir, files, &size);
        printf("\n");
    }

    //comparacao
    do{
        printf("Pesquisa por:\n1 - Nome\n2 - Conteudo\n");
        scanf("%d", &option);

        switch (option){
            case 1:
            //nome
            // comp_name(files, size); 
            break;

            case 2:
            //conteúdo
            //comp_content(files, size); 
            break;

            default:
            printf("Opcao invalida\n");
        }
    }while (option!=1 && option !=2);
    
    //tava pensando em usar a propria struct dirent p pegar nome e extensao(strncat) e colocar num vetor assim dá p comparar por nome e tamanho
    //path = entrada->d_name + '.' + entrada->d_type; //se usasse essas logicas precisa ta dentro de um for ou até msm dentro do while de cima p pegar tds e colocar no vetor p fazer o fopen de cada arq na h da comparacao

    free(files);
    closedir(dir);
    return 0;
}