#include "../include/validacao.h"
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

// Validações gerais movidas para namespaces específicos:
// - FuncionarioValidacao: nomeValido, cpfValido (em funcionario.cpp)
// - RegistroPonto: validarData, validarHora (métodos estáticos em ponto.cpp)
// - SetorValidacao: nomeSetorValido, nomeCargoValido (em setor.cpp)

bool Validacao::idValido(int id, const SistemaRH& sistema, int idAtual) {
    if (id < ID_MIN || id > ID_MAX) return false;
    
    const auto& funcionarios = sistema.getFuncionarios();
    return none_of(funcionarios.begin(), funcionarios.end(), 
                   [id, idAtual](const Funcionario& f) {
                       return f.getID() == id && id != idAtual;
                   });
}

bool Validacao::funcionarioExiste(int id, const SistemaRH& sistema) {
    if (id < ID_MIN || id > ID_MAX) return false;
    
    const auto& funcionarios = sistema.getFuncionarios();
    return any_of(funcionarios.begin(), funcionarios.end(), 
                  [id](const Funcionario& f) {
                      return f.getID() == id;
                  });
}

bool Validacao::idFormatoValido(const string& idStr) {
    if (idStr.length() != ID_LENGTH) return false;
    
    return all_of(idStr.begin(), idStr.end(), [](char c) {
        return isdigit(static_cast<unsigned char>(c));
    });
}

string Validacao::entradaString(const string& mensagem, bool (*validador)(const string&), bool obrigatorio) {
    string entrada;
    
    while (true) {
        cout << mensagem;
        getline(cin, entrada);
        
        if (entrada == "0") return "0";
        
        if (!obrigatorio && entrada.empty()) return "";
        
        if (!validador || validador(entrada)) return entrada;
        
        cout << "Entrada invalida. Tente novamente ou digite 0 para cancelar." << endl;
    }
}

int Validacao::entradaID(const string& mensagem, const SistemaRH& sistema, int idAtual) {
    string entrada;
    
    while (true) {
        cout << mensagem;
        getline(cin, entrada);
        
        if (entrada == "0") return 0;
        
        if (idFormatoValido(entrada)) {
            int id = stoi(entrada);
            if (idValido(id, sistema, idAtual)) {
                return id;
            }
        }
        
        cout << "ID invalido. Tente novamente ou digite 0 para cancelar." << endl;
    }
}

int Validacao::entradaIDExistente(const string& mensagem, const SistemaRH& sistema) {
    string entrada;
    
    while (true) {
        cout << mensagem;
        getline(cin, entrada);
        
        if (entrada == "0") return 0;
        
        if (idFormatoValido(entrada)) {
            int id = stoi(entrada);
            if (funcionarioExiste(id, sistema)) {
                return id;
            }
        }
        
        cout << "Funcionario nao encontrado. Tente novamente ou digite 0 para cancelar." << endl;
    }
}

void Validacao::limparEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}