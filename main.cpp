#include "include/sistema_rh.h"
#include "include/validacao.h"
#include "include/operacoes.h"
#include "include/persistencia.h"
#include "include/interface.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <limits>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <memory>

using namespace std;

// ---------------------- Classe base para polimorfismo ----------------------

class Pessoa {
protected:
    string nome;
    string cpf;
    
public:
    Pessoa(const string& nome, const string& cpf) : nome(nome), cpf(cpf) {}
    virtual ~Pessoa() = default;
    
    // Métodos virtuais puros para polimorfismo
    virtual void exibirDados() const = 0;
    virtual string formatarParaArquivo() const = 0;
    
    // Getters básicos
    virtual string getNome() const { return nome; }
    virtual string getCPF() const { return cpf; }
    
    // Setters básicos com validação
    virtual void setNome(const string& novoNome) {
        if (FuncionarioValidacao::nomeValido(novoNome)) {
            nome = FuncionarioValidacao::formatarNome(novoNome);
        }
    }
    
    virtual void setCPF(const string& novoCPF) {
        if (FuncionarioValidacao::cpfValido(novoCPF)) {
            cpf = novoCPF;
        }
    }
};

// ---------------------- Classe derivada que integra com a arquitetura existente ----------------------

class FuncionarioPoli : public Pessoa {
private:
    Funcionario funcionarioInterno;
public:
    FuncionarioPoli(const string& id, const string& nome, const string& cpf, 
                   const string& setor, const string& cargo, const string& foto = "")
        : Pessoa(nome, cpf), funcionarioInterno(stoi(id), nome, cpf, setor, cargo, foto) {}
    
    // Construtor a partir de Funcionario existente
    explicit FuncionarioPoli(const Funcionario& f) 
        : Pessoa(f.getNome(), f.getCPF()), funcionarioInterno(f) {}
    
    // Implementação dos métodos virtuais
    void exibirDados() const override {
        funcionarioInterno.exibirDados();
    }
    
    string formatarParaArquivo() const override {
        return funcionarioInterno.formatarParaArquivo();
    }
    
    // Getters específicos do funcionário
    int getID() const { return funcionarioInterno.getID(); }
    string getSetor() const { return funcionarioInterno.getSetor(); }
    string getCargo() const { return funcionarioInterno.getCargo(); }
    string getFoto() const { return funcionarioInterno.getFoto(); }
    
    // Setters específicos
    void setID(int id) { funcionarioInterno.setID(id); }
    void setSetor(const string& setor) { funcionarioInterno.setSetor(setor); }
    void setCargo(const string& cargo) { funcionarioInterno.setCargo(cargo); }
    void setFoto(const string& foto) { funcionarioInterno.setFoto(foto); }
    
    // Conversão para Funcionario original
    Funcionario toFuncionario() const {
        return funcionarioInterno;
    }
    
    // Sobrescrita dos setters base para manter sincronia
    void setNome(const string& novoNome) override {
        Pessoa::setNome(novoNome);
        funcionarioInterno.setNome(novoNome);
    }
    
    void setCPF(const string& novoCPF) override {
        Pessoa::setCPF(novoCPF);
        funcionarioInterno.setCPF(novoCPF);
    }
};

// ---------------------- Hierarquia global ----------------------

vector<string> hierarquia = {
    "Estagiario", "Auxiliar", "Assistente 2", "Assistente 1",
    "Analista Junior", "Analista Pleno", "Analista Senior",
    "Coordenador", "Gerente"
};

int cargoRank(const string& cargo) {
    for (size_t i = 0; i < hierarquia.size(); ++i) {
        if (hierarquia[i] == cargo) return (int)i;
    }
    return (int)hierarquia.size();
}

// ---------------------- Predicados simplificados para validação ----------------------

struct PredNome { 
    bool operator()(const string& s) const { 
        return FuncionarioValidacao::nomeValido(s); 
    } 
};

struct PredCPF { 
    bool operator()(const string& s) const { 
        return FuncionarioValidacao::cpfValido(s); 
    } 
};

struct PredFotoOpcional { 
    bool operator()(const string& s) const { 
        return FuncionarioValidacao::fotoValida(s); 
    } 
};

