#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Para compilar: gcc processo.c main.c -o processo.exe
//Para executar: ./processo.exe

int main(){
    int n = 0;
    Processo *Dados = LerDados("processo_043_202409032338.csv", &n);

    for (int i = 0; i < n; i++) {
        printf("Id: %d\t Numero de processos: %s\t Data_ajuizamento: %s\t Id_classe: %s\t Id_assunto: %s\t Ano_eleicao: %d\n",
            Dados[i].id,
            Dados[i].numProcesso,
            Dados[i].data_ajuizamento,
            Dados[i].id_classe,
            Dados[i].id_assunto,
            Dados[i].ano_eleicao
        );
    }

    ordenarPorId(Dados, n);
    salvarCSV("ordenado_por_id.csv", Dados, n);

    free(Dados);
    return 0;
}
