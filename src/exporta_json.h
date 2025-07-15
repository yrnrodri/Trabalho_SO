#ifndef EXPORTA_JSON_H
#define EXPORTA_JSON_H

#include "processo.h"
#include "algoritmos.h"
#include "escalonador.h"

void salvar_json_processos_iniciais(Processo processos[], int n, const char* nome_arquivo); 
void salvar_json(Processo processos[], int n, TipoAlgoritmo algoritmo, ParamsAlgoritmo params, const char* nome_arquivo, int timeline[], int timeline_len);

#endif
