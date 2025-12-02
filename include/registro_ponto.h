#ifndef REGISTRO_PONTO_H
#define REGISTRO_PONTO_H

#include "types.h"

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
    int getIdFuncionario() const;
    string getData() const;
    string getHora() const;
    string getTipo() const;
    string getObservacao() const;
    
    // Setters
    void setIdFuncionario(int id);
    void setData(const string& data);
    void setHora(const string& hora);
    void setTipo(const string& tipo);
    void setObservacao(const string& obs);
    string formatarParaArquivo() const;
};

// Validações específicas para registros de ponto
namespace RegistroPontoValidacao {
    bool dataValida(const string& data);
    bool horaValida(const string& hora);
    bool tipoValido(const string& tipo);
    bool observacaoValida(const string& observacao);
    string formatarData(const string& data);
    string formatarHora(const string& hora);
}

#endif