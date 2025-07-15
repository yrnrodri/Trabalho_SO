#include "escalonador.h"
#include <stdio.h>
#include "exporta_json.h"
#include <stdlib.h>
#include <string.h>

void simular_escalonamento(Processo processos[], int n, TipoAlgoritmo algoritmo, ParamsAlgoritmo params, const char* nome_arquivo) {
    int tempo = 0, finalizados = 0;
    int fila_prontos[MAX], n_prontos;

    // para construir GUI 
    int timeline[MAX_TEMPO]; 
    int t_tick = 0;

    const char* estado_str[] = {"Pronto", "Executando", "Finalizado"};

    if (algoritmo == LOTTERY)
        srand(params.seed);

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
            // Salva o PID executando nesse tick
            timeline[t_tick++] = p->pid; 
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

    // salvando json
    salvar_json(processos, n, algoritmo, params, nome_arquivo, timeline, t_tick);
}
