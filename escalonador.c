#include "escalonador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void simular_escalonamento(Processo processos[], int n, TipoAlgoritmo algoritmo, ParamsAlgoritmo params) {
    int tempo = 0, finalizados = 0;
    int fila_prontos[MAX], n_prontos;

    const char* estado_str[] = {"Pronto", "Executando", "Finalizado"};

    if (algoritmo == LOTTERY)
        srand(params.seed);

    // Inicializa processos
    for (int i = 0; i < n; i++) {
        processos[i].tempo_restante = processos[i].tempo_execucao;
        processos[i].tempo_espera = 0;
        processos[i].estado = PRONTO;
    }

    printf("\n=== Simulação %s ===\n", 
        algoritmo==FIFO?"FIFO":
        algoritmo==SJF?"SJF":
        algoritmo==PRIORIDADE?"Prioridade":"Lottery");

    while (finalizados < n) {
        // Monta fila de prontos
        n_prontos = 0;
        for (int i = 0; i < n; i++) {
            if (processos[i].tempo_chegada <= tempo 
                && processos[i].estado != FINALIZADO
                && processos[i].tempo_restante > 0)
                fila_prontos[n_prontos++] = i;
        }
        if (n_prontos == 0) { tempo++; continue; }

        // Seleciona processo a ser executado
        ExecucaoAlgoritmo exec = algoritmo_escalonamento(
            processos, n, fila_prontos, n_prontos, algoritmo, params, 0);

        // processo em execucao
        Processo *p = &processos[exec.idx_processo];

        // Atualiza estados
        for (int i = 0; i < n; i++) {
            if (processos[i].estado == EXECUTANDO)
                processos[i].estado = PRONTO;
        }
        p->estado = EXECUTANDO;

        // Executa pelo tempo definido pelo algoritmo
        for (int t = 0; t < exec.duracao; t++) {
            p->tempo_restante--;
            // Atualiza espera dos outros prontos
            for (int i = 0; i < n_prontos; i++)
                if (fila_prontos[i] != exec.idx_processo && processos[fila_prontos[i]].estado != FINALIZADO)
                    processos[fila_prontos[i]].tempo_espera++;
            tempo++;
            printf("Tempo %2d: Processo %d em execução (restante %d, estado: %s)\n", 
                   tempo, p->pid, p->tempo_restante, estado_str[p->estado]);
            if (p->tempo_restante == 0) break;
        }

        // Finalizou?
        if (p->tempo_restante == 0 && p->estado != FINALIZADO) {
            p->estado = FINALIZADO;
            finalizados++;
        } else if (p->estado == EXECUTANDO) {
            p->estado = PRONTO; // voltou para pronto se foi preemptado
        }
    }

    if (algoritmo == LOTTERY) {
        printf("PID | Espera | Turnaround | Estado final | Tickets\n");
        for (int i = 0; i < n; i++) {
            int turnaround = processos[i].tempo_espera + processos[i].tempo_execucao;
            printf(" %2d |   %2d   |     %2d    | %s |   %d\n", 
                processos[i].pid, processos[i].tempo_espera, turnaround, 
                estado_str[processos[i].estado], params.tickets[i]);
        }
    } else {
        printf("PID | Espera | Turnaround | Estado final\n");
        for (int i = 0; i < n; i++) {
            int turnaround = processos[i].tempo_espera + processos[i].tempo_execucao;
            printf(" %2d |   %2d   |     %2d    | %s\n", 
                processos[i].pid, processos[i].tempo_espera, turnaround, 
                estado_str[processos[i].estado]);
        }
    }

}
