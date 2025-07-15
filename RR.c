#include "processo.h"
#include "FilaCircular.c"
#include <string.h>

void simular_RR(Processo processos[], int n, int quantum)
{
    int tempo = 0;
    int finalizados = 0;
    int processo_em_exec = -1;
    int tempo_executado = 0;

    FilaCircular *fila = criarFila(n);
    if (fila == NULL)
    {
        printf("Erro ao criar a fila.\n");
        return;
    }

    while (finalizados < n)
    {
        printf("Tempo = %d\n", tempo);
        // coloca os processos que chegaram agora na fila
        for (int i = 0; i < n; i++)
        {
            if (processos[i].tempo_chegada == tempo && processos[i].finalizado == 0)
            {
                enfileirar(fila, i);
                printf("Fila depois que chega um processo: \n");
                imprimirFila(fila);
            }
        }

        // se nao tiver nenhum processo em execucao, pega o primeiro da fila
        if (processo_em_exec == -1 && !filaVazia(fila))
        {
            processo_em_exec = primeiro(fila);
            desenfileirar(fila);                // removo da fila
            tempo_executado = 0;
            strcpy(processos[processo_em_exec].estado, "Em execucao");
        }

        // atualiza os tempos de espera dos outros processos prontos
        for (int i = 0; i < n; i++)
        {
            if (i != processo_em_exec && processos[i].tempo_chegada <= tempo 
                && processos[i].finalizado == 0)
            {
                processos[i].tempo_espera++;
            }
        }

        // executa 1 unidade de tempo
        if (processo_em_exec != -1)
        {
            processos[processo_em_exec].tempo_restante--;
            tempo_executado++;

            printf("Processo com ID = %d executado; Tempo restante do processo = %d\n",
                 processo_em_exec + 1, processos[processo_em_exec].tempo_restante);

            // se terminou
            if (processos[processo_em_exec].tempo_restante == 0)
            {
                processos[processo_em_exec].finalizado = 1;
                strcpy(processos[processo_em_exec].estado, "Finalizado");
                //desenfileirar(fila);
                finalizados++;
                processo_em_exec = -1;
            }
            // se atingiu o quantum mas ainda não terminou
            else if (tempo_executado == quantum)
            {
                strcpy(processos[processo_em_exec].estado, "Pronto");
                enfileirar(fila, processo_em_exec); // insiro mais uma vez, assim ele volta pro fim da fila
                printf("Fila depois de terminar um quantum: \n");
                imprimirFila(fila);
                processo_em_exec = -1;
            }
        }

        tempo++;
    }

    printf("\nResultados Round Robin (quantum = %d):\n", quantum);
    printf("PID | Espera | Turnaround\n");
    for (int i = 0; i < n; i++)
    {
        int turnaround = processos[i].tempo_espera + processos[i].tempo_execucao;
        printf(" %2d |   %2d   |     %2d\n", processos[i].pid, processos[i].tempo_espera, turnaround);
    }
}
