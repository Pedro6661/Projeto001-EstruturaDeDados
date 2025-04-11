#ifndef PROCESSO_H
#define PROCESSO_H


typedef struct {
    int id;
    char numProcesso[100];
    char data_ajuizamento[100];
    char id_classe[50];
    char id_assunto[50];
    char qtd_assuntos[30];
    int ano_eleicao;
} Processo;

Processo *LerDados(const char *nomeArquivo, int *n);
void limparMultivalorado(char *campo);
void ordenarPorId(Processo processos[], int n);
void ordenarPorData(Processo processos[], int n);
void salvarCSV(const char *nomeArquivo, Processo *vet, int n);
void ContarPorClasse(Processo processos[], int n);
void ContarPorAssunto(Processo processos[], int n);
void ListarProcessosComMultiplosAssuntos(Processo processos[], int n);
void mostrarDiasTramitado(Processo processos[], int n);

#endif