struct PredID5Digitos {
    bool operator()(const string& s) const {
        return Validacao::idFormatoValido(s);
    }
};

struct PredIDCadastro {
    const SistemaRH& sistema;
    PredIDCadastro(const SistemaRH& ref) : sistema(ref) {}
    bool operator()(const string& s) const {
        if (!Validacao::idFormatoValido(s)) return false;
        int id = atoi(s.c_str());
        return Validacao::idValido(id, sistema);
    }
};

// ---------------------- Função template para entrada validada ----------------------

template <typename Predicado>
string entradaValidada(const string& mensagem, const Predicado& validador, bool obrigatorio = true) {
    string entrada;
    do {
        cout << mensagem;
        getline(cin, entrada);
        if (entrada == "0") return "0";
        if (!obrigatorio && entrada.empty()) return "";
        if (validador(entrada)) return entrada;
        cout << "Entrada invalida. Tente novamente ou digite 0 para cancelar.\n";
    } while (true);
}

// ---------------------- Funções de listagem com polimorfismo ----------------------

void listarPorNomeExtendido(const SistemaRH& sistema) {
    const auto& funcionarios = sistema.getFuncionarios();
    if (funcionarios.empty()) { 
        cout << "Nenhum funcionario cadastrado.\n"; 
        return; 
    }
    
    vector<unique_ptr<Pessoa>> pessoas;
    for (const auto& f : funcionarios) {
        pessoas.push_back(make_unique<FuncionarioPoli>(f));
    }
    
    sort(pessoas.begin(), pessoas.end(), 
         [](const unique_ptr<Pessoa>& a, const unique_ptr<Pessoa>& b) {
             return a->getNome() < b->getNome();
         });
    
    cout << "\nFuncionarios em ordem alfabetica (polimorfismo):\n";
    for (const auto& p : pessoas) {
        p->exibirDados();
    }
}

void listarPorCargoHierarquicoExtendido(const SistemaRH& sistema) {
    const auto& funcionarios = sistema.getFuncionarios();
    if (funcionarios.empty()) { 
        cout << "Nenhum funcionario cadastrado.\n"; 
        return; 
    }
    
    vector<unique_ptr<FuncionarioPoli>> funcionariosPoli;
    for (const auto& f : funcionarios) {
        funcionariosPoli.push_back(make_unique<FuncionarioPoli>(f));
    }
    
    sort(funcionariosPoli.begin(), funcionariosPoli.end(), 
         [](const unique_ptr<FuncionarioPoli>& a, const unique_ptr<FuncionarioPoli>& b) {
             int ra = cargoRank(a->getCargo());
             int rb = cargoRank(b->getCargo());
             if (ra != rb) return ra < rb;
             return a->getNome() < b->getNome();
         });
    
    cout << "\nFuncionarios ordenados por cargo (hierarquia crescente - polimorfismo):\n";
    for (const auto& f : funcionariosPoli) {
        f->exibirDados(); // Demonstração de polimorfismo
    }
}

// ---------------------- Cadastro com interface melhorada ----------------------

void cadastrarFuncionarioExtendido(SistemaRH& sistema) {
    cout << "\n=== Cadastrar Funcionário ===" << endl;
    
    string nome = entradaValidada("Nome (ou 0 para cancelar): ", PredNome());
    if (nome == "0") { cout << "Cadastro cancelado.\n"; return; }

    string cpf = entradaValidada("CPF (11 digitos, ou 0 para cancelar): ", PredCPF());
    if (cpf == "0") { cout << "Cadastro cancelado.\n"; return; }

    string idStr = entradaValidada("ID (00001 a 99999, 5 digitos, ou 0 para cancelar): ", PredIDCadastro(sistema));
    if (idStr == "0") { cout << "Cadastro cancelado.\n"; return; }
    int id = atoi(idStr.c_str());

    int idxSetor = Interface::selecionarSetorInterativo(sistema);
    if (idxSetor == -1) { cout << "Cadastro cancelado.\n"; return; }

    const auto& setores = sistema.getSetores();
    int idxCargo = Interface::selecionarCargoInterativo(setores[idxSetor]);
    if (idxCargo == -1) { cout << "Cadastro cancelado.\n"; return; }

    string foto = entradaValidada("Caminho da foto (ou deixe vazio): ", PredFotoOpcional(), false);

    try {
        Funcionario novoFunc(
            id, nome, cpf,
            setores[idxSetor].getNome(),
            setores[idxSetor].getCargos()[idxCargo].getNome(),
            foto
        );
        sistema.adicionarFuncionario(novoFunc);
        cout << "Funcionario cadastrado com sucesso!\n";
    } catch (const exception& e) {
        cout << "Falha ao cadastrar: " << e.what() << "\n";
    }
}

