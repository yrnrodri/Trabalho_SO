#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#define MAX 100
#define MAX_TEMPO 200

#include "algoritmos.h"     // enums, ParamsAlgoritmo

void simular_escalonamento(Processo processos[], int n, TipoAlgoritmo algoritmo, ParamsAlgoritmo params, const char* nome_arquivo);

#endif
