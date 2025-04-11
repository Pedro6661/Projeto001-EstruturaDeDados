#include "processo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAX_LINHAS 18393
#define MAX_CAMPOS 10
#define TAM_CAMPO 256

void limparMultivalorado(char *campo) {
    char temp[100];
    int j = 0;

    for (int i = 0; campo[i] != '\0'; i++) {
        char c = campo[i];
        if (c != '{' && c != '}' && c != '"' /*&& c != ','*/) {
            temp[j++] = c;
        }
    }

    temp[j] = '\0';
    strcpy(campo, temp);
}
//vai ler o arquivo sem as ""
//vai retornar o index da coluna tal tratada
//Parsing: solução encontrada lendo e estudando manual C e foruns stackoverflow para tratamento de linhas de string complexas
int parseCSV(char *linha, char campos[][TAM_CAMPO], int max_campos) {
    int index = 0;
    char *cursor = linha;

    while (*cursor && index < max_campos) {
        //percorre o indice, a coluna e cada caracter dela com o cursosr
        if (*cursor == '"') {
            cursor++;// se o cursor encontrar as aspas ele pula as aspas
            char *inicioCampo = cursor; //inicio de campo agr tem os valores entre as aspas
            
            int tamanho = 0; 
            //procura a proxima aspas para pular
            while (*cursor && *cursor != '"') {
                cursor++;
                tamanho++;// contador de caracteres
            }
            // copia inicio de campo para campos index, e o numero de caracteres a serem copiados
            strncpy(campos[index], inicioCampo, tamanho);
            campos[index][tamanho] = '\0';

            //Após terminar o campo, se o cursor está numa aspa ("), pula essa aspa de fechamento.
            if (*cursor == '"') cursor++;
            if (*cursor == ',') cursor++;
        } else {
            char *inicioCampo = cursor;
            int tamanho = 0;

            while (*cursor && *cursor != ',') {
                cursor++;
                tamanho++;
            }

            strncpy(campos[index], inicioCampo, tamanho);
            campos[index][tamanho] = '\0';

            if (*cursor == ',') cursor++;
        }

        index++;
    }

    return index;
}

//função para ler o CSV
Processo *LerDados(const char *nomeArquivo, int *n) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (fp == NULL) {
        printf("ERRO: O arquivo não pode ser aberto!\n");
        exit(1);
    }

    Processo *processo = malloc(MAX_LINHAS * sizeof(Processo));
    if (processo == NULL) {
        printf("ERRO: Não foi possível alocar memória para a tabela!\n");
        exit(1);
    }

    char linha[512];
    int i = 0;

    if (fgets(linha, sizeof(linha), fp) == NULL) {
        fclose(fp);
        free(processo);
        return NULL;
    }

    while (fgets(linha, sizeof(linha), fp) && i < MAX_LINHAS) {
        linha[strcspn(linha, "\n")] = '\0';

        char campos[6][256] = {{0}};
        int qtd = parseCSV(linha, campos, 6);
        // depois de ler os valores sem aspas ele vai devolvolver e copiar no struct
        if (qtd == 6) {
            processo[i].id = atoi(campos[0]);
            strncpy(processo[i].numProcesso, campos[1], sizeof(processo[i].numProcesso));
            strncpy(processo[i].data_ajuizamento, campos[2], sizeof(processo[i].data_ajuizamento));
            strncpy(processo[i].id_classe, campos[3], sizeof(processo[i].id_classe));
            strncpy(processo[i].id_assunto, campos[4], sizeof(processo[i].id_assunto));
            processo[i].ano_eleicao = atoi(campos[5]);

            limparMultivalorado(processo[i].id_classe);
            limparMultivalorado(processo[i].id_assunto);

            i++;
        }
    }

    fclose(fp);
    *n = i;
    return processo;
}
//1. Ordenar, em ordem crescente, o conjunto de dados a partir do atributo “id”;
void swap(Processo *a, Processo *b) {
    Processo tmp = *a;
    *a = *b;
    *b = tmp;
}

int compararPorId(Processo *a, Processo *b) {
    return a->id - b->id;
}

void quicksort(Processo processos[], int inf, int sup, int (*comparar)(Processo *, Processo *)) {
    int i = inf, j = sup;
    Processo pivo = processos[(inf + sup) / 2];

    while (i <= j) {
        while (comparar(&processos[i], &pivo) < 0) i++;
        while (comparar(&processos[j], &pivo) > 0) j--;
        if (i <= j) {
            swap(&processos[i], &processos[j]);
            i++; j--;
        }
    }
    if (inf < j) quicksort(processos, inf, j, comparar);
    if (i < sup) quicksort(processos, i, sup, comparar);
}

void ordenarPorId(Processo processos[], int n) {
    quicksort(processos, 0, n - 1, compararPorId);
}

