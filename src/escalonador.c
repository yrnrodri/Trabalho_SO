#include "escalonador.h"
#include <stdio.h>
#include "exporta_json.h"
#include <stdlib.h>
#include <string.h>

void simular_escalonamento(Processo processos[], int n, TipoAlgoritmo algoritmo, ParamsAlgoritmo params, const char* nome_arquivo) {
    int tempo = 0, finalizados = 0;
    int fila_prontos[MAX], n_prontos;
    ExecucaoAlgoritmo exec = {-1,0};
    Processo *p = NULL;

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

        if (exec.duracao == 0){
            // Seleciona processo a ser executado
            exec = algoritmo_escalonamento(
            processos, n, fila_prontos, n_prontos, algoritmo, params, 0);

            // processo em execucao
            p = &processos[exec.idx_processo];

            // Atualiza estados
            for (int i = 0; i < n; i++) {
                if (processos[i].estado == EXECUTANDO)
                    processos[i].estado = PRONTO;
            }
            p->estado = EXECUTANDO;
        }

        // Executa 1 tick
        p->tempo_restante--;
        exec.duracao--;

        // Atualiza espera dos outros prontos no tick atual
        for (int i = 0; i < n_prontos; i++)
            if (fila_prontos[i] != exec.idx_processo && processos[fila_prontos[i]].estado != FINALIZADO)
                processos[fila_prontos[i]].tempo_espera++;
        tempo++;
        printf("Tempo %2d: Processo %d em execução (restante %d, estado: %s)\n", 
                tempo, p->pid, p->tempo_restante, estado_str[p->estado]);
        // Salva o PID executando nesse tick
        timeline[t_tick++] = p->pid; 

        // Finalizou?
        if (p->tempo_restante == 0 && p->estado != FINALIZADO) {
            p->estado = FINALIZADO;
            finalizados++;
            exec.duracao = 0;
        }
    }

    // salvando json
    salvar_json(processos, n, algoritmo, params, nome_arquivo, timeline, t_tick);
}
