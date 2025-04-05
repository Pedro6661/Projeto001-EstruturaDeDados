#include "processo.h"
#include <stdlib.h>
#include <string.h>

Processo *LerDados (const char *nomeArquivo){
    FILE *fp = fopen(nomeArquivo, "r");
    if(fp == NULL) {
        printf("ERRO: O arquivo não pode ser aberto!\n");
        exit(1);
    }

    Processo *X = malloc(20000 * sizeof(Processo));
    if(X == NULL) {
        printf("ERRO: Não foi possível alocar memória para a tabela!\n");
        exit(1);
    }

	char Cabecalho[100];
	fscanf(fp, "%s", Cabecalho);
	int N = 0;
	/*(fscanf(fp,formatação do tipo da variavel se for int %d se for char %[^;];%[^;]",atribui a variável vetor de processos*/
	while(fscanf(fp,"%d,\"%[^\"]\",%[^,],{%[^}]},{%[^}]},%d", &X[N].id, &X[N].numProcesso,&X[N].data_ajuizamento,&X[N].id_classe,&X[N].id_assunto,&X[N].ano_eleicao )== 6/*numero de colunas*/){
		N++;
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