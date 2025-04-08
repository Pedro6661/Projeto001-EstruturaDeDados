#include "processo.h"
#include <stdlib.h>
#include <string.h>
#define MAX_LINHAS 15

Processo *LerDados(const char *nomeArquivo, int *n) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (fp == NULL) {
        printf("ERRO: O arquivo não pode ser aberto!\n");
        exit(1);
    }

    Processo *X = malloc(MAX_LINHAS * sizeof(Processo));
    if (X == NULL) {
        printf("ERRO: Não foi possível alocar memória para a tabela!\n");
        exit(1);
    }

    char Cabecalho[100];
    fscanf(fp, "%s", Cabecalho);
    *n = 0;

    char linha[512]; 

    while (*n < MAX_LINHAS && fgets(linha, sizeof(linha), fp)) {
        if (sscanf(linha, "%d,\"%99[^\"]\",%99[^,],%99[^,],%99[^,],%d",
                   &X[*n].id, X[*n].numProcesso,
                   X[*n].data_ajuizamento, X[*n].id_classe,
                   X[*n].id_assunto, &X[*n].ano_eleicao) == 6) {

            int i = 0, j = 0;
            while (X[*n].id_classe[i] != '\0') {
                if (X[*n].id_classe[i] != '{' && X[*n].id_classe[i] != '}' && X[*n].id_classe[i] != '"') {
                    X[*n].id_classe[j++] = X[*n].id_classe[i];
                }
                i++;
            }
            X[*n].id_classe[j] = '\0';

            i = j = 0;
            while (X[*n].id_assunto[i] != '\0') {
                if (X[*n].id_assunto[i] != '{' && X[*n].id_assunto[i] != '}' && X[*n].id_assunto[i] != '"') {
                    X[*n].id_assunto[j++] = X[*n].id_assunto[i];
                }
                i++;
            }
            X[*n].id_assunto[j] = '\0';

            (*n)++;
        }
    }

    fclose(fp);
    return X;
}

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