// ---------------------- Menu principal integrado ----------------------

void exibirMenuIntegrado() {
    cout << "\n";
    Interface::exibirSeparador();
    cout << "           SISTEMA RH INTEGRADO" << endl;
    Interface::exibirSeparador();
    cout << "1 - Cadastrar funcionário" << endl;
    cout << "2 - Listar funcionários" << endl;
    cout << "3 - Buscar funcionário por ID" << endl;
    cout << "4 - Editar funcionário" << endl;
    cout << "5 - Excluir funcionário" << endl;
    cout << "6 - Bater ponto" << endl;
    cout << "7 - Registrar ponto manual" << endl;
    cout << "8 - Consultar pontos" << endl;
    cout << "9 - Importar/Exportar dados" << endl;
    cout << "0 - Sair" << endl;
    Interface::exibirSeparador();
    cout << "Selecione a opção desejada: ";
}

void exibirSubmenuListagensIntegrado() {
    cout << "\n--- LISTAGENS INTEGRADAS ---\n";
    cout << "1 - Listar todos (original)\n";
    cout << "2 - Listar por nome (original)\n";
    cout << "3 - Listar por nome (interface avançada)\n";
    cout << "4 - Listar por cargo hierarquico (original)\n";
    cout << "5 - Listar por cargo hierarquico (interface avançada)\n";
    cout << "6 - Listar por setor hierarquico\n";
    cout << "7 - Listar por setor e cargo\n";
    cout << "0 - Voltar\n";
    cout << "============================\n";
    cout << "Selecione a opcao: ";
}

void exibirSubmenuImportExport() {
    cout << "\n--- IMPORTAR/EXPORTAR ---\n";
    cout << "1 - Salvar dados atuais\n";
    cout << "2 - Importar funcionários de arquivo\n";
    cout << "3 - Importar pontos de arquivo\n";
    cout << "4 - Exportar funcionários para arquivo\n";
    cout << "5 - Exportar pontos para arquivo\n";
    cout << "0 - Voltar\n";
    cout << "=========================\n";
    cout << "Selecione a opcao: ";
}

