#ifndef PROCESSO_H
#define PROCESSO_H

#include <stdio.h>

typedef struct {
	int id;
	char numProcesso[100];
	char data_ajuizamento[100];
	char id_classe[50]; 
	char id_assunto[50];
	int ano_eleicao;	
} Processo;

Processo *LerDados(const char *nomeArquivo, int *n);

/*

Processo ordenarPorId(Processo p, int n);
Processo ordenarPorData(Processo *p, int n);
Processo contadorDeIdClasse(Processo *p, int n, const char *id_classe);
Processo contadorDeIdAssuntos(Processo *p, int n);
Processo listarProcessosMultiplosAssuntos(Processo *p, int n);
Processo diasemTramitacao(Processo *p, int n);
*/

#endif