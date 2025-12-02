#include "../include/registro_ponto.h"
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Implementação da classe RegistroPonto
RegistroPonto::RegistroPonto(int idFuncionario, const string& data, const string& hora,
                           const string& tipo, const string& observacao)
    : idFuncionario(idFuncionario), data(data), hora(hora), tipo(tipo), observacao(observacao) {}

int RegistroPonto::getIdFuncionario() const { return idFuncionario; }
string RegistroPonto::getData() const { return data; }
string RegistroPonto::getHora() const { return hora; }
string RegistroPonto::getTipo() const { return tipo; }
string RegistroPonto::getObservacao() const { return observacao; }

void RegistroPonto::setIdFuncionario(int id) { this->idFuncionario = id; }
void RegistroPonto::setData(const string& data) { this->data = data; }
void RegistroPonto::setHora(const string& hora) { this->hora = hora; }
void RegistroPonto::setTipo(const string& tipo) { this->tipo = tipo; }
void RegistroPonto::setObservacao(const string& obs) { this->observacao = obs; }

string RegistroPonto::formatarParaArquivo() const {
    ostringstream oss;
    oss << idFuncionario << ";" << data << ";" << hora << ";" << tipo << ";" << observacao;
    return oss.str();
}

// Implementação das validações específicas para registros de ponto
namespace RegistroPontoValidacao {
    bool dataValida(const string& data) {
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
    
    bool horaValida(const string& hora) {
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
    
    bool tipoValido(const string& tipo) {
        return tipo == "ENTRADA" || tipo == "SAIDA";
    }
    
    bool observacaoValida(const string& observacao) {
        return observacao.length() <= 100; // Limite de caracteres
    }
    
    string formatarData(const string& data) {
        // Assumindo entrada no formato DD/MM/AAAA
        if (dataValida(data)) {
            return data;
        }
        return "";
    }
    
    string formatarHora(const string& hora) {
        // Assumindo entrada no formato HH:MM:SS
        if (horaValida(hora)) {
            return hora;
        }
        return "";
    }
}