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
                   X[*n].id_assunto, &X[*n].ano_eleicao) == 6/*numero de colunas*/) {

            // Limpar id_classe 
            //vai rodar até o final da palavra \0
            int i = 0, j = 0;
            while (X[*n].id_classe[i] != '\0') {
                if (X[*n].id_classe[i] != '{' && X[*n].id_classe[i] != '}' && X[*n].id_classe[i] != '"') {
                    X[*n].id_classe[j++] = X[*n].id_classe[i];
                }
                i++;
            }
            X[*n].id_classe[j] = '\0';

            // Limpar id_assunto 
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


/*
//funções

//1. Ordenar, em ordem crescente, o conjunto de dados a partir do atributo “id”;
ordenarPorId(Processo p, int n){

}//crescente

//2. Ordenar, em ordem decrescente, o conjunto de dados a partir do atributo “data_ajuizamento”;
ordenarPorData(){

} //decrescente

//3. Contar quantos processos estão vinculados a um determinado “id_classe”;
contadorDeIdClasse(){

}
//4. Identificar quantos “id_assuntos” constam nos processos presentes na base de dados; 
contadorDeIdAssuntos(){

}
//5. Listar todos os processos que estão vinculados a mais de um assunto;
listarProcessosMultiplosAssuntos(){

}
//6. Indicar a quantos dias um processo está em tramitação na justiça.
diasemTramitacao(){

}
*/