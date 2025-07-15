#ifndef MODULO1_H
#define MODULO1_H

#include "algoritmos.h"
#include "processo.h"
#include "exporta_json.h"

int gerar_processos(Processo processos[], int n, int op, const char* nome_arquivo);
void imprimir_processos_iniciais(Processo processos[], int n);

#endif