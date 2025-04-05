#ifndef PROCESSO_H
#define PROCESSO_H

#include <stdio.h>

typedef struct {
	int id;
	char numProcesso[50];
	char data_ajuizamento[50];
	char id_classe[50]; 
	char id_assunto[50];
	int ano_eleicao;	
} Processo;

void ordenarPorId(Processo *p, int n);
void ordenarPorData(Processo *p, int n);
int contadorDeIdClasse(Processo *p, int n, const char *id_classe);
int contadorDeIdAssuntos(Processo *p, int n);
void listarProcessosMultiplosAssuntos(Processo *p, int n);
void diasDeTramitacao(Processo *p, int n);

#endif