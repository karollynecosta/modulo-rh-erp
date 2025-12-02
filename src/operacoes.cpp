#include "../include/operacoes.h"
#include "../include/validacao.h"
#include "../include/ponto.h"
#include "../include/interface.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

void OperacoesRH::cadastrarFuncionario(SistemaRH& sistema) {
    Interface::exibirCabecalho("CADASTRAR FUNCIONÁRIO");
    
    // Obter dados do funcionário
    string nome = Validacao::entradaString("Nome: ", &FuncionarioValidacao::nomeValido);
    if (nome == "0") return;
    
    string cpf = Validacao::entradaString("CPF (11 dígitos): ", &FuncionarioValidacao::cpfValido);
    if (cpf == "0") return;
    
    int id = Validacao::entradaID("ID (5 dígitos): ", sistema, -1);
    if (id == 0) return;
    
    int idxSetor = Interface::selecionarSetorInterativo(sistema);
    if (idxSetor == -1) return;
    
    const auto& setores = sistema.getSetores();
    int idxCargo = Interface::selecionarCargoInterativo(setores[idxSetor]);
    if (idxCargo == -1) return;
    
    string foto = Validacao::entradaString("Foto (opcional): ", nullptr, false);
    if (foto == "0") foto = "";
    
    // Criar funcionário
    Funcionario novoFunc(id, nome, Interface::formatarCPF(cpf), setores[idxSetor].getNome(),
                        setores[idxSetor].getCargos()[idxCargo].getNome(), foto);
    sistema.adicionarFuncionario(novoFunc);
    
    Interface::exibirMensagemSucesso("Funcionário cadastrado com sucesso!");
}

void OperacoesRH::buscarFuncionario(const SistemaRH& sistema) {
    Interface::exibirCabecalho("BUSCAR FUNCIONÁRIO");
    
    int id = Interface::selecionarFuncionarioInterativo(sistema);
    if (id == -1) return;
    
    const Funcionario* func = sistema.buscarFuncionarioPorId(id);
    if (func) {
        cout << "\n";
        Interface::exibirSeparador();
        cout << "Nome: " << func->getNome() << endl;
        cout << "CPF: " << Interface::formatarCPF(func->getCPF()) << endl;
        cout << "ID: " << Interface::formatarID(func->getID()) << endl;
        cout << "Setor: " << func->getSetor() << endl;
        cout << "Cargo: " << func->getCargo() << endl;
        cout << "Foto: " << (func->getFoto().empty() ? "Não cadastrada" : func->getFoto()) << endl;
        Interface::exibirSeparador();
    } else {
        Interface::exibirMensagemErro("Funcionário não encontrado.");
    }
}

void OperacoesRH::editarFuncionario(SistemaRH& sistema) {
    cout << "\n=== Editar Funcionário ===" << endl;
    
    int id = Validacao::entradaIDExistente("ID do funcionário: ", sistema);
    if (id == 0) return;
    
    Funcionario* func = sistema.buscarFuncionarioPorId(id);
    if (!func) {
        cout << "Funcionário não encontrado." << endl;
        return;
    }
    
    cout << "\nDados atuais:" << endl;
    func->exibirDados();
    
    cout << "\n=== Editar Dados (deixe em branco para manter valor atual) ===" << endl;
    
    string nome = Validacao::entradaString("Novo nome: ", &FuncionarioValidacao::nomeValido, false);
    if (nome == "0") return;
    if (!nome.empty()) func->setNome(nome);
    
    string cpf = Validacao::entradaString("Novo CPF: ", &FuncionarioValidacao::cpfValido, false);
    if (cpf == "0") return;
    if (!cpf.empty()) func->setCPF(cpf);
    
    string foto = Validacao::entradaString("Nova foto: ", nullptr, false);
    if (foto == "0") return;
    if (!foto.empty()) func->setFoto(foto);
    
    cout << "Funcionário editado com sucesso!" << endl;
}

