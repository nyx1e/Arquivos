#include "head.h"
//outros includes em head...

int main() {
    int option;
    int size = 0; //tam do vetor aloc dinam
    DIR *dir; 
    char dir_name[256] = "C:/Users/Sousa/OneDrive/Documentos/codigo c/Retirando arquivos repetidos/pasta_teste"; //path
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
            comp_name(files, size); 
            break;

            case 2:
            //conteúdo
            //comp_content(files, size); 
            break;

            default:
            printf("Opcao invalida\n");
        }
    }while (option!=1 && option !=2);

    free(files);
    closedir(dir);
    return 0;
}