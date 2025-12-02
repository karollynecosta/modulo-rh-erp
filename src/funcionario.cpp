#include "../include/funcionario.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Implementação da classe Funcionario
Funcionario::Funcionario(int id, const string& nome, const string& cpf, 
                        const string& setor, const string& cargo, const string& foto)
    : id(to_string(id)), nome(nome), cpf(cpf), setor(setor), cargo(cargo), foto(foto) {}

string Funcionario::getNome() const { return nome; }
string Funcionario::getCPF() const { return cpf; }
int Funcionario::getID() const { return stoi(id); }
string Funcionario::getSetor() const { return setor; }
string Funcionario::getCargo() const { return cargo; }
string Funcionario::getFoto() const { return foto; }

void Funcionario::setNome(const string& nome) {
    if (FuncionarioValidacao::nomeValido(nome)) {
        this->nome = FuncionarioValidacao::formatarNome(nome);
    }
}

void Funcionario::setCPF(const string& cpf) {
    if (FuncionarioValidacao::cpfValido(cpf)) {
        this->cpf = cpf;
    }
}

void Funcionario::setID(int id) {
    if (id >= ID_MIN && id <= ID_MAX) this->id = to_string(id);
}

void Funcionario::setSetor(const string& setor) {
    if (FuncionarioValidacao::setorValido(setor)) {
        this->setor = setor;
    }
}

void Funcionario::setCargo(const string& cargo) {
    if (FuncionarioValidacao::cargoValido(cargo)) {
        this->cargo = cargo;
    }
}

void Funcionario::setFoto(const string& foto) {
    if (FuncionarioValidacao::fotoValida(foto)) {
        this->foto = foto;
    }
}

void Funcionario::exibirDados() const {
    cout << "-----------------------------" << endl;
    cout << "Nome   : " << nome << endl;
    cout << "CPF    : " << cpf << endl;
    cout << "ID     : " << setw(5) << setfill('0') << stoi(id) << endl;
    cout << "Setor  : " << setor << endl;
    cout << "Cargo  : " << cargo << endl;
    cout << "Foto   : " << (foto.empty() ? "Nao cadastrada" : foto) << endl;
    cout << "-----------------------------" << endl;
    cout << setfill(' ');
}

string Funcionario::formatarParaArquivo() const {
    ostringstream oss;
    oss << stoi(id) << ";" << nome << ";" << cpf << ";" << setor << ";" << cargo << ";" << foto;
    return oss.str();
}

// Implementação das validações específicas para funcionários
namespace FuncionarioValidacao {
    bool nomeValido(const string& nome) {
        if (nome.empty() || nome.length() < 2 || nome.length() > 50) {
            return false;
        }
        
        for (char c : nome) {
            if (!isalpha(c) && c != ' ') {
                return false;
            }
        }
        return true;
    }
    
    bool cpfValido(const string& cpf) {
        if (cpf.length() != CPF_LENGTH) {
            return false;
        }
        
        for (char c : cpf) {
            if (!isdigit(c)) {
                return false;
            }
        }
        
        // Para desenvolvimento: aceita qualquer sequência de 11 dígitos
        // Em produção, você pode descomentar o algoritmo de validação completo abaixo
        return true;
        
        /* Algoritmo completo de validação de CPF (descomentado para produção):
        int soma = 0;
        for (int i = 0; i < 9; i++) {
            soma += (cpf[i] - '0') * (10 - i);
        }
        int digito1 = (soma * 10) % 11;
        if (digito1 == 10) digito1 = 0;
        
        soma = 0;
        for (int i = 0; i < 10; i++) {
            soma += (cpf[i] - '0') * (11 - i);
        }
        int digito2 = (soma * 10) % 11;
        if (digito2 == 10) digito2 = 0;
        
        return (digito1 == (cpf[9] - '0')) && (digito2 == (cpf[10] - '0'));
        */
    }
    
    bool setorValido(const string& setor) {
        return !setor.empty() && setor.length() >= 2 && setor.length() <= 30;
    }
    
    bool cargoValido(const string& cargo) {
        return !cargo.empty() && cargo.length() >= 2 && cargo.length() <= 40;
    }
    
    bool fotoValida(const string& foto) {
        if (foto.empty()) return true; // Foto é opcional
        return foto.length() <= 100; // Limitar tamanho do path
    }
    
    string formatarCPF(const string& cpf) {
        if (cpf.length() == CPF_LENGTH) {
            return cpf.substr(0,3) + "." + cpf.substr(3,3) + "." + cpf.substr(6,3) + "-" + cpf.substr(9,2);
        }
        return cpf;
    }
    
    string formatarNome(const string& nome) {
        string resultado = nome;
        transform(resultado.begin(), resultado.end(), resultado.begin(), ::tolower);
        
        bool proximaMaiuscula = true;
        for (char& c : resultado) {
            if (isalpha(c)) {
                if (proximaMaiuscula) {
                    c = toupper(c);
                    proximaMaiuscula = false;
                }
            } else if (c == ' ') {
                proximaMaiuscula = true;
            }
        }
        return resultado;
    }
}