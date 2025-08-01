#include <stdio.h>
#include <stdlib.h>
#include "FilaCircular.h"

FilaCircular *criarFila(int capacidade)
{
    FilaCircular *fila = malloc(sizeof(FilaCircular));
    if (fila == NULL)
        return NULL;

    fila->dados = malloc(capacidade * sizeof(int));
    if (fila->dados == NULL)
    {
        free(fila);
        return NULL;
    }

    fila->inicio = 0;
    fila->fim = 0;
    fila->capacidade = capacidade;

    return fila;
}

bool filaVazia(FilaCircular *fila)
{
    return fila->inicio == fila->fim;
}

bool filaCheia(FilaCircular *fila)
{
    return (fila->fim - fila->inicio) == fila->capacidade;
}

bool enfileirar(FilaCircular *fila, int valor)
{
    if (filaCheia(fila))
        return false;
    fila->dados[fila->fim % fila->capacidade] = valor;
    fila->fim++;
    return true;
}

bool desenfileirar(FilaCircular *fila)
{
    if (filaVazia(fila))
        return false;
    fila->inicio++;
    return true;
}

int primeiro(FilaCircular *fila)
{
    if (filaVazia(fila))
        return -1;
    return fila->dados[fila->inicio % fila->capacidade];
}

int tamanhoFila(FilaCircular *fila)
{
    return (fila->fim - fila->inicio);
}

void imprimirFila(FilaCircular *fila)
{
    printf("Fila: ");
    for (int i = fila->inicio; i < fila->fim; i++)
    {
        printf("%d ", fila->dados[i % fila->capacidade] + 1);
    }
    printf("\n");
}

void liberarFila(FilaCircular *fila)
{
    if (fila != NULL)
    {
        free(fila->dados);
        free(fila);
    }
}
