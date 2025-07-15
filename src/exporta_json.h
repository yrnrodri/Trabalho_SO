#ifndef EXPORTA_JSON_H
#define EXPORTA_JSON_H

#include "processo.h"
#include "algoritmos.h"
#include "escalonador.h"

// PFunção de exportação
void salvar_json(Processo processos[], int n, TipoAlgoritmo algoritmo, ParamsAlgoritmo params, const char* nome_arquivo, int timeline[], int timeline_len);

#endif
