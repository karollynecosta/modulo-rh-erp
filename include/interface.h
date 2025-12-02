#ifndef INTERFACE_H
#define INTERFACE_H

#include "sistema_rh.h"

class Interface {
public:
    // Funções para interface do usuário
    static void exibirMenu();
    static void exibirSubmenuListagens();
    static void exibirSubmenuPontos();
    static void exibirCabecalho(const string& titulo);
    static void exibirSeparador();
    static void pausar();
    
    // Seleções interativas
    static int selecionarSetorInterativo(const SistemaRH& sistema);
    static int selecionarCargoInterativo(const Setor& setor);
    static int selecionarFuncionarioInterativo(const SistemaRH& sistema);
    
    // Confirmações e alertas
    static bool confirmarAcao(const string& mensagem);
    static void exibirMensagemSucesso(const string& mensagem);
    static void exibirMensagemErro(const string& mensagem);
    static void exibirMensagemInfo(const string& mensagem);
    
    // Formatação
    static void limparTela();
    static string formatarData(const string& data);
    static string formatarHora(const string& hora);
    static string formatarCPF(const string& cpf);
    static string formatarID(int id);
};

#endif