#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "types.h"

// Funções para salvar e carregar dados de funcionários
void persistencia_importar_de_arquivo(SistemaRH *sistema, const char *arquivo);
void persistencia_exportar_para_arquivo(const SistemaRH *sistema, const char *arquivo);

// Funções para salvar e carregar dados de pontos
void persistencia_importar_pontos(SistemaRH *sistema, const char *arquivo);
void persistencia_exportar_pontos(const SistemaRH *sistema, const char *arquivo);

#endif