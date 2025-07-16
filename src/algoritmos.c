#include "algoritmos.h"
#include <stdio.h>
#include <stdlib.h>

ExecucaoAlgoritmo algoritmo_escalonamento(
    Processo processos[], int n, int *fila, int n_prontos,
    TipoAlgoritmo algoritmo, ParamsAlgoritmo params, int tempo_executado, FilaCircular *filaa)
{
    ExecucaoAlgoritmo exec;
    exec.idx_processo = -1;
    exec.duracao = 1;

    switch(algoritmo) {
        case FIFO:
            exec.idx_processo = primeiro(filaa);
            exec.duracao = processos[exec.idx_processo].tempo_restante;
            break;
        case SJF: {
            int min = 100000;
            for (int i = 0; i < n_prontos; i++) {
                if (processos[fila[i]].tempo_execucao < min) {
                    min = processos[fila[i]].tempo_execucao;
                    exec.idx_processo = fila[i];
                }
            }
            exec.duracao = processos[exec.idx_processo].tempo_restante;
            break;
        }
        case PRIORIDADE: {
            int max = -1;
            for (int i = 0; i < n_prontos; i++) {
                if (processos[fila[i]].prioridade > max) {
                    max = processos[fila[i]].prioridade;
                    exec.idx_processo = fila[i];
                }
            }
            int resto = processos[exec.idx_processo].tempo_restante;
            exec.duracao = (params.quantum < resto) ? params.quantum : resto;
            break;
        }
        case LOTTERY: {
            int total_tickets = 0;
            // soma todos os tickets dos processos prontos
            for (int i = 0; i < n_prontos; i++)
                total_tickets += params.tickets[fila[i]];

            // se não tiver tickets, executa o primeiro
            if (total_tickets == 0) { exec.idx_processo = fila[0]; exec.duracao = 1; break; }

            // sorteia um número entre 0 e total de tickets-1
            int sorteio = rand() % total_tickets;
            // percorre a fila de prontos acumulando tickets até encontrar quem ganhou
            for (int i = 0, acc = 0; i < n_prontos; i++) {
                acc += params.tickets[fila[i]];
                // processo sorteado é escolhido
                if (sorteio < acc) {
                    exec.idx_processo = fila[i];
                    break;
                }
            }
            exec.duracao = 1; // sempre 1 tick por sorteio, logo um novo sorteio por tick
            break;
        }
        case RR: {
            exec.idx_processo = primeiro(filaa);
            exec.duracao = processos[exec.idx_processo].tempo_restante;
            

            int resto = processos[exec.idx_processo].tempo_restante;
            exec.duracao = (params.quantum < resto) ? params.quantum : resto;
            break;
        }
        case SRTN: {
            int min = 100000;
            for (int i = 0; i < n_prontos; i++) {
                if (processos[fila[i]].tempo_restante < min) {
                    min = processos[fila[i]].tempo_restante;
                    exec.idx_processo = fila[i];
                }
            }
            exec.duracao = 1;
            break;
        }
    }
    return exec;
}
