#include "../include/setor.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Implementação da classe Cargo
Cargo::Cargo(const string& nome) : nome(nome) {}

string Cargo::getNome() const {
    return nome;
}

void Cargo::setNome(const string& nome) {
    this->nome = nome;
}

// Implementação da classe Setor
Setor::Setor(const string& nome) : nome(nome) {}

string Setor::getNome() const {
    return nome;
}

void Setor::setNome(const string& nome) {
    this->nome = nome;
}

void Setor::adicionarCargo(const Cargo& cargo) {
    cargosPermitidos.push_back(cargo);
}

const vector<Cargo>& Setor::getCargos() const {
    return cargosPermitidos;
}

int Setor::getNumCargos() const {
    return static_cast<int>(cargosPermitidos.size());
}

// Implementação das validações específicas para setores e cargos
namespace SetorValidacao {
    bool nomeSetorValido(const string& nome) {
        if (nome.empty() || nome.length() < 2 || nome.length() > 50) {
            return false;
        }
        
        for (char c : nome) {
            if (!isalnum(c) && c != ' ' && c != '-' && c != '_') {
                return false;
            }
        }
        return true;
    }
    
    bool nomeCargoValido(const string& nome) {
        if (nome.empty() || nome.length() < 2 || nome.length() > 40) {
            return false;
        }
        
        for (char c : nome) {
            if (!isalnum(c) && c != ' ' && c != '-' && c != '_') {
                return false;
            }
        }
        return true;
    }
    
    bool setorTemCargo(const Setor& setor, const string& nomeCargo) {
        const auto& cargos = setor.getCargos();
        return find_if(cargos.begin(), cargos.end(), 
                      [&nomeCargo](const Cargo& cargo) {
                          return cargo.getNome() == nomeCargo;
                      }) != cargos.end();
    }
    
    vector<string> listarCargosPermitidos(const Setor& setor) {
        vector<string> nomesCargos;
        const auto& cargos = setor.getCargos();
        transform(cargos.begin(), cargos.end(), back_inserter(nomesCargos),
                 [](const Cargo& cargo) { return cargo.getNome(); });
        return nomesCargos;
    }
}