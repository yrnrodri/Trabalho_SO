#ifndef FILA_CIRCULAR_H
#define FILA_CIRCULAR_H

#include <stdbool.h>

// Estrutura da fila circular
typedef struct
{
    int *dados;     // Vetor alocado dinamicamente
    int inicio;     // Índice absoluto do primeiro elemento
    int fim;        // Índice absoluto do próximo elemento
    int capacidade; // Capacidade da fila
} FilaCircular;

// Interface das funções
FilaCircular *criarFila(int capacidade);
bool filaVazia(FilaCircular *fila);
bool filaCheia(FilaCircular *fila);
bool enfileirar(FilaCircular *fila, int valor);
bool desenfileirar(FilaCircular *fila);
int primeiro(FilaCircular *fila);
int tamanhoFila(FilaCircular *fila);
void imprimirFila(FilaCircular *fila);
void liberarFila(FilaCircular *fila);

#endif // FILA_CIRCULAR_H
