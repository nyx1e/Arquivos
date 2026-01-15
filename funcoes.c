#include "head.h"

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
}

Arquivo *all_files(DIR *dir, Arquivo *files, int *size)
{
    struct dirent *entrada; //estrutura para cada entrada (arquivo/pasta)
    Arquivo *temp; //garante q eu n perca os q foram alocados corretamente

    printf("Lista de arquivos no diretorio:\n");

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

void comp_name(Arquivo *files, int size)
{ //lista de arq com msm nome
    bool printed[size];

    for (int i = 0; i < size; i++){ //inicializa
        printed[i] = false;
    }

    for(int i=0; i<size-1; i++){
        for(int j=i+1; j<size; j++){ //_stricmp testar se precisa dps
            if (stricmp((files+i)->s_extensao, (files+j)->s_extensao) == 0){
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

void find_size(FILE *arq, long int *size){
    fseek(arq, 0L, SEEK_END); //ponteiro p fim retorna 0 em sucesso
    *size = ftell(arq); // retorna a posicao do ponteiro como um long intq é o num de bytes
    rewind(arq);
}

FILE *abre_arquivo(char *nome){ //implenta junto do loop p abrir tds os arq do vetor
    FILE *arq = fopen(nome, "rb");
    
    if (arq == NULL) {
        printf("Erro de abertura arq\n");
    }
    
    return arq; 
}

char *get_conteudo(char *dir_name, char *arq_name, char *conteudo, long int *size){ //adaptar p retornar um char
    char *temp = NULL;
    char name[400];
    sprintf(name, "%s/%s", dir_name, arq_name); //se eu usar strcat muda dir_name
    FILE *arq = abre_arquivo(name);
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
            size_t bytes_lidos = fread(conteudo, 1, *size, arq);
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

Texto *get_text(char *conteudo, long int size_conteudo, Texto *texto, int *qntd_palavras){
    char marcadores[] = ".,; \n";
    char *copia = (char*) malloc(size_conteudo + 1); //precisa p n perder conteudo

    if (copia == NULL) return NULL;
    if (size_conteudo ==0){
        *qntd_palavras = 0;
        return NULL;
    }
    strcpy(copia, conteudo);

    char *palavra = strtok(copia, marcadores); 
    while (palavra != NULL){
        int encontrada = 0;
        if (strlen(palavra) >= 47){ 
            palavra[47] = '\0';
        }
        for (int i = 0; i < *qntd_palavras; i++){
            if (strcmp(texto[i].palavras, palavra) == 0){
                encontrada = 1;
                break;
            }
        }
        if (!encontrada){ //só qro q palavras unicas sejam consideradas
            Texto *temp = (Texto*) realloc(texto, ((*qntd_palavras) + 1) * sizeof(Texto));
            if (temp == NULL){
                free(texto);
                free(copia);
                return NULL;
            }
            texto = temp;
            strncpy(texto[*qntd_palavras].palavras, palavra, 46);
            texto[*qntd_palavras].qntd = 1;
            (*qntd_palavras)++;
        }
        palavra = strtok(NULL, marcadores);
    }
    free(copia);
    return texto;
}

int comp_byte(char *conteudo1, char *conteudo2, long int size){ 
    for (long int i = 0; i < size; i++) {
        if (conteudo1[i] != conteudo2[i]) return 0;
    }

    return 1;
}

void comp_content(Arquivo *files, int size){
// fazer funcao get strings p usar dentro de for p acessar td conteudo do arq p comparar usando algoritmo de jaccard
    float similaridade;
    for (int i=0; i<size-1; i++){
        //pego arq da vez
        for (int j=i+1; j<size; j++){
            if((files+i)->size==(files+j)->size){
                if (!comp_byte((files+i)->conteudo, (files+j)->conteudo, (files+i)->size)){
                    //comp byte a byte e se n for 100 igual faz jaccard
                    // similaridade = jaccard();
                }else{
                    similaridade = 100.0;
                }
            }else{
                long int max, min;
                if ((files+i)->size > (files+j)->size){
                    max = (files+j)->size;
                }else{
                    max = (files+i)->size;
                }
                //se tamanhos diferentes aplica jaccard 
                // similaridade = jaccard();
            }
        }
    }
}