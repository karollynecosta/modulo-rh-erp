#include "../include/ponto.h"
#include "../include/sistema_rh.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <algorithm>
#include <map>

using namespace std;

// ========== Implementação da Classe RegistroPonto ==========

RegistroPonto::RegistroPonto(int idFuncionario, const string& data, const string& hora,
                           const string& tipo, const string& observacao)
    : idFuncionario(idFuncionario), data(data), hora(hora), tipo(tipo), observacao(observacao) {}

string RegistroPonto::formatarParaArquivo() const {
    ostringstream oss;
    oss << idFuncionario << ";" << data << ";" << hora << ";" << tipo << ";" << observacao;
    return oss.str();
}

// ========== Métodos Estáticos de Validação ==========

bool RegistroPonto::validarData(const string& data) {
    if (data.length() != 10) return false;
    if (data[2] != '/' || data[5] != '/') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(data[i])) {
            return false;
        }
    }
    
    int dia = stoi(data.substr(0, 2));
    int mes = stoi(data.substr(3, 2));
    int ano = stoi(data.substr(6, 4));
    
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    if (ano < 1900 || ano > 2100) return false;
    
    // Verificação básica de dias por mês
    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0)) {
        diasPorMes[1] = 29; // Ano bissexto
    }
    
    return dia <= diasPorMes[mes - 1];
}

bool RegistroPonto::validarHora(const string& hora) {
    if (hora.length() != 8) return false;
    if (hora[2] != ':' || hora[5] != ':') return false;
    
    for (int i = 0; i < 8; i++) {
        if (i != 2 && i != 5 && !isdigit(hora[i])) {
            return false;
        }
    }
    
    int horas = stoi(hora.substr(0, 2));
    int minutos = stoi(hora.substr(3, 2));
    int segundos = stoi(hora.substr(6, 2));
    
    return horas >= 0 && horas < 24 && 
           minutos >= 0 && minutos < 60 && 
           segundos >= 0 && segundos < 60;
}

bool RegistroPonto::validarTipo(const string& tipo) {
    return tipo == "ENTRADA" || tipo == "SAIDA";
}

// ========== Métodos Utilitários Estáticos ==========

string RegistroPonto::obterDataAtual() {
    time_t agora = time(0);
    tm* tempoLocal = localtime(&agora);
    
    stringstream ss;
    ss << setfill('0') << setw(2) << tempoLocal->tm_mday << "/"
       << setw(2) << (tempoLocal->tm_mon + 1) << "/"
       << (tempoLocal->tm_year + 1900);
    
    return ss.str();
}

string RegistroPonto::obterHoraAtual() {
    time_t agora = time(0);
    tm* tempoLocal = localtime(&agora);
    
    stringstream ss;
    ss << setfill('0') << setw(2) << tempoLocal->tm_hour << ":"
       << setw(2) << tempoLocal->tm_min << ":"
       << setw(2) << tempoLocal->tm_sec;
    
    return ss.str();
}

double RegistroPonto::converterHoraParaDecimal(const string& hora) {
    if (hora.length() < 8) return 0.0;
    
    int h = stoi(hora.substr(0, 2));
    int m = stoi(hora.substr(3, 2));
    int s = stoi(hora.substr(6, 2));
    
    return h + (m / 60.0) + (s / 3600.0);
}

double RegistroPonto::calcularHorasEntreHorarios(const string& entrada, const string& saida) {
    double horaEntrada = converterHoraParaDecimal(entrada);
    double horaSaida = converterHoraParaDecimal(saida);
    
    if (horaSaida >= horaEntrada) {
        return horaSaida - horaEntrada;
    } else {
        // Saída no dia seguinte
        return (24.0 - horaEntrada) + horaSaida;
    }
}

// ========== Implementação da Classe GerenciadorPonto (para CLI) ==========

