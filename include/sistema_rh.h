#ifndef SISTEMA_RH_H
#define SISTEMA_RH_H

#include "types.h"
#include "funcionario.h"
#include "setor.h"
#include "registro_ponto.h"

// Classe principal do sistema de RH
class SistemaRH {
private:
    vector<Funcionario> funcionarios;
    vector<Setor> setores;
    vector<RegistroPonto> pontos;
    
public:
    SistemaRH() = default;
    
    // Métodos para funcionários
    void adicionarFuncionario(const Funcionario& funcionario);
    Funcionario* buscarFuncionarioPorId(int id);
    const Funcionario* buscarFuncionarioPorId(int id) const;
    bool removerFuncionario(int id);
    vector<Funcionario>& getFuncionarios();
    const vector<Funcionario>& getFuncionarios() const;
    int getNumFuncionarios() const;
    
    // Métodos para setores
    void adicionarSetor(const Setor& setor);
    vector<Setor>& getSetores();
    const vector<Setor>& getSetores() const;
    int getNumSetores() const;
    void inicializarSetores();
    
    // Métodos para pontos
    void adicionarPonto(const RegistroPonto& ponto);
    vector<RegistroPonto>& getPontos();
    const vector<RegistroPonto>& getPontos() const;
    int getNumPontos() const;
    
    // Utilitários
    int cargoRank(const string& cargo) const;
};

#endif