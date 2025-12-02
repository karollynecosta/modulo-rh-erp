#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "sistema_rh.h"
#include <string>

class Persistencia {
public:
    // Funções para salvar e carregar dados de funcionários
    static void importarDeArquivo(SistemaRH& sistema, const string& arquivo);
    static void exportarParaArquivo(const SistemaRH& sistema, const string& arquivo);

    // Funções para salvar e carregar dados de pontos
    static void importarPontos(SistemaRH& sistema, const string& arquivo);
    static void exportarPontos(const SistemaRH& sistema, const string& arquivo);
};

#endif