void GerenciadorPonto::registrarEntrada(SistemaRH& sistema, int idFuncionario, const string& observacao) {
    string data = RegistroPonto::obterDataAtual();
    string hora = RegistroPonto::obterHoraAtual();
    
    // Verificar se já existe entrada sem saída no mesmo dia
    const auto& pontos = sistema.getPontos();
    string ultimoTipo = "";
    string ultimaHora = "";
    
    for (const auto& ponto : pontos) {
        if (ponto.getIdFuncionario() == idFuncionario && ponto.getData() == data) {
            if (ponto.getHora() > ultimaHora) {
                ultimaHora = ponto.getHora();
                ultimoTipo = ponto.getTipo();
            }
        }
    }
    
    if (ultimoTipo == "ENTRADA") {
        cout << "Erro: Já existe uma entrada registrada sem saída para este funcionário hoje." << endl;
        return;
    }
    
    registrarManual(sistema, idFuncionario, data, hora, "ENTRADA", observacao);
    cout << "Entrada registrada com sucesso às " << hora << "." << endl;
}

void GerenciadorPonto::registrarSaida(SistemaRH& sistema, int idFuncionario, const string& observacao) {
    string data = RegistroPonto::obterDataAtual();
    string hora = RegistroPonto::obterHoraAtual();
    
    // Verificar se existe entrada sem saída
    const auto& pontos = sistema.getPontos();
    string ultimoTipo = "";
    string ultimaHora = "";
    
    for (const auto& ponto : pontos) {
        if (ponto.getIdFuncionario() == idFuncionario && ponto.getData() == data) {
            if (ponto.getHora() > ultimaHora) {
                ultimaHora = ponto.getHora();
                ultimoTipo = ponto.getTipo();
            }
        }
    }
    
    if (ultimoTipo != "ENTRADA") {
        cout << "Erro: Não há entrada registrada para este funcionário hoje." << endl;
        return;
    }
    
    registrarManual(sistema, idFuncionario, data, hora, "SAIDA", observacao);
    cout << "Saída registrada com sucesso às " << hora << "." << endl;
}

void GerenciadorPonto::registrarManual(SistemaRH& sistema, int idFuncionario, const string& data, 
                                      const string& hora, const string& tipo, const string& observacao) {
    RegistroPonto ponto(idFuncionario, data, hora, tipo, observacao);
    sistema.adicionarPonto(ponto);
}

void GerenciadorPonto::listarPorFuncionario(const SistemaRH& sistema, int idFuncionario) {
    const auto& pontos = sistema.getPontos();
    
    cout << "\n=== Registros de Ponto - Funcionário ID: " << idFuncionario << " ===" << endl;
    cout << "Data       | Hora     | Tipo    | Observação" << endl;
    cout << "-----------|----------|---------|------------------------" << endl;
    
    bool encontrou = false;
    for (const auto& ponto : pontos) {
        if (ponto.getIdFuncionario() == idFuncionario) {
            cout << ponto.getData() << " | " << ponto.getHora() << " | "
                 << setw(7) << left << ponto.getTipo() << " | " << ponto.getObservacao() << endl;
            encontrou = true;
        }
    }
    
    if (!encontrou) {
        cout << "Nenhum registro encontrado para este funcionário." << endl;
    }
}

void GerenciadorPonto::listarPorData(const SistemaRH& sistema, const string& data) {
    const auto& pontos = sistema.getPontos();
    const auto& funcionarios = sistema.getFuncionarios();
    
    cout << "\n=== Registros de Ponto - Data: " << data << " ===" << endl;
    cout << "Funcionário                   | Hora     | Tipo    | Observação" << endl;
    cout << "-------------------------------|----------|---------|------------------------" << endl;
    
    bool encontrou = false;
    for (const auto& ponto : pontos) {
        if (ponto.getData() == data) {
            // Buscar nome do funcionário
            string nome = "Desconhecido";
            for (const auto& func : funcionarios) {
                if (func.getID() == ponto.getIdFuncionario()) {
                    nome = func.getNome();
                    break;
                }
            }
            
            cout << setw(30) << left << nome << " | " << ponto.getHora() << " | "
                 << setw(7) << left << ponto.getTipo() << " | " << ponto.getObservacao() << endl;
            encontrou = true;
        }
    }
    
    if (!encontrou) {
        cout << "Nenhum registro encontrado para esta data." << endl;
    }
}

