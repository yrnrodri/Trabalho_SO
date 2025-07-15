#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "escalonador.h"
#include "modulo1.h"

int main() {
    Processo originais[MAX];
    int n = 5;

    // Exemplo: preencher manualmente
    for (int i = 0; i < n; i++) {
        originais[i].pid = i+1;
        originais[i].tempo_chegada = i*2;
        originais[i].tempo_execucao = 2+i;
        originais[i].prioridade = 1 + (i%3);
    }

    // n = gerar_processos(originais, 5);

    imprimir_processos_iniciais(originais, n);

    ParamsAlgoritmo params;
    Processo copia[MAX];

    // FIFO
    memcpy(copia, originais, sizeof(Processo)*n);
    params.quantum = 0;
    params.tickets = NULL;
    params.seed = 0;
    simular_escalonamento(copia, n, FIFO, params, "results/fifo.json");


    // SJF
    memcpy(copia, originais, sizeof(Processo)*n);
    simular_escalonamento(copia, n, SJF, params, "results/sjf.json");

    // PRIORIDADE
    memcpy(copia, originais, sizeof(Processo)*n);
    params.quantum = 2;
    simular_escalonamento(copia, n, PRIORIDADE, params, "results/prioridade.json");

    // LOTTERY
    memcpy(copia, originais, sizeof(Processo)*n);
    int tickets[MAX] = {1, 2, 3, 1, 2};
    params.tickets = tickets;
    params.seed = (unsigned) time(NULL);
    simular_escalonamento(copia, n, LOTTERY, params, "results/lottery.json");

    return 0;
}