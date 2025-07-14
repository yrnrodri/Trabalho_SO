#include "processo.h"
#include <string.h>

void simular_SJF(Processo processos[], Processo escalonados_em_ordem[], int n)
{
    int tempo = 0;
    int finalizados = 0;

    while (finalizados < n)
    {
        int escolhido = -1;
        int menor_tempo_exec = 100;

        // Checa se o processo já chegou, se não está finalizado e encontra o que tem o
        // menor tempo de execução
        for (int i = 0; i < n; i++)
        {
            if (processos[i].tempo_chegada <= tempo &&
                processos[i].finalizado == 0 &&
                processos[i].tempo_execucao < menor_tempo_exec)
            {
                escolhido = i;
                menor_tempo_exec = processos[i].tempo_execucao;
            }
        }

        if (escolhido == -1)
        { // Se nenhum naquele tempo chegou, passa o tempo
            tempo++;
            continue;
        }

        strcpy(processos[escolhido].estado, "Em execucao");
        // Atualiza o tempo passado
        for (int i = 0; i < processos[escolhido].tempo_execucao; i++)
        {
            tempo++;

            // Atualiza o tempo de espera dos outros processos
            for (int j = 0; j < n; j++)
            {
                if (j != escolhido && processos[j].tempo_chegada <= tempo &&
                    processos[j].finalizado == 0)
                {
                    processos[j].tempo_espera++;
                }
            }
        }

        processos[escolhido].finalizado = 1;
        strcpy(processos[escolhido].estado, "Finalizado");

        escalonados_em_ordem[finalizados] = processos[escolhido];
        finalizados++;
    }
}