void GerenciadorPonto::listarTodos(const SistemaRH& sistema) {
    const auto& pontos = sistema.getPontos();
    const auto& funcionarios = sistema.getFuncionarios();
    
    cout << "\n=== Todos os Registros de Ponto ===" << endl;
    cout << "Data       | Funcionário               | Hora     | Tipo    | Observação" << endl;
    cout << "-----------|---------------------------|----------|---------|------------------------" << endl;
    
    for (const auto& ponto : pontos) {
        // Buscar nome do funcionário
        string nome = "Desconhecido";
        for (const auto& func : funcionarios) {
            if (func.getID() == ponto.getIdFuncionario()) {
                nome = func.getNome();
                break;
            }
        }
        
        cout << ponto.getData() << " | " << setw(25) << left << nome << " | "
             << ponto.getHora() << " | " << setw(7) << left << ponto.getTipo() 
             << " | " << ponto.getObservacao() << endl;
    }
    
    if (pontos.empty()) {
        cout << "Nenhum registro encontrado." << endl;
    }
}

void GerenciadorPonto::relatorioMensal(const SistemaRH& sistema, int idFuncionario, int mes, int ano) {
    const auto& pontos = sistema.getPontos();
    
    cout << "\n=== Relatório Mensal - Funcionário ID: " << idFuncionario 
         << " - " << mes << "/" << ano << " ===" << endl;
    
    double totalHoras = 0.0;
    
    // Filtrar pontos do mês/ano
    vector<RegistroPonto> pontosMes;
    for (const auto& ponto : pontos) {
        if (ponto.getIdFuncionario() == idFuncionario) {
            string data = ponto.getData();
            if (data.length() >= 10) {
                int pontoMes = stoi(data.substr(3, 2));
                int pontoAno = stoi(data.substr(6, 4));
                
                if (pontoMes == mes && pontoAno == ano) {
                    pontosMes.push_back(ponto);
                }
            }
        }
    }
    
    if (pontosMes.empty()) {
        cout << "Nenhum registro encontrado para este período." << endl;
        return;
    }
    
    // Agrupar por data e calcular horas
    map<string, vector<RegistroPonto>> pontosPorDia;
    for (const auto& ponto : pontosMes) {
        pontosPorDia[ponto.getData()].push_back(ponto);
    }
    
    cout << "Data       | Entrada  | Saída    | Horas Trabalhadas" << endl;
    cout << "-----------|----------|----------|------------------" << endl;
    
    for (auto& [data, pontosDia] : pontosPorDia) {
        sort(pontosDia.begin(), pontosDia.end(), 
             [](const RegistroPonto& a, const RegistroPonto& b) {
                 return a.getHora() < b.getHora();
             });
        
        string entrada = "-", saida = "-";
        double horasDia = 0.0;
        
        for (const auto& ponto : pontosDia) {
            if (ponto.getTipo() == "ENTRADA" && entrada == "-") {
                entrada = ponto.getHora();
            } else if (ponto.getTipo() == "SAIDA" && entrada != "-") {
                saida = ponto.getHora();
                horasDia += RegistroPonto::calcularHorasEntreHorarios(entrada, saida);
                entrada = "-"; // Reset para próxima entrada
            }
        }
        
        totalHoras += horasDia;
        
        cout << data << " | " << setw(8) << left << entrada << " | "
             << setw(8) << left << saida << " | " << fixed << setprecision(2) 
             << horasDia << "h" << endl;
    }
    
    cout << "\nTotal de horas trabalhadas no mês: " << fixed << setprecision(2) 
         << totalHoras << "h" << endl;
}
