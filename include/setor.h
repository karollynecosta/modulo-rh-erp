#ifndef SETOR_H
#define SETOR_H

#include "types.h"

// Classe para representar um cargo
class Cargo {
private:
    string nome;
public:
    Cargo() = default;
    Cargo(const string& nome);
    
    string getNome() const;
    void setNome(const string& nome);
};

// Classe para representar um setor
class Setor {
private:
    string nome;
    vector<Cargo> cargosPermitidos;
    
public:
    Setor() = default;
    Setor(const string& nome);
    
    string getNome() const;
    void setNome(const string& nome);
    void adicionarCargo(const Cargo& cargo);
    const vector<Cargo>& getCargos() const;
    int getNumCargos() const;
};

// Validações específicas para setores e cargos
namespace SetorValidacao {
    bool nomeSetorValido(const string& nome);
    bool nomeCargoValido(const string& nome);
    bool setorTemCargo(const Setor& setor, const string& nomeCargo);
    vector<string> listarCargosPermitidos(const Setor& setor);
}

#endif