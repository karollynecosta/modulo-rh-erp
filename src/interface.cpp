#include "../include/interface.h"
#include <iostream>
#include <iomanip>
#include <exception>
#include <sstream>
#include <string>

using namespace std;

void Interface::exibirMenu() {
    cout << "\n=== SISTEMA DE RH ===" << endl;
    cout << "1. Cadastrar funcionário" << endl;
    cout << "2. Buscar funcionário" << endl;
    cout << "3. Editar funcionário" << endl;
    cout << "4. Excluir funcionário" << endl;
    cout << "5. Bater ponto" << endl;
    cout << "6. Registrar ponto manual" << endl;
    cout << "7. Consultar pontos" << endl;
    cout << "8. Listagens" << endl;
    cout << "9. Importar dados" << endl;
    cout << "10. Exportar dados" << endl;
    cout << "0. Sair" << endl;
    cout << "Opção: ";
}

void Interface::exibirSubmenuListagens() {
    cout << "\n=== LISTAGENS ===" << endl;
    cout << "1. Exibir todos os funcionários" << endl;
    cout << "2. Listar por nome (A-Z)" << endl;
    cout << "3. Listar por hierarquia de cargo" << endl;
    cout << "4. Listar por setor" << endl;
    cout << "5. Listar por setor e cargo" << endl;
    cout << "0. Voltar" << endl;
    cout << "Opção: ";
}

int Interface::selecionarSetorInterativo(const SistemaRH& sistema) {
    const auto& setores = sistema.getSetores();
    
    cout << "\n=== Selecionar Setor ===" << endl;
    for (size_t i = 0; i < setores.size(); i++) {
        cout << i + 1 << ". " << setores[i].getNome() << endl;
    }
    cout << "0. Cancelar" << endl;
    cout << "Opção: ";
    
    string entrada;
    getline(cin, entrada);
    
    if (entrada == "0") return -1;
    
    try {
        int opcao = stoi(entrada) - 1;
        if (opcao >= 0 && opcao < static_cast<int>(setores.size())) {
            return opcao;
        }
    } catch (const exception&) {
        // Entrada inválida
    }
    
    cout << "Opção inválida." << endl;
    return -1;
}

int Interface::selecionarCargoInterativo(const Setor& setor) {
    const auto& cargos = setor.getCargos();
    
    cout << "\n=== Selecionar Cargo - " << setor.getNome() << " ===" << endl;
    for (size_t i = 0; i < cargos.size(); i++) {
        cout << i + 1 << ". " << cargos[i].getNome() << endl;
    }
    cout << "0. Cancelar" << endl;
    cout << "Opção: ";
    
    string entrada;
    getline(cin, entrada);
    
    if (entrada == "0") return -1;
    
    try {
        int opcao = stoi(entrada) - 1;
        if (opcao >= 0 && opcao < static_cast<int>(cargos.size())) {
            return opcao;
        }
    } catch (const exception&) {
        // Entrada inválida
    }
    
    cout << "Opção inválida." << endl;
    return -1;
}

void Interface::exibirSubmenuPontos() {
    cout << "\n=== GESTÃO DE PONTOS ===" << endl;
    cout << "1. Registrar entrada" << endl;
    cout << "2. Registrar saída" << endl;
    cout << "3. Registrar ponto manual" << endl;
    cout << "4. Consultar pontos por funcionário" << endl;
    cout << "5. Consultar pontos por data" << endl;
    cout << "6. Relatório mensal" << endl;
    cout << "7. Calcular horas trabalhadas" << endl;
    cout << "0. Voltar" << endl;
    cout << "Opção: ";
}

void Interface::exibirCabecalho(const string& titulo) {
    exibirSeparador();
    cout << "   " << titulo << endl;
    exibirSeparador();
}

void Interface::exibirSeparador() {
    cout << string(50, '=') << endl;
}

void Interface::pausar() {
    cout << "\nPressione ENTER para continuar...";
    cin.get();
}

int Interface::selecionarFuncionarioInterativo(const SistemaRH& sistema) {
    const auto& funcionarios = sistema.getFuncionarios();
    
    if (funcionarios.empty()) {
        exibirMensagemInfo("Nenhum funcionário cadastrado.");
        return -1;
    }
    
    cout << "\n=== Selecionar Funcionário ===" << endl;
    cout << setw(5) << "ID" << " | " << setw(30) << left << "Nome" << " | " << "Setor" << endl;
    cout << string(60, '-') << endl;
    
    for (const auto& func : funcionarios) {
        cout << setw(5) << func.getID() << " | "
             << setw(30) << left << func.getNome() << " | "
             << func.getSetor() << endl;
    }
    
    cout << "\nDigite o ID do funcionário (0 para cancelar): ";
    string entrada;
    getline(cin, entrada);
    
    if (entrada == "0") return -1;
    
    try {
        int id = stoi(entrada);
        const Funcionario* func = sistema.buscarFuncionarioPorId(id);
        if (func) {
            return id;
        } else {
            exibirMensagemErro("Funcionário não encontrado.");
            return -1;
        }
    } catch (const exception&) {
        exibirMensagemErro("ID inválido.");
        return -1;
    }
}

bool Interface::confirmarAcao(const string& mensagem) {
    cout << mensagem << " (s/N): ";
    string resposta;
    getline(cin, resposta);
    return (resposta == "s" || resposta == "S" || resposta == "sim" || resposta == "Sim");
}

void Interface::exibirMensagemSucesso(const string& mensagem) {
    cout << "\n✓ " << mensagem << endl;
}

void Interface::exibirMensagemErro(const string& mensagem) {
    cout << "\n✗ ERRO: " << mensagem << endl;
}

void Interface::exibirMensagemInfo(const string& mensagem) {
    cout << "\nℹ " << mensagem << endl;
}

void Interface::limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

string Interface::formatarData(const string& data) {
    if (data.length() == 10) {
        return data; // Já está no formato DD/MM/AAAA
    }
    return data;
}

string Interface::formatarHora(const string& hora) {
    if (hora.length() == 8) {
        return hora; // Já está no formato HH:MM:SS
    }
    return hora;
}

string Interface::formatarCPF(const string& cpf) {
    if (cpf.length() == 11) {
        return cpf.substr(0,3) + "." + cpf.substr(3,3) + "." + cpf.substr(6,3) + "-" + cpf.substr(9,2);
    }
    return cpf;
}

string Interface::formatarID(int id) {
    ostringstream oss;
    oss << setfill('0') << setw(5) << id;
    return oss.str();
}