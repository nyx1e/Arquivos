#include "head.h"

void write_csv(char *tipo, char *arq1, char *arq2, float similaridade){
    FILE *saida = fopen("saida.csv", "a");
    if (saida == NULL){
        printf("Erro ao escrever no csv\n");
    }
    else{
        fprintf(saida, "%s;%s;%s;similaridade:%.2f%%;\n", tipo, arq1, arq2, similaridade);
        fclose(saida); 
    }
    return;
}

void get_name(Arquivo *file){
    int idx=0, j=0;
    int s_name = strlen(file->name);
    for(int i=0; i<s_name; i++){
        if (file->name[i] != '.'){
            file->s_extensao[i] = file->name[i];  
        }
        else{
            idx=i;
            file->s_extensao[i] = '\0';
            break;
        }
    }
    for(int i=idx; i<s_name; i++){
        file->ext[j]=file->name[i];
        j++;
    }
    file->ext[j]= '\0';
}

Arquivo *all_files(DIR *dir, Arquivo *files, int *size){
    struct dirent *entrada; //estrutura para cada entrada (arquivo/pasta)
    Arquivo *temp; //garante q eu n perca os q foram alocados corretamente
    printf("\nLista de arquivos no diretorio:\n");
    while ((entrada = readdir(dir)) != NULL) { //readdir lê
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) continue;
        if (entrada->d_type == DT_DIR) continue;
        printf("%s\n", entrada->d_name); 

        //coloca tds os arq do dir num vetor alc dinam
        temp = (Arquivo*) realloc(files, ((*size)+1)*sizeof(Arquivo)); 
        if (temp == NULL){
            printf("Erro de alocacao\n");
            free(files);
            closedir(dir);
            return NULL;
        }
        files = temp;
        strcpy((files+(*size))->name, entrada->d_name);
        get_name(files+(*size));
        (*size)++;
    }
    return files;
}

void comp_name(Arquivo *files, int size){ //lista de arq com msm nome
    for(int i=0; i<size-1; i++){
        for(int j=i+1; j<size; j++){ //_stricmp testar se precisa dps
            if (stricmp((files+i)->s_extensao, (files+j)->s_extensao) == 0){
                write_csv("comp_nome", (files+i)->name, (files+j)->name, 100.0);
                printf("Arquivos com nomes iguais:\n%s\n%s\n", (files+i)->name, (files+j)->name);
                return;
            }        
        }
    }
    printf("Nao existem arquivos com nomes iguais\n");
    return;
}

void find_size(FILE *arq, long int *size){
    fseek(arq, 0L, SEEK_END); //ponteiro p fim retorna 0 em sucesso
    *size = ftell(arq); // retorna a posicao do ponteiro como um long intq é o num de bytes
    rewind(arq);
}

FILE *open_archieve(char *nome){ //implenta junto do loop p abrir tds os arq do vetor
    FILE *arq = fopen(nome, "rb");
    if (arq == NULL) {
        printf("Erro de abertura arq\n");
    }
    return arq; 
}

char *get_content(char *dir_name, char *arq_name, char *conteudo, long int *size){ //adaptar p retornar um char
    char *temp = NULL;
    char name[400];
    sprintf(name, "%s/%s", dir_name, arq_name); //se eu usar strcat muda dir_name
    FILE *arq = open_archieve(name);
    if (arq != NULL){
        find_size(arq, size);
        temp = (char*) realloc(conteudo, (*size) + 1); //por conta do \0 p n dar birolha
        if (temp == NULL){
            printf("Erro de alocacao\n");
            fclose(arq);
            return NULL;
        }
        else{
            conteudo = temp;
            size_t bytes_lidos = fread(conteudo, 1, *size, arq); //garantir q leu td
            if (bytes_lidos != *size){
                printf("Erro n leu completo, lidos %ld\n", bytes_lidos);
                free(conteudo);
                fclose(arq);
                return NULL;
            }
            conteudo[*size] = '\0';
        }
        fclose(arq);
        return conteudo;
    }
    return NULL;
}

Texto *get_line(char *conteudo, long int size_conteudo, Texto *texto, int *qntd_linhas){
    char marcadores[]="\n\r";
    char *copia = (char*) malloc(size_conteudo + 1); //p n perder o q tem em conteudo
    if (copia == NULL) return NULL;
    if (size_conteudo == 0){ //p caso de n ter nd
        *qntd_linhas = 0;
        return NULL;
    }
    strcpy(copia, conteudo);
    char *linha = strtok(copia, marcadores); 
    while (linha != NULL){
        int encontrada = 0; //só qro linhas únicas
        for (int i = 0; i < *qntd_linhas; i++){
            if (strcmp(texto[i].linhas, linha) == 0){
                encontrada = 1;
                break;
            }
        }
        if (!encontrada){
            Texto *temp = (Texto*) realloc(texto, ((*qntd_linhas) + 1) * sizeof(Texto));
            if (temp == NULL){
                free(texto);
                free(copia);
                *qntd_linhas = 0;
                return NULL;
            }
            texto = temp;
            size_t tam_linha = strlen(linha) + 1; //garante q retorna um valor suficiente p armazenar o char
            texto[*qntd_linhas].linhas = (char*) malloc(tam_linha);
            if (texto[*qntd_linhas].linhas == NULL){
                for (int i = 0; i < *qntd_linhas; i++){
                    free(texto[i].linhas);
                }
                free(texto);
                free(copia);
                *qntd_linhas = 0;
                return NULL;
            }
            strcpy(texto[*qntd_linhas].linhas, linha);
            (*qntd_linhas)++;
        }
        linha = strtok(NULL, marcadores);
    }
    free(copia);
    return texto;
}