void OperacoesRH::excluirFuncionario(SistemaRH& sistema) {
    Interface::exibirCabecalho("EXCLUIR FUNCIONÁRIO");
    
    int id = Interface::selecionarFuncionarioInterativo(sistema);
    if (id == -1) return;
    
    const Funcionario* func = sistema.buscarFuncionarioPorId(id);
    if (!func) {
        Interface::exibirMensagemErro("Funcionário não encontrado.");
        return;
    }
    
    cout << "\nDados do funcionário a ser excluído:" << endl;
    cout << "Nome: " << func->getNome() << endl;
    cout << "CPF: " << Interface::formatarCPF(func->getCPF()) << endl;
    cout << "ID: " << Interface::formatarID(func->getID()) << endl;
    cout << "Setor: " << func->getSetor() << " / " << func->getCargo() << endl;
    
    if (Interface::confirmarAcao("Tem certeza que deseja excluir este funcionário?")) {
        sistema.removerFuncionario(id);
        Interface::exibirMensagemSucesso("Funcionário excluído com sucesso!");
    } else {
        Interface::exibirMensagemInfo("Operação cancelada.");
    }
}

void OperacoesRH::baterPonto(SistemaRH& sistema) {
    Interface::exibirCabecalho("BATER PONTO");
    
    int id = Interface::selecionarFuncionarioInterativo(sistema);
    if (id == -1) return;
    
    const Funcionario* func = sistema.buscarFuncionarioPorId(id);
    if (!func) {
        Interface::exibirMensagemErro("Funcionário não encontrado.");
        return;
    }
    
    cout << "\nFuncionário: " << func->getNome() << endl;
    cout << "1. Registrar Entrada" << endl;
    cout << "2. Registrar Saída" << endl;
    cout << "0. Cancelar" << endl;
    cout << "Opção: ";
    
    string opcao;
    getline(cin, opcao);
    
    if (opcao == "0") return;
    
    string observacao = Validacao::entradaString("Observação (opcional): ", nullptr, false);
    if (observacao == "0") observacao = "";
    
    if (opcao == "1") {
        GerenciadorPonto::registrarEntrada(sistema, id, observacao);
    } else if (opcao == "2") {
        GerenciadorPonto::registrarSaida(sistema, id, observacao);
    } else {
        Interface::exibirMensagemErro("Opção inválida.");
    }
}

void OperacoesRH::registrarPontoManual(SistemaRH& sistema) {
    cout << "\n=== Registrar Ponto Manual ===" << endl;
    
    int id = Validacao::entradaIDExistente("ID do funcionário: ", sistema);
    if (id == 0) return;
    
    const Funcionario* func = sistema.buscarFuncionarioPorId(id);
    if (!func) {
        cout << "Funcionário não encontrado." << endl;
        return;
    }
    
    cout << "Funcionário: " << func->getNome() << endl;
    
    string data = Validacao::entradaString("Data (DD/MM/AAAA): ", &RegistroPontoValidacao::dataValida);
    if (data == "0") return;
    
    string hora = Validacao::entradaString("Hora (HH:MM:SS): ", &RegistroPontoValidacao::horaValida);
    if (hora == "0") return;
    
    cout << "1. Entrada" << endl;
    cout << "2. Saída" << endl;
    cout << "Tipo: ";
    
    string opcao;
    getline(cin, opcao);
    
    if (opcao == "0") return;
    
    string tipo = (opcao == "1") ? "ENTRADA" : "SAIDA";
    
    string observacao = Validacao::entradaString("Observação (opcional): ", nullptr, false);
    if (observacao == "0") observacao = "";
    
    GerenciadorPonto::registrarManual(sistema, id, data, hora, tipo, observacao);
    cout << "Ponto registrado com sucesso!" << endl;
}

void OperacoesRH::consultarPontos(const SistemaRH& sistema) {
    cout << "\n=== Consultar Pontos ===" << endl;
    cout << "1. Por funcionário" << endl;
    cout << "2. Por data" << endl;
    cout << "3. Todos os registros" << endl;
    cout << "4. Relatório mensal" << endl;
    cout << "Opção: ";
    
    string opcao;
    getline(cin, opcao);
    
    if (opcao == "0") return;
    
    if (opcao == "1") {
        int id = Validacao::entradaIDExistente("ID do funcionário: ", sistema);
        if (id == 0) return;
        GerenciadorPonto::listarPorFuncionario(sistema, id);
    } else if (opcao == "2") {
        string data = Validacao::entradaString("Data (DD/MM/AAAA): ", &RegistroPontoValidacao::dataValida);
        if (data == "0") return;
        GerenciadorPonto::listarPorData(sistema, data);
    } else if (opcao == "3") {
        GerenciadorPonto::listarTodos(sistema);
    } else if (opcao == "4") {
        int id = Validacao::entradaIDExistente("ID do funcionário: ", sistema);
        if (id == 0) return;
        
        cout << "Mês (1-12): ";
        string mesStr;
        getline(cin, mesStr);
        int mes = stoi(mesStr);
        
        cout << "Ano: ";
        string anoStr;
        getline(cin, anoStr);
        int ano = stoi(anoStr);
        
        GerenciadorPonto::relatorioMensal(sistema, id, mes, ano);
    } else {
        cout << "Opção inválida." << endl;
    }
}

