#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
  #include <direct.h>
  #define MKDIR(p) _mkdir(p)
#else
  #include <unistd.h>
  #define MKDIR(p) mkdir((p), 0755)
#endif

#include "escalonador.h"
#include "modulo1.h"
#include "exporta_json.h"

/* =======================================================================
   Parser simples do JSON gerado pelo Python (indentado).
   Não depende de linhas específicas: lê o arquivo inteiro e procura,
   em sequência, pelos campos "pid", "chegada", "execucao" e "prioridade".
   Ignora qualquer coisa extra no JSON.
   Retorna número de processos lidos.
   ======================================================================= */
int carregar_processos_do_json(const char *nome_arquivo,
                               Processo processos[],
                               int max)
{
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f) {
        fprintf(stderr, "carregar_processos_do_json: não abriu %s\n", nome_arquivo);
        return 0;
    }
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (tamanho <= 0) {
        fclose(f);
        return 0;
    }
    char *buf = (char*)malloc((size_t)tamanho + 1);
    if (!buf) {
        fclose(f);
        return 0;
    }
    if (fread(buf, 1, (size_t)tamanho, f) != (size_t)tamanho) {
        fclose(f);
        free(buf);
        return 0;
    }
    fclose(f);
    buf[tamanho] = '\0';

    int n = 0;
    char *p = buf;
    while (n < max) {
        char *kpid = strstr(p, "\"pid\"");
        if (!kpid) break;
        char *kche = strstr(kpid, "\"chegada\"");
        char *kexe = strstr(kpid, "\"execucao\"");
        char *kpri = strstr(kpid, "\"prioridade\"");
        if (!kche || !kexe || !kpri) break;  /* JSON malformado */

        int pid, chegada, execucao, prioridade;

        if (sscanf(kpid, "%*[^:] : %d", &pid) != 1) break;
        if (sscanf(kche, "%*[^:] : %d", &chegada) != 1) break;
        if (sscanf(kexe, "%*[^:] : %d", &execucao) != 1) break;
        if (sscanf(kpri, "%*[^:] : %d", &prioridade) != 1) break;

        processos[n].pid            = pid;
        processos[n].tempo_chegada  = chegada;
        processos[n].tempo_execucao = execucao;
        processos[n].prioridade     = prioridade;
        processos[n].tempo_restante = execucao;
        processos[n].tempo_espera   = 0;
        processos[n].estado         = PRONTO;
        processos[n].finalizado     = 0;
        n++;

        /* avança 'p' para depois do bloco atual para procurar o próximo */
        p = kpri + 1;
    }

    free(buf);
    return n;
}

/* ======================================================================= */
int main(int argc, char *argv[])
{
    /* Garante existência da pasta results/ (relativa ao cwd) */
    MKDIR("results");

    Processo originais[MAX];
    int n = 5, op;
    char *modo = NULL;
    char *algoritmo = NULL;

    if (argc > 1) modo      = argv[1];
    if (argc > 2) algoritmo = argv[2];

    /* Se pediu --fixed ou --random: gera processos e salva; opcionalmente roda algoritmo */
    if (modo && (strcmp(modo, "--fixed") == 0 || strcmp(modo, "--random") == 0)) {
        op = (strcmp(modo, "--random") == 0) ? 0 : 1;
        n = gerar_processos(originais, n, op, "results/dados_originais.json");
        printf("Processos %s gerados!\n", op == 0 ? "aleatórios" : "fixos");
        imprimir_processos_iniciais(originais, n);
        if (!algoritmo) return 0;  /* só queria gerar */
    }
    /* Se pediu --load: carrega processos previamente salvos em JSON */
    else if (modo && strcmp(modo, "--load") == 0) {
        n = carregar_processos_do_json("results/dados_originais.json", originais, MAX);
        if (n == 0) {
            fprintf(stderr, "Erro: não carregou processos! (results/dados_originais.json)\n");
            return 1;
        }
    }
    else {
        fprintf(stderr, "Uso: %s [--fixed|--random|--load] [ALGORITMO]\n", argv[0]);
        return 1;
    }

    if (!algoritmo) {
        fprintf(stderr, "Nenhum algoritmo especificado.\n");
        return 0;
    }

    /* Com processos carregados, roda algoritmo solicitado */
    ParamsAlgoritmo params;
    Processo copia[MAX];

    if (strcmp(algoritmo, "FIFO") == 0) {
        memcpy(copia, originais, sizeof(Processo) * n);
        params.quantum = 0; params.tickets = NULL; params.seed = 0;
        simular_escalonamento(copia, n, FIFO, params, "results/fifo.json");

    } else if (strcmp(algoritmo, "SJF") == 0) {
        memcpy(copia, originais, sizeof(Processo) * n);
        simular_escalonamento(copia, n, SJF, params, "results/sjf.json");

    } else if (strcmp(algoritmo, "PRIORIDADE") == 0) {
        memcpy(copia, originais, sizeof(Processo) * n);
        params.quantum = 2;
        simular_escalonamento(copia, n, PRIORIDADE, params, "results/prioridade.json");

    } else if (strcmp(algoritmo, "LOTTERY") == 0) {
        memcpy(copia, originais, sizeof(Processo) * n);
        int tickets[MAX] = {1,2,3,1,2};
        params.tickets = tickets; params.seed = (unsigned)time(NULL);
        simular_escalonamento(copia, n, LOTTERY, params, "results/lottery.json");

    } else {
        fprintf(stderr, "Algoritmo desconhecido: %s\n", algoritmo);
        return 1;
    }

    return 0;
}
