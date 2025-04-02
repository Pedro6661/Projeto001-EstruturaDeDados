#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char numProcesso[50];
	char data_ajuizamento[50];
	char id_classe[50];
	int id_assunto;
	int ano_eleicao;
	
}Processo;

Processo *LerDados (const char *nomeArquivo);


int main(){
	int i;
	Processo *Dados = LerDados("C:\\Users\\pedry\\OneDrive\\Desktop\\Projeto001 - EST_DADOS\\processo_043_202409032338.csv");
	for (i = 0; i<20;i++)
		printf("%d \t %s \t %s \t %s \t %d \t %d \n",Dados[i].id,Dados[i].numProcesso,Dados[i].data_ajuizamento,Dados[i].id_classe,Dados[i].id_assunto,Dados[i].ano_eleicao);
}

Processo *LerDados(const char *nomeArquivo){
	FILE *fp = fopen(nomeArquivo,"r");
	if(fp == NULL){
		printf("ERRO: O arquivo nao pode ser aberto\n");
		exit(1);
	}
	
	Processo *X = malloc(18400*sizeof(Processo));
	if(X==NULL){
		printf("ERRO: N�o foi possivel alocar memoria para a tabela\n");
		exit(1);
	}
	
	char Cabecalho[100];
	fscanf(fp,"%s",Cabecalho);
	int N=0;
	
	while(fscanf(fp,"%d,\"%49[^\"]\",%49[^,] ,{%49[^}]},{%d},%d",&X[N].id,X[N].numProcesso,X[N].data_ajuizamento,X[N].id_classe,&X[N].id_assunto,&X[N].ano_eleicao)==6){
	N++;
	}
	
	fclose(fp);
	return X;
	
}
