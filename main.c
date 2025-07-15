#include <stdio.h>
#include "processo.h"
#include "modulo1.c"
#include "SJF.c"
#include "RR.c"

// Declaração das funções do módulo 1
int gerar_processos(Processo[], int);
void imprimir_processos(Processo[], int);
void simular_SJF(Processo[], Processo[], int n);
void simular_RR(Processo processos[], int n, int quantum);

int main()
{
    Processo lista[20];

    int total = gerar_processos(lista, 5);
    imprimir_processos(lista, total);

    //simular_RR(lista, total, 2);



    return 0;
}
