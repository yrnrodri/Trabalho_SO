#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "escalonador.h"
#include "modulo1.h"

#include "exporta_json.h"

int carregar_processos_do_json(const char* nome_arquivo, Processo processos[], int max) {
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) return 0;
    int n = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), f)) {
        int pid, chegada, execucao, prioridade;
        if (sscanf(linha, " {\"pid\": %d, \"chegada\": %d, \"execucao\": %d, \"prioridade\": %d}", &pid, &chegada, &execucao, &prioridade) == 4) {
            processos[n].pid = pid;
            processos[n].tempo_chegada = chegada;
            processos[n].tempo_execucao = execucao;
            processos[n].prioridade = prioridade;
            processos[n].tempo_restante = execucao;
            processos[n].tempo_espera = 0;
            processos[n].estado = PRONTO;
            processos[n].finalizado = 0;
            n++;
            if (n >= max) break;
        }
    }
    fclose(f);
    return n;
}

int main(int argc, char *argv[])
{
    Processo originais[MAX];
    int n = 5, op;
    char *tipo = "--fixed";
    char *algoritmo = NULL;

    if (argc > 1) tipo = argv[1];
    if (argc > 2) algoritmo = argv[2];

    if (!algoritmo) {
        // GERA processos originais só se não estiver rodando um algoritmo!
        op = (strcmp(tipo, "--random") == 0) ? 0 : 1;
        n = gerar_processos(originais, n, op, "results/dados_originais.json");
        printf("Processos %s gerados!\n", op == 0 ? "aleatórios" : "fixos");
        imprimir_processos_iniciais(originais, n);
        return 0;
    }

    // --- NOVO: carregar os processos originais do arquivo antes de rodar algoritmo ---
    n = carregar_processos_do_json("results/dados_originais.json", originais, MAX);
    if (n == 0) {
        fprintf(stderr, "Erro: Não foi possível carregar os processos originais!\n");
        return 1;
    }

    ParamsAlgoritmo params;
    Processo copia[MAX];


    if (strcmp(algoritmo, "FIFO") == 0) {
        memcpy(copia, originais, sizeof(Processo)*n);
        params.quantum = 0; params.tickets = NULL; params.seed = 0;
        simular_escalonamento(copia, n, FIFO, params, "results/fifo.json");

    } else if (strcmp(algoritmo, "SJF") == 0) {
        memcpy(copia, originais, sizeof(Processo)*n);
        simular_escalonamento(copia, n, SJF, params, "results/sjf.json");

    } else if (strcmp(algoritmo, "PRIORIDADE") == 0) {
        memcpy(copia, originais, sizeof(Processo)*n);
        params.quantum = 2;
        simular_escalonamento(copia, n, PRIORIDADE, params, "results/prioridade.json");

    } else if (strcmp(algoritmo, "LOTTERY") == 0) {
        memcpy(copia, originais, sizeof(Processo)*n);
        int tickets[MAX] = {1,2,3,1,2};
        params.tickets = tickets; params.seed = (unsigned)time(NULL);
        simular_escalonamento(copia, n, LOTTERY, params, "results/lottery.json");

    } else {
        fprintf(stderr, "Algoritmo desconhecido: %s\n", algoritmo);
        return 1;
    }

    return 0;
}
