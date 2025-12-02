#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include "types.h"

// Classe para representar um funcionário
class Funcionario {
private:
    string id;
    string nome;
    string cpf;
    string setor;
    string cargo;
    string foto;
    
public:
    Funcionario() = default;
    Funcionario(int id, const string& nome, const string& cpf,
               const string& setor, const string& cargo, const string& foto = "");
    
    // Getters
    string getNome() const;
    string getCPF() const;
    int getID() const;
    string getSetor() const;
    string getCargo() const;
    string getFoto() const;
    
    // Setters
    void setNome(const string& nome);
    void setCPF(const string& cpf);
    void setID(int id);
    void setSetor(const string& setor);
    void setCargo(const string& cargo);
    void setFoto(const string& foto);
    
    void exibirDados() const;
    string formatarParaArquivo() const;
};

// Validações específicas para funcionários
namespace FuncionarioValidacao {
    bool nomeValido(const string& nome);
    bool cpfValido(const string& cpf);
    bool setorValido(const string& setor);
    bool cargoValido(const string& cargo);
    bool fotoValida(const string& foto);
    string formatarCPF(const string& cpf);
    string formatarNome(const string& nome);
}

#endif