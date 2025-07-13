#include <stdio.h>
#include "processo.h"
#include "modulo1.c"
#include "SJF.c"

// Declaração das funções do módulo 1
int gerar_processos(Processo[], int);
void imprimir_processos(Processo[], int);

int main()
{
    Processo lista[20];
    int total = gerar_processos(lista, 5);
    imprimir_processos(lista, total);

    return 0;
}
