#include <stdio.h>
#include "processo.h"

int main()
{

    int n = 0; //caso queira ler quantidade que leu em processo.c
    Processo *Dados = LerDados("processo_043_202409032338.csv", &n);

    ordenarPorId(Dados, n);
    salvarCSV("ordenado_por_id.csv", Dados, n);
    ContarPorClasse(Dados, n);
    
    /*// Descomentar para ver os printf abaixo tratados e limpos:
    //ou i< n;
    for (int i = 0; i < 15; i++)
    {
        printf(" | ID:%d\n | Numero:%s\n | Data:%s\n | Classe:%s\n | Assunto:%s\n | Ano:%d\n",
               Dados[i].id,
               Dados[i].numProcesso,
               Dados[i].data_ajuizamento,
               Dados[i].id_classe,
               Dados[i].id_assunto,
               Dados[i].ano_eleicao);
               printf("_________________________________\n");
    }*/
    return 0;
}
