#ifndef PONTO_H
#define PONTO_H

#include "sistema_rh.h"
#include "registro_ponto.h"
#include <ctime>
#include <string>

class GerenciadorPonto {
public:
    // Funções para registrar ponto
    static void registrarEntrada(SistemaRH& sistema, int idFuncionario, const string& observacao = "");
    static void registrarSaida(SistemaRH& sistema, int idFuncionario, const string& observacao = "");
    static void registrarManual(SistemaRH& sistema, int idFuncionario, const string& data, 
                               const string& hora, const string& tipo, const string& observacao = "");

    // Funções de consulta
    static void listarPorFuncionario(const SistemaRH& sistema, int idFuncionario);
    static void listarPorData(const SistemaRH& sistema, const string& data);
    static void listarTodos(const SistemaRH& sistema);
    static void relatorioMensal(const SistemaRH& sistema, int idFuncionario, int mes, int ano);
    static void calcularHorasTrabalhadas(const SistemaRH& sistema, int idFuncionario, int mes, int ano);
    static void listarHorasDiarias(const SistemaRH& sistema, int idFuncionario, int mes, int ano);

    // Funções auxiliares
    static string obterDataAtual();
    static string obterHoraAtual();
    static bool validarData(const string& data);
    static bool validarHora(const string& hora);
    static string getUltimoTipo(const SistemaRH& sistema, int idFuncionario, const string& data);
    static double converterHoraParaDecimal(const string& hora);
    static double calcularHorasEntreHorarios(const string& entrada, const string& saida);
};

#endif