int main() {
    SistemaRH sistema;
    sistema.inicializarSetores();
    
    // Exibir boas-vindas
    Interface::limparTela();
    Interface::exibirCabecalho("SISTEMA DE GESTÃO DE RECURSOS HUMANOS");
    cout << "Bem-vindo ao Sistema RH Integrado!" << endl;
    Interface::exibirSeparador();
    
    // Carregar dados existentes
    Persistencia::importarDeArquivo(sistema, "funcionarios.txt");
    Persistencia::importarPontos(sistema, "pontos.txt");
    
    if (sistema.getNumFuncionarios() > 0) {
        Interface::exibirMensagemInfo("Dados carregados com sucesso!");
    }
    
    string opcao;
    
    do {
        exibirMenuIntegrado();
        getline(cin, opcao);
        
        if (opcao == "1") {
            cadastrarFuncionarioExtendido(sistema);
        }
        else if (opcao == "2") {
            string subOpcao;
            do {
                exibirSubmenuListagensIntegrado();
                getline(cin, subOpcao);
                
                if (subOpcao == "1") {
                    OperacoesRH::exibirFuncionarios(sistema);
                }
                else if (subOpcao == "2") {
                    OperacoesRH::listarPorNome(sistema);
                }
                else if (subOpcao == "3") {
                    listarPorNomeExtendido(sistema);
                }
                else if (subOpcao == "4") {
                    OperacoesRH::listarPorCargoHierarquico(sistema);
                }
                else if (subOpcao == "5") {
                    listarPorCargoHierarquicoExtendido(sistema);
                }
                else if (subOpcao == "6") {
                    OperacoesRH::listarPorSetorHierarquico(sistema);
                }
                else if (subOpcao == "7") {
                    OperacoesRH::listarPorSetorECargo(sistema);
                }
                else if (subOpcao == "0") {
                    cout << "Voltando ao menu principal...\n";
                }
                else {
                    cout << "Opcao invalida.\n";
                }
            } while (subOpcao != "0");
        }
        else if (opcao == "3") {
            OperacoesRH::buscarFuncionario(sistema);
        }
        else if (opcao == "4") {
            OperacoesRH::editarFuncionario(sistema);
        }
        else if (opcao == "5") {
            OperacoesRH::excluirFuncionario(sistema);
        }
        else if (opcao == "6") {
            OperacoesRH::baterPonto(sistema);
        }
        else if (opcao == "7") {
            OperacoesRH::registrarPontoManual(sistema);
        }
        else if (opcao == "8") {
            OperacoesRH::consultarPontos(sistema);
        }
        else if (opcao == "9") {
            string subOpcao;
            do {
                exibirSubmenuImportExport();
                getline(cin, subOpcao);
                
                if (subOpcao == "1") {
                    try {
                        Persistencia::exportarParaArquivo(sistema, "funcionarios.txt");
                        Persistencia::exportarPontos(sistema, "pontos.txt");
                        cout << "Dados salvos com sucesso.\n";
                    } catch (const exception& e) {
                        cout << "Erro ao salvar: " << e.what() << "\n";
                    }
                }
                else if (subOpcao == "2") {
                    string arquivo;
                    cout << "Nome do arquivo para importar funcionários (ENTER para 'funcionarios.txt'): ";
                    getline(cin, arquivo);
                    if (arquivo.empty()) arquivo = "funcionarios.txt";
                    Persistencia::importarDeArquivo(sistema, arquivo);
                }
                else if (subOpcao == "3") {
                    string arquivo;
                    cout << "Nome do arquivo para importar pontos (ENTER para 'pontos.txt'): ";
                    getline(cin, arquivo);
                    if (arquivo.empty()) arquivo = "pontos.txt";
                    Persistencia::importarPontos(sistema, arquivo);
                }
                else if (subOpcao == "4") {
                    string arquivo;
                    cout << "Nome do arquivo para exportar funcionários (ENTER para 'funcionarios.txt'): ";
                    getline(cin, arquivo);
                    if (arquivo.empty()) arquivo = "funcionarios.txt";
                    Persistencia::exportarParaArquivo(sistema, arquivo);
                }
                else if (subOpcao == "5") {
                    string arquivo;
                    cout << "Nome do arquivo para exportar pontos (ENTER para 'pontos.txt'): ";
                    getline(cin, arquivo);
                    if (arquivo.empty()) arquivo = "pontos.txt";
                    Persistencia::exportarPontos(sistema, arquivo);
                }
                else if (subOpcao == "0") {
                    cout << "Voltando ao menu principal...\n";
                }
                else {
                    cout << "Opcao invalida.\n";
                }
            } while (subOpcao != "0");
        }
        else if (opcao == "0") {
            if (Interface::confirmarAcao("Deseja salvar antes de sair?")) {
                try {
                    Persistencia::exportarParaArquivo(sistema, "funcionarios.txt");
                    Persistencia::exportarPontos(sistema, "pontos.txt");
                    Interface::exibirMensagemSucesso("Dados salvos com sucesso!");
                } catch (const exception& e) {
                    Interface::exibirMensagemErro("Erro ao salvar: " + string(e.what()));
                }
            }
            Interface::exibirCabecalho("OBRIGADO POR USAR O SISTEMA RH!");
            cout << "Sistema encerrado com sucesso." << endl;
        }
        else {
            Interface::exibirMensagemErro("Opção inválida. Tente novamente.");
        }
        
        if (opcao != "0") {
            Interface::pausar();
        }
    } while (opcao != "0");

    return 0;
}