#include "../include/ponto.h"
#include "../include/validacao.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <map>

using namespace std;

void GerenciadorPonto::registrarEntrada(SistemaRH& sistema, int idFuncionario, const string& observacao) {
    string data = obterDataAtual();
    string hora = obterHoraAtual();
    
    // Verificar se já existe entrada sem saída no mesmo dia
    string ultimoTipo = getUltimoTipo(sistema, idFuncionario, data);
    if (ultimoTipo == "ENTRADA") {
        cout << "Erro: Já existe uma entrada registrada sem saída para este funcionário hoje." << endl;
        return;
    }
    
    registrarManual(sistema, idFuncionario, data, hora, "ENTRADA", observacao);
    cout << "Entrada registrada com sucesso às " << hora << "." << endl;
}

void GerenciadorPonto::registrarSaida(SistemaRH& sistema, int idFuncionario, const string& observacao) {
    string data = obterDataAtual();
    string hora = obterHoraAtual();
    
    // Verificar se existe entrada sem saída
    string ultimoTipo = getUltimoTipo(sistema, idFuncionario, data);
    if (ultimoTipo != "ENTRADA") {
        cout << "Erro: Não há entrada registrada para este funcionário hoje." << endl;
        return;
    }
    
    registrarManual(sistema, idFuncionario, data, hora, "SAIDA", observacao);
    cout << "Saída registrada com sucesso às " << hora << "." << endl;
}

void GerenciadorPonto::registrarManual(SistemaRH& sistema, int idFuncionario, const string& data, 
                                      const string& hora, const string& tipo, const string& observacao) {
    RegistroPonto ponto;
    ponto.setIdFuncionario(idFuncionario);
    ponto.setData(data);
    ponto.setHora(hora);
    ponto.setTipo(tipo);
    ponto.setObservacao(observacao);
    
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
                horasDia += calcularHorasEntreHorarios(entrada, saida);
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

void GerenciadorPonto::calcularHorasTrabalhadas(const SistemaRH& sistema, int idFuncionario, int mes, int ano) {
    relatorioMensal(sistema, idFuncionario, mes, ano);
}

void GerenciadorPonto::listarHorasDiarias(const SistemaRH& sistema, int idFuncionario, int mes, int ano) {
    relatorioMensal(sistema, idFuncionario, mes, ano);
}

string GerenciadorPonto::obterDataAtual() {
    time_t agora = time(0);
    tm* tempoLocal = localtime(&agora);
    
    stringstream ss;
    ss << setfill('0') << setw(2) << tempoLocal->tm_mday << "/"
       << setw(2) << (tempoLocal->tm_mon + 1) << "/"
       << (tempoLocal->tm_year + 1900);
    
    return ss.str();
}

string GerenciadorPonto::obterHoraAtual() {
    time_t agora = time(0);
    tm* tempoLocal = localtime(&agora);
    
    stringstream ss;
    ss << setfill('0') << setw(2) << tempoLocal->tm_hour << ":"
       << setw(2) << tempoLocal->tm_min << ":"
       << setw(2) << tempoLocal->tm_sec;
    
    return ss.str();
}

bool GerenciadorPonto::validarData(const string& data) {
    return RegistroPontoValidacao::dataValida(data);
}

bool GerenciadorPonto::validarHora(const string& hora) {
    return RegistroPontoValidacao::horaValida(hora);
}

string GerenciadorPonto::getUltimoTipo(const SistemaRH& sistema, int idFuncionario, const string& data) {
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
    
    return ultimoTipo;
}

double GerenciadorPonto::converterHoraParaDecimal(const string& hora) {
    if (hora.length() < 8) return 0.0;
    
    int h = stoi(hora.substr(0, 2));
    int m = stoi(hora.substr(3, 2));
    int s = stoi(hora.substr(6, 2));
    
    return h + (m / 60.0) + (s / 3600.0);
}

double GerenciadorPonto::calcularHorasEntreHorarios(const string& entrada, const string& saida) {
    double horaEntrada = converterHoraParaDecimal(entrada);
    double horaSaida = converterHoraParaDecimal(saida);
    
    if (horaSaida >= horaEntrada) {
        return horaSaida - horaEntrada;
    } else {
        // Saída no dia seguinte
        return (24.0 - horaEntrada) + horaSaida;
    }
}