#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Para compilar, no prompt de comando, deve-se digitar gcc processo.c main.c -o processo.exe. A seguir, paraexecutar, digite ./processo.exe ou ./processo

int main(){
	Processo *Dados = LerDados("processo_043_202409032338.csv");
		for (int i = 0; i < 20; i++) {
			printf("%d\t%s\t%s\t%s\t%s\t%d\n",
				Dados[i].id,
				Dados[i].numProcesso,
				Dados[i].data_ajuizamento,
				Dados[i].id_classe,
				Dados[i].id_assunto,
				Dados[i].ano_eleicao
			);
		}
		return 0;
}
