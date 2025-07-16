#include "escalonador.h"
#include "FilaCircular.h"
#include <stdio.h>
#include "exporta_json.h"
#include <stdlib.h>
#include <string.h>

void simular_escalonamento(Processo processos[], int n, TipoAlgoritmo algoritmo, ParamsAlgoritmo params, const char *nome_arquivo)
{
    int tempo = 0, finalizados = 0;
    int fila_prontos[MAX], n_prontos;
    ExecucaoAlgoritmo exec = {-1, 0};
    Processo *p = NULL;
    FilaCircular *fila_circular = NULL;

    if (algoritmo == RR || algoritmo == FIFO)
    {
        fila_circular = criarFila(n);
        if (fila_circular == NULL)
        {
            printf("Erro ao criar a fila circular.\n");
            return;
        }
    }

    // para construir GUI
    int timeline[MAX_TEMPO];
    int t_tick = 0;

    const char *estado_str[] = {"Pronto", "Executando", "Finalizado"};

    if (algoritmo == LOTTERY)
        srand(params.seed);

    printf("\n=== Simulação %s ===\n",
           algoritmo == FIFO ? "FIFO" : algoritmo == SJF      ? "SJF"
                                    : algoritmo == PRIORIDADE ? "Prioridade"
                                    : algoritmo == RR         ? "Round Robin"
                                                              : "Lottery");

    while (finalizados < n)
    {
        // Monta fila de prontos
        if (algoritmo == RR || algoritmo == FIFO)
        {
            // Enfileira novos processos na fila circular
            for (int i = 0; i < n; i++)
            {
                if (processos[i].tempo_chegada == tempo)
                {
                    enfileirar(fila_circular, i);
                }
            }
        }
        else
        {
            // Atualiza lista de prontos normal
            n_prontos = 0;
            for (int i = 0; i < n; i++)
            {
                if (processos[i].tempo_chegada <= tempo &&
                    processos[i].estado != FINALIZADO &&
                    processos[i].tempo_restante > 0)
                {
                    fila_prontos[n_prontos++] = i;
                }
            }
            if (n_prontos == 0)
            {
                tempo++;
                continue;
            }
        }

        if (exec.duracao == 0)
        {
            if ((algoritmo == RR && !filaVazia(fila_circular)) 
                || (algoritmo == FIFO && !filaVazia(fila_circular)))
            {
                exec = algoritmo_escalonamento(processos, n, NULL, 0, algoritmo, params, 0, fila_circular);
                desenfileirar(fila_circular);
            }
            else if (algoritmo != RR && algoritmo != FIFO)
            {
                exec = algoritmo_escalonamento(processos, n, fila_prontos, n_prontos, algoritmo, params, 0, NULL);
            }
            else
            {
                tempo++;
                continue;
            }

            p = &processos[exec.idx_processo];

            // Atualiza estados
            for (int i = 0; i < n; i++)
                if (processos[i].estado == EXECUTANDO)
                    processos[i].estado = PRONTO;

            p->estado = EXECUTANDO;
        }

        // Executa 1 tick
        p->tempo_restante--;
        exec.duracao--;

        // Atualiza espera dos outros prontos no tick atual
        if(algoritmo == RR || algoritmo == FIFO)
        {
        // atualiza os tempos de espera dos outros processos prontos
            for (int i = 0; i < n; i++)
            {
                if (i != exec.idx_processo && processos[i].tempo_chegada <= tempo 
                    && processos[i].estado != FINALIZADO)
                {
                    processos[i].tempo_espera++;
                }
            }
        }
        else
        {
            for (int i = 0; i < n_prontos; i++)
                if (fila_prontos[i] != exec.idx_processo && processos[fila_prontos[i]].estado != FINALIZADO)
                    processos[fila_prontos[i]].tempo_espera++;
        }

        printf("Tempo %2d: Processo %d em execução (restante %d, estado: %s)\n",
               tempo, p->pid, p->tempo_restante, estado_str[p->estado]);
        if(algoritmo == RR){
            imprimirFila(fila_circular);
        }

        tempo++;
        // Salva o PID executando nesse tick
        timeline[t_tick++] = p->pid;

        // Finalizou?
        if (algoritmo == RR)
        {
            if (p->tempo_restante == 0)
            {
                p->estado = FINALIZADO;
                finalizados++;
                exec.duracao = 0;
            }
            else if (exec.duracao == 0)
            {
                enfileirar(fila_circular, exec.idx_processo);
                p->estado = PRONTO;
            }
        }
        else
        {
            if (p->tempo_restante == 0)
            {
                p->estado = FINALIZADO;
                finalizados++;
                exec.duracao = 0;
            }
        }
    }

    if (fila_circular != NULL) liberarFila(fila_circular);
    
    // salvando json
    salvar_json(processos, n, algoritmo, params, nome_arquivo, timeline, t_tick);
}
