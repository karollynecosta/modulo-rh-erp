#include "../include/sistema_rh.h"
#include <algorithm>

using namespace std;

// Implementação da classe SistemaRH
void SistemaRH::adicionarFuncionario(const Funcionario& funcionario) {
    if (funcionarios.size() < MAX_FUNCIONARIOS) {
        funcionarios.push_back(funcionario);
    }
}

Funcionario* SistemaRH::buscarFuncionarioPorId(int id) {
    for (auto& funcionario : funcionarios) {
        if (funcionario.getID() == id) {
            return &funcionario;
        }
    }
    return nullptr;
}

const Funcionario* SistemaRH::buscarFuncionarioPorId(int id) const {
    for (const auto& funcionario : funcionarios) {
        if (funcionario.getID() == id) {
            return &funcionario;
        }
    }
    return nullptr;
}

bool SistemaRH::removerFuncionario(int id) {
    auto it = funcionarios.begin();
    while (it != funcionarios.end()) {
        if (it->getID() == id) {
            funcionarios.erase(it);
            return true;
        }
        ++it;
    }
    return false;
}

vector<Funcionario>& SistemaRH::getFuncionarios() { return funcionarios; }
const vector<Funcionario>& SistemaRH::getFuncionarios() const { return funcionarios; }
int SistemaRH::getNumFuncionarios() const { return static_cast<int>(funcionarios.size()); }

void SistemaRH::adicionarSetor(const Setor& setor) {
    if (setores.size() < MAX_SETORES) {
        setores.push_back(setor);
    }
}

vector<Setor>& SistemaRH::getSetores() { return setores; }
const vector<Setor>& SistemaRH::getSetores() const { return setores; }
int SistemaRH::getNumSetores() const { return static_cast<int>(setores.size()); }

void SistemaRH::inicializarSetores() {
    vector<string> hierarquia = {
        "Estagiario", "Auxiliar", "Assistente 2", "Assistente 1",
        "Analista Junior", "Analista Pleno", "Analista Senior",
        "Coordenador", "Gerente"
    };
    
    vector<string> nomesSetores = {
        "Recursos Humanos", "Financeiro", "Producao", "Estoque",
        "Compras", "Vendas", "TI", "Manutencao",
        "Controle de Qualidade", "Garantia de Qualidade"
    };
    
    setores.clear();
    
    for (const string& nomeSetor : nomesSetores) {
        Setor setor(nomeSetor);
        for (const string& nomeCargo : hierarquia) {
            setor.adicionarCargo(Cargo(nomeCargo));
        }
        setores.push_back(setor);
    }
}

void SistemaRH::adicionarPonto(const RegistroPonto& ponto) {
    if (pontos.size() < MAX_PONTOS) {
        pontos.push_back(ponto);
    }
}

vector<RegistroPonto>& SistemaRH::getPontos() { return pontos; }
const vector<RegistroPonto>& SistemaRH::getPontos() const { return pontos; }
int SistemaRH::getNumPontos() const { return static_cast<int>(pontos.size()); }

int SistemaRH::cargoRank(const string& cargo) const {
    vector<string> hierarquia = {
        "Estagiario", "Auxiliar", "Assistente 2", "Assistente 1",
        "Analista Junior", "Analista Pleno", "Analista Senior",
        "Coordenador", "Gerente"
    };
    
    auto it = find(hierarquia.begin(), hierarquia.end(), cargo);
    if (it != hierarquia.end()) {
        return static_cast<int>(distance(hierarquia.begin(), it));
    }
    return static_cast<int>(hierarquia.size());
}