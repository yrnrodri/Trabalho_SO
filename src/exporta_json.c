#include "exporta_json.h"
#include <stdio.h>

void salvar_json(Processo processos[], int n, TipoAlgoritmo algoritmo, ParamsAlgoritmo params, const char *nome_arquivo, int timeline[], int timeline_len)
{
    const char *estado_str[] = {"Pronto", "Executando", "Finalizado"};
    FILE *f = fopen(nome_arquivo, "w");
    if (!f)
    {
        printf("Erro ao abrir arquivo %s para escrita!\n", nome_arquivo);
        return;
    }
    fprintf(f, "{\n  \"algoritmo\": \"%s\",\n",
            algoritmo == FIFO ? "FIFO" : algoritmo == SJF      ? "SJF"
                                     : algoritmo == PRIORIDADE ? "Prioridade"
                                                               : "Lottery");

    if (algoritmo == LOTTERY)
    {
        fprintf(f, "  \"tickets\": [");
        for (int i = 0; i < n; i++)
            fprintf(f, "%d%s", params.tickets[i], i < n - 1 ? ", " : "");
        fprintf(f, "],\n");
    }

    fprintf(f, "  \"processos\": [\n");
    for (int i = 0; i < n; i++)
    {
        int turnaround = processos[i].tempo_espera + processos[i].tempo_execucao;
        fprintf(f, "    {\"pid\": %d, \"chegada\": %d, \"execucao\": %d, \"prioridade\": %d, \"espera\": %d, \"turnaround\": %d, \"estado\": \"%s\"}%s\n",
                processos[i].pid,
                processos[i].tempo_chegada,
                processos[i].tempo_execucao,
                processos[i].prioridade,
                processos[i].tempo_espera,
                turnaround,
                estado_str[processos[i].estado],
                i < n - 1 ? "," : "");
    }
    fprintf(f, "  ],\n  \"timeline\": [");
    for (int i = 0; i < timeline_len; i++)
        fprintf(f, "%d%s", timeline[i], i < timeline_len - 1 ? ", " : "");
    fprintf(f, "]\n");

    fprintf(f, "}\n");
    fclose(f);
}
