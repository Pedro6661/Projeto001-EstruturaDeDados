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
void ordenarPorId(Processo *vet, int n);
void salvarCSV(const char *nomeArquivo, Processo *vet, int n);

#endif
