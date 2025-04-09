#include "processo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINHAS 18393
#define MAX_CAMPOS 10
#define TAM_CAMPO 256

void limparMultivalorado(char *campo) {
    char temp[100];
    int j = 0;

    for (int i = 0; campo[i] != '\0'; i++) {
        char c = campo[i];
        if (c != '{' && c != '}' && c != '"' && c != ',') {
            temp[j++] = c;
        }
    }

    temp[j] = '\0';
    strcpy(campo, temp);
}
//vai ler o arquivo sem as ""
//vai retornar o index da coluna tal tratada
//Parsing: solução encontrada lendo em manual C e foruns stackoverflow para tratamento de linhas de string complexas
int parseCSV(char *linha, char campos[][TAM_CAMPO], int max_campos) {
    int index = 0;
    char *cursor = linha;

    while (*cursor && index < max_campos) {
        //percorre o indice, a coluna e cada caracter dela com o cursosr
        if (*cursor == '"') {
            cursor++;// se o cursor encontrar as aspas ele pula as aspas
            char *inicioCampo = cursor; //inicio de campo agr tem os valores entre as aspas
            
            int tamanho = 0; 
            //procura a proxima aspas para pular
            while (*cursor && *cursor != '"') {
                cursor++;
                tamanho++;// contador de caracteres
            }
            // copia inicio de campo para campos index, e o numero de caracteres a serem copiados
            strncpy(campos[index], inicioCampo, tamanho);
            campos[index][tamanho] = '\0';

            //Após terminar o campo, se o cursor está numa aspa ("), pula essa aspa de fechamento.
            if (*cursor == '"') cursor++;
            if (*cursor == ',') cursor++;
        } else {
            char *inicioCampo = cursor;
            int tamanho = 0;

            while (*cursor && *cursor != ',') {
                cursor++;
                tamanho++;
            }

            strncpy(campos[index], inicioCampo, tamanho);
            campos[index][tamanho] = '\0';

            if (*cursor == ',') cursor++;
        }

        index++;
    }

    return index;
}

//função para ler o CSV
Processo *LerDados(const char *nomeArquivo, int *n) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (fp == NULL) {
        printf("ERRO: O arquivo não pode ser aberto!\n");
        exit(1);
    }

    Processo *processo = malloc(MAX_LINHAS * sizeof(Processo));
    if (processo == NULL) {
        printf("ERRO: Não foi possível alocar memória para a tabela!\n");
        exit(1);
    }

    char linha[512];
    int i = 0;

    if (fgets(linha, sizeof(linha), fp) == NULL) {
        fclose(fp);
        free(processo);
        return NULL;
    }

    while (fgets(linha, sizeof(linha), fp) && i < MAX_LINHAS) {
        linha[strcspn(linha, "\n")] = '\0';

        char campos[6][256] = {{0}};
        int qtd = parseCSV(linha, campos, 6);
        // depois de ler os valores sem aspas ele vai devolvolver e copiar no struct
        if (qtd == 6) {
            processo[i].id = atoi(campos[0]);
            strncpy(processo[i].numProcesso, campos[1], sizeof(processo[i].numProcesso));
            strncpy(processo[i].data_ajuizamento, campos[2], sizeof(processo[i].data_ajuizamento));
            strncpy(processo[i].id_classe, campos[3], sizeof(processo[i].id_classe));
            strncpy(processo[i].id_assunto, campos[4], sizeof(processo[i].id_assunto));
            processo[i].ano_eleicao = atoi(campos[5]);

            limparMultivalorado(processo[i].id_classe);
            limparMultivalorado(processo[i].id_assunto);

            i++;
        }
    }

    fclose(fp);
    *n = i;
    return processo;
}
//1. Ordenar, em ordem crescente, o conjunto de dados a partir do atributo “id”;
void swap(Processo *a, Processo *b) {
    Processo tmp = *a;
    *a = *b;
    *b = tmp;
}

int compararPorId(Processo *a, Processo *b) {
    return a->id - b->id;
}

void quicksort(Processo *vet, int inf, int sup, int (*comparar)(Processo *, Processo *)) {
    int i = inf, j = sup;
    Processo pivo = vet[(inf + sup) / 2];

    while (i <= j) {
        while (comparar(&vet[i], &pivo) < 0) i++;
        while (comparar(&vet[j], &pivo) > 0) j--;
        if (i <= j) {
            swap(&vet[i], &vet[j]);
            i++; j--;
        }
    }
    if (inf < j) quicksort(vet, inf, j, comparar);
    if (i < sup) quicksort(vet, i, sup, comparar);
}

void ordenarPorId(Processo *vet, int n) {
    quicksort(vet, 0, n - 1, compararPorId);
}

//Salvar em csv 1 e 2
void salvarCSV(const char *nomeArquivo, Processo *vet, int n) {
    FILE *fp = fopen(nomeArquivo, "w");
    if (!fp) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }

    fprintf(fp, "id,numProcesso,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d,\"%s\",%s,{%s},{%s},%d\n",
                vet[i].id,
                vet[i].numProcesso,
                vet[i].data_ajuizamento,
                vet[i].id_classe,
                vet[i].id_assunto,
                vet[i].ano_eleicao);
    }

    fclose(fp);
    printf("Dados salvos com sucesso em %s\n", nomeArquivo);
}

//3. Contar quantos processos estão vinculados a um determinado “id_classe”;
void ContarPorClasse(Processo processos[], int n) {
    char idsAgrupados[5000][50]; // Vetor local de id_classe únicos
    int contadores[5000] = {0};  // Contador de ocorrências
    int totalGrupos = 0;

    for (int i = 0; i < n; i++) {
        int encontrado = 0;

        for (int j = 0; j < totalGrupos; j++) {
            if (strcmp(processos[i].id_classe, idsAgrupados[j]) == 0) {
                contadores[j]++;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            strcpy(idsAgrupados[totalGrupos], processos[i].id_classe);
            contadores[totalGrupos] = 1;
            totalGrupos++;
        }
    }

    printf("\nContagem de processos por id_classe:\n");
    for (int i = 0; i < totalGrupos; i++) {
        printf("id_classe: %s tem: %d processos\n", idsAgrupados[i], contadores[i]);
    }
}