//2. Ordenar, em ordem decrescente, o conjunto de dados a partir do atributo “data_ajuizamento”;
void ordenarPorData(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(processos[i].data_ajuizamento, processos[j].data_ajuizamento) < 0) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }
}

//Salvar em csv 1 e 2
void salvarCSV(const char *nomeArquivo, Processo *vet, int n) {
    FILE *fp = fopen(nomeArquivo, "w");
    if (!fp) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }

    fprintf(fp, "id,numProcesso,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d,\"%s\",%s,{%s},{%s},%d\n",
                vet[i].id,
                vet[i].numProcesso,
                vet[i].data_ajuizamento,
                vet[i].id_classe,
                vet[i].id_assunto,
                vet[i].ano_eleicao);
    }

    fclose(fp);
    printf("Dados salvos com sucesso em %s\n", nomeArquivo);
}

//3. Contar quantos processos estão vinculados a um determinado “id_classe”;
void ContarPorClasse(Processo processos[], int n) {
    char idsAgrupados[5000][50]; // Vetor local de id_classe únicos
    int contadores[5000] = {0};  // Contador de ocorrências
    int totalGrupos = 0;

    for (int i = 0; i < n; i++) {
        int encontrado = 0;

        for (int j = 0; j < totalGrupos; j++) {
            if (strcmp(processos[i].id_classe, idsAgrupados[j]) == 0) {
                contadores[j]++;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            strcpy(idsAgrupados[totalGrupos], processos[i].id_classe);
            contadores[totalGrupos] = 1;
            totalGrupos++;
        }
    }
    printf("_________________________________\n");
    printf("\nContagem de processos por id_classe:\n");
    printf("_________________________________\n");
    for (int i = 0; i < totalGrupos; i++) {
        printf("| id_classe: %s tem: %d processos\n", idsAgrupados[i], contadores[i]);
        
    }

}

//4. Identificar quantos “id_assuntos” constam nos processos presentes na base de dados;
void ContarPorAssunto(Processo processos[], int n) {
    char idsAgrupados[5000][50]; // Vetor local de id_assunto únicos
    int contadores[5000] = {0};  // Contador de ocorrências
    int totalGrupos = 0;

    for (int i = 0; i < n; i++) {
        int encontrado = 0;

        for (int j = 0; j < totalGrupos; j++) {
            if (strcmp(processos[i].id_assunto, idsAgrupados[j]) == 0) {
                contadores[j]++;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            strcpy(idsAgrupados[totalGrupos], processos[i].id_assunto);
            contadores[totalGrupos] = 1;
            totalGrupos++;
        }
    }
    printf("_________________________________________\n");
    printf("\nContagem de 'id_assuntos' na base de dados:\n");
    printf("_________________________________________\n");
    for (int i = 0; i < totalGrupos; i++) {
        printf("| id_assunto: %s aparece: %d vezes\n", idsAgrupados[i], contadores[i]);
    }
    
}

//5. Listar todos os processos que estão vinculados a mais de um assunto;
void ListarProcessosComMultiplosAssuntos(Processo processos[], int n) {
    printf("_________________________________\n");
    printf("\nProcessos vinculados a mais de um assunto:\n");
    printf("_________________________________\n");

    for (int i = 0; i < n; i++) {
        // Verifica se o campo id_assunto contém mais de um assunto
        if (strchr(processos[i].id_assunto, ',') != NULL) {
            printf("ID: %d | Numero: %s | Assuntos: %s\n",
                   processos[i].id,
                   processos[i].numProcesso,
                   processos[i].id_assunto);
        }
    }

}

//6. Indicar a quantos dias um processo está em tramitação na justiça
void mostrarDiasTramitado(Processo processos[], int n) {
    if (n <= 0) {
        printf("Nenhum processo carregado.\n");
        return;
    }
    else{
        printf("\n_________________________________\n");
        printf("\nDias de Tramitacao por Processo:\n");
        printf("Limitado para 15 linhas para melhor visualizacao via terminal\n");
        printf("\n_________________________________\n");
        
        for (int i = 0; i < 15; i++) {
        struct tm data_ajuizamento = {0};
        int ano, mes, dia, hora, minuto, segundo;

        sscanf(processos[i].data_ajuizamento, "%d-%d-%d %d:%d:%d",
               &ano, &mes, &dia, &hora, &minuto, &segundo);

        data_ajuizamento.tm_year = ano - 1900;
        data_ajuizamento.tm_mon = mes - 1;
        data_ajuizamento.tm_mday = dia;
        data_ajuizamento.tm_hour = hora;
        data_ajuizamento.tm_min = minuto;
        data_ajuizamento.tm_sec = segundo;

        time_t agora = time(NULL);
        time_t t1 = mktime(&data_ajuizamento);
        time_t t2 = agora;

        double segundos = difftime(t2, t1);
        int dias = segundos / (60 * 60 * 24);

        printf("ID %d esta em tramitacao ha %d dias.\n", processos[i].id, dias);
    }
}
}


//----


