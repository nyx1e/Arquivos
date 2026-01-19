#include "head.h"
//outros includes em head...

int main() {
    int option;
    int size = 0; //tam do vetor aloc dinam
    DIR *dir; 
    char dir_name[256]; //path
    Arquivo *files = NULL;

    printf("Informe o path ou path relativo do diretorio\n");
    scanf("%[^\n]s", dir_name);
    
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
        printf("Pesquisa por:\n0 - Sair\n1 - Comparacao por Nome\n2 - Comparacao por Conteudo\n");
        scanf("%d", &option);

        switch (option){
            case 0:
            printf("Programa encerrado\n");   
            break;

            case 1:
            //nome
            comp_name(files, size); 
            printf("Sucesso em arquivar similaridades\n\n");   
            break;

            case 2:
            //conteúdo
            for(int i=0;i<size;i++){
                (files+i)->conteudo = NULL;
                (files+i)->texto = NULL;
                (files+i)->conteudo = get_content(dir_name, (files+i)->name, (files+i)->conteudo, &(files+i)->size);
                if ((files+i)->conteudo != NULL){ 
                    (files+i)->qntd_linhas = 0;
                    (files+i)->texto = get_line((files+i)->conteudo, (files+i)->size, (files+i)->texto, &(files+i)->qntd_linhas);
                    if ((files+i)->texto==NULL){ 
                        printf("Erro get_text\n");
                        break;
                    }
                }
                else{
                    printf("Erro get_conteudo\n");
                    break;
                }
            }
            comp_content(files, size);
            printf("Sucesso em arquivar similaridades\n\n");   
            break;

            default:
            printf("Opcao invalida\n");
        }
    }while (option!=0);

    free(files);
    closedir(dir);
    return 0;
}