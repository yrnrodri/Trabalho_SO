#include "modulo1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_PROCESSOS 100


int gerar_processos(Processo processos[], int n, int op, const char* nome_arquivo)
{
    if (op == 0){
        srand(time(NULL));
        for (int i = 0; i < n; i++) {
            processos[i].pid = i + 1;
            processos[i].tempo_chegada = rand() % 10;
            processos[i].tempo_execucao = 1 + rand() % 10;
            processos[i].prioridade = 1 + rand() % 4;
        }
    } else {
        for (int i = 0; i < n; i++) {
            processos[i].pid = i + 1;
            processos[i].tempo_chegada = i * 2;
            processos[i].tempo_execucao = 2 + i;
            processos[i].prioridade = 1 + (i % 3);
        }
    }
    // Inicializa os campos adicionais para ambos os casos
    for (int i = 0; i < n; i++) {
        processos[i].tempo_restante = processos[i].tempo_execucao;
        processos[i].tempo_espera = 0;
        processos[i].estado = PRONTO;
        processos[i].finalizado = 0;
    }
    salvar_json_processos_iniciais(processos, n, nome_arquivo);
    return n;
}


// Imprime todos os processos
void imprimir_processos_iniciais(Processo processos[], int n) {
    printf("PID | Tempo de Chegada | Tempo de Execucao | Prioridade\n");
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf(" %2d |        %2d        |        %2d         |     %2d\n",
               processos[i].pid,
               processos[i].tempo_chegada,
               processos[i].tempo_execucao,
               processos[i].prioridade);
    }
}