int comp_byte(char *conteudo1, char *conteudo2, long int size){ 
    if (size==0) return 1;
    for (long int i = 0; i < size; i++) {
        if (conteudo1[i] != conteudo2[i]) return 0;
    }
    return 1;
}

int mini(int a, int b, int c){
    int menor = a;
    if (b < menor) menor = b;
    if (c < menor) menor = c;
    return menor;
}

int edit_distance(char *word1, char *word2){
    int len1 = strlen(word1);
    int len2 = strlen(word2);
    int **M = (int**)malloc((len1+1)*sizeof(int*));
    if (M == NULL){
        printf("Erro de alocacao\n");
        return -1;
    }
    for (int i = 0; i <= len1; i++){
        M[i] = (int*)malloc((len2+1)*sizeof(int));
        if (M[i] == NULL){
            printf("Erro de alocacao\n");
            for (int k = 0; k < i; k++){
                free(M[k]);
            }
            free(M);
            return -1;
        }
        for (int j = 0; j <= len2; j++){
            M[i][j] = 0;
        }
    }
    for (int i = 0; i <= len1; i++){ //caso base com linha e coluna ""
        M[i][0] = i;
    }
    for (int j = 0; j <= len2; j++){
        M[0][j] = j;
    }
    for (int i = 1; i <= len1; i++){
        for (int j = 1; j <= len2; j++){
            if (word1[i-1] == word2[j-1]){
                M[i][j] = M[i-1][j-1];
            }
            else{
                M[i][j] = mini(
                    M[i-1][j] + 1,      //del
                    M[i][j-1] + 1,      //inser
                    M[i-1][j-1] + 1     //subst
                );
            }
        }
    }    
    int resultado = M[len1][len2];
    for (int i = 0; i <= len1; i++){
        free(M[i]);
    }
    free(M);
    return resultado;
}

float line_line(Texto *texto1, Texto *texto2, int qntd1, int qntd2){
    long int diff_total = 0;
    int dist = 0;
    int max_len = 0; 
    int min_linhas = (qntd1 < qntd2) ? qntd1 : qntd2;
    for (int i = 0; i < min_linhas; i++){
        dist = edit_distance(texto1[i].linhas, texto2[i].linhas);
        if (dist == -1){
            return -1.0;
        }
        diff_total += dist;
        int len1 = strlen(texto1[i].linhas), len2 = strlen(texto2[i].linhas);
        max_len += len1>len2? len1:len2;
    }
    for (int i = min_linhas; i < qntd1; i++){ //adiciona as linhas q sobram a diferença e tbm ao total
        int len = strlen(texto1[i].linhas);
        diff_total += len;
        max_len += len;
    }
    for (int j = min_linhas; j < qntd2; j++){
        int len = strlen(texto2[j].linhas);
        diff_total += len;
        max_len += len;
    }
    return (1.0 - ((float)diff_total / (float)max_len)) * 100.0;
}

void comp_content(Arquivo *files, int size){
    float similaridade; 
    for (int i=0; i<size; i++){ 
        if ((files+i)->conteudo == NULL || (files+i)->texto == NULL){
            printf("Erro de processamento\n");
            continue;
        }
        for (int j=0; j<size; j++){
            if (files+i == files+j) continue;
            if ((files+j)->conteudo == NULL || (files+j)->texto == NULL){
                printf("Erro de processamento\n");
                continue;
            }
            if (strcmp((files+i)->ext, (files+j)->ext)!=0) continue;
            if ((files+i)->size == (files+j)->size){
                if (comp_byte((files+i)->conteudo, (files+j)->conteudo, (files+i)->size)){ 
                    similaridade = 100.0;
                }
                else{
                    similaridade = line_line((files+i)->texto, (files+j)->texto, (files+i)->qntd_linhas, (files+j)->qntd_linhas);
                }
            }
            else{
                similaridade = line_line((files+i)->texto, (files+j)->texto, (files+i)->qntd_linhas, (files+j)->qntd_linhas);
            }
            if (similaridade>=0){
                write_csv("comp_content", (files+i)->name, (files+j)->name, similaridade);
                printf("A similaridade dos arquivos %s e %s e de %.2f%%\n", (files+i)->name, (files+j)->name, similaridade);
            }
        }
    }
}