#ifndef PONTO_H
#define PONTO_H

#include "types.h"
#include <string>

// Forward declaration
class SistemaRH;

// Classe para representar um registro de ponto
class RegistroPonto {
private:
    int idFuncionario;
    string data;        // formato DD/MM/AAAA
    string hora;        // formato HH:MM:SS
    string tipo;        // "ENTRADA" ou "SAIDA"
    string observacao;
    
public:
    RegistroPonto() = default;
    RegistroPonto(int idFuncionario, const string& data, const string& hora,
                 const string& tipo, const string& observacao = "");
    
    // Getters
    int getIdFuncionario() const { return idFuncionario; }
    string getData() const { return data; }
    string getHora() const { return hora; }
    string getTipo() const { return tipo; }
    string getObservacao() const { return observacao; }
    
    // Setters
    void setIdFuncionario(int id) { idFuncionario = id; }
    void setData(const string& d) { data = d; }
    void setHora(const string& h) { hora = h; }
    void setTipo(const string& t) { tipo = t; }
    void setObservacao(const string& obs) { observacao = obs; }
    
    // Formatação
    string formatarParaArquivo() const;
    
    // Validações estáticas
    static bool validarData(const string& data);
    static bool validarHora(const string& hora);
    static bool validarTipo(const string& tipo);
    
    // Utilitários estáticos
    static string obterDataAtual();
    static string obterHoraAtual();
    static double converterHoraParaDecimal(const string& hora);
    static double calcularHorasEntreHorarios(const string& entrada, const string& saida);
};

// Classe utilitária para operações de ponto no CLI
class GerenciadorPonto {
public:
    // Funções para registrar ponto
    static void registrarEntrada(SistemaRH& sistema, int idFuncionario, const string& observacao = "");
    static void registrarSaida(SistemaRH& sistema, int idFuncionario, const string& observacao = "");
    static void registrarManual(SistemaRH& sistema, int idFuncionario, const string& data, 
                               const string& hora, const string& tipo, const string& observacao = "");

    // Funções de consulta e relatórios
    static void listarPorFuncionario(const SistemaRH& sistema, int idFuncionario);
    static void listarPorData(const SistemaRH& sistema, const string& data);
    static void listarTodos(const SistemaRH& sistema);
    static void relatorioMensal(const SistemaRH& sistema, int idFuncionario, int mes, int ano);
};

#endif