void OperacoesRH::exibirFuncionarios(const SistemaRH& sistema) {
    cout << "\n=== Lista de Funcionários ===" << endl;
    
    const auto& funcionarios = sistema.getFuncionarios();
    if (funcionarios.empty()) {
        cout << "Nenhum funcionário cadastrado." << endl;
        return;
    }
    
    cout << "ID   | Nome                          | CPF          | Setor/Cargo" << endl;
    cout << "-----|-------------------------------|--------------|----------------------------" << endl;
    
    for (const auto& func : funcionarios) {
        cout << setw(4) << func.getID() << " | "
             << setw(29) << left << func.getNome() << " | "
             << func.getCPF() << " | " 
             << func.getSetor() << "/" << func.getCargo() << endl;
    }
}

void OperacoesRH::listarPorNome(const SistemaRH& sistema) {
    cout << "\n=== Funcionários por Nome (A-Z) ===" << endl;
    
    auto funcionarios = sistema.getFuncionarios();
    sort(funcionarios.begin(), funcionarios.end(), 
         [](const Funcionario& a, const Funcionario& b) {
             return a.getNome() < b.getNome();
         });
    
    for (const auto& func : funcionarios) {
        func.exibirDados();
        cout << "---" << endl;
    }
}

void OperacoesRH::listarPorCargoHierarquico(const SistemaRH& sistema) {
    cout << "\n=== Funcionários por Hierarquia de Cargo ===" << endl;
    
    auto funcionarios = sistema.getFuncionarios();
    sort(funcionarios.begin(), funcionarios.end(), 
         [&sistema](const Funcionario& a, const Funcionario& b) {
             return sistema.cargoRank(a.getCargo()) < sistema.cargoRank(b.getCargo());
         });
    
    for (const auto& func : funcionarios) {
        func.exibirDados();
        cout << "---" << endl;
    }
}

void OperacoesRH::listarPorSetorHierarquico(const SistemaRH& sistema) {
    cout << "\n=== Funcionários por Setor ===" << endl;
    
    const auto& setores = sistema.getSetores();
    for (const auto& setor : setores) {
        cout << "\n" << setor.getNome() << ":" << endl;
        cout << string(setor.getNome().length() + 1, '=') << endl;
        
        const auto& funcionarios = sistema.getFuncionarios();
        for (const auto& func : funcionarios) {
            if (func.getSetor() == setor.getNome()) {
                func.exibirDados();
                cout << "---" << endl;
            }
        }
    }
}

void OperacoesRH::listarPorSetorECargo(const SistemaRH& sistema) {
    cout << "\n=== Funcionários por Setor e Cargo ===" << endl;
    
    const auto& setores = sistema.getSetores();
    for (const auto& setor : setores) {
        cout << "\n" << setor.getNome() << ":" << endl;
        cout << string(setor.getNome().length() + 1, '=') << endl;
        
        const auto& cargos = setor.getCargos();
        for (const auto& cargo : cargos) {
            cout << "\n  " << cargo.getNome() << ":" << endl;
            
            const auto& funcionarios = sistema.getFuncionarios();
            bool encontrou = false;
            for (const auto& func : funcionarios) {
                if (func.getSetor() == setor.getNome() && func.getCargo() == cargo.getNome()) {
                    cout << "    " << func.getNome() << " (ID: " << func.getID() << ")" << endl;
                    encontrou = true;
                }
            }
            
            if (!encontrou) {
                cout << "    (Nenhum funcionário)" << endl;
            }
        }
    }
}