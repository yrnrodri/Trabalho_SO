#ifndef PROCESSO_H
#define PROCESSO_H

typedef enum {
    PRONTO,
    EXECUTANDO,
    FINALIZADO
} EstadoProcesso;

typedef struct
{
    int pid;            // Identificador do processo
    int tempo_chegada;  // Quando o processo chega na fila
    int tempo_execucao; // Quanto tempo ele precisa para terminar
    int prioridade;
    EstadoProcesso estado; // Estado atual do processo
    int tempo_restante; // Tempo restante de execução
    int tempo_espera;   // Tempo total esperando
    int finalizado;     // 0 = não finalizado, 1 = finalizado
} Processo;

#endif
