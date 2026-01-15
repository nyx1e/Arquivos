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
            for(int i=0;i<size;i++){
                (files+i)->conteudo = NULL;
                (files+i)->texto = NULL;
                (files+i)->conteudo = get_conteudo(dir_name, (files+i)->name, (files+i)->conteudo, &(files+i)->size);
                if ((files+i)->conteudo != NULL){ 
                    (files+i)->qntd_palavras = 0;
                    (files+i)->texto = get_text((files+i)->conteudo, (files+i)->size, (files+i)->texto, &(files+i)->qntd_palavras);
                    if ((files+i)->texto==NULL){ 
                        printf("Erro get_text");
                        break;
                    }
                }
                else{
                    printf("Erro get_conteudo");
                    break;
                }
            }
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