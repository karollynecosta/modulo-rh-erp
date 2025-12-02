#ifndef OPERACOES_H
#define OPERACOES_H

#include "sistema_rh.h"
#include "validacao.h"

class OperacoesRH {
public:
    // Operações principais do sistema
    static void cadastrarFuncionario(SistemaRH& sistema);
    static void buscarFuncionario(const SistemaRH& sistema);
    static void editarFuncionario(SistemaRH& sistema);
    static void excluirFuncionario(SistemaRH& sistema);

    // Operações de ponto
    static void baterPonto(SistemaRH& sistema);
    static void registrarPontoManual(SistemaRH& sistema);
    static void consultarPontos(const SistemaRH& sistema);

    // Funções de listagem
    static void exibirFuncionarios(const SistemaRH& sistema);
    static void listarPorNome(const SistemaRH& sistema);
    static void listarPorCargoHierarquico(const SistemaRH& sistema);
    static void listarPorSetorHierarquico(const SistemaRH& sistema);
    static void listarPorSetorECargo(const SistemaRH& sistema);
};

#endif