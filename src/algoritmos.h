#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "processo.h"     // struct Processo

typedef enum { FIFO, SJF, PRIORIDADE, LOTTERY } TipoAlgoritmo;

typedef struct {
    int quantum;
    int *tickets;
    unsigned seed;
} ParamsAlgoritmo;

// retorno do algoritmo
typedef struct {
    int idx_processo; // índice do processo escolhido
    int duracao;      // por quanto tempo vai executar
} ExecucaoAlgoritmo;

// algoritmo de escalonamento
ExecucaoAlgoritmo algoritmo_escalonamento(
    Processo processos[], int n, int *fila, int n_prontos,
    TipoAlgoritmo algoritmo, ParamsAlgoritmo params, int tempo_executado);

#endif
