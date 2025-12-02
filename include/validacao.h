#ifndef VALIDACAO_H
#define VALIDACAO_H

#include "sistema_rh.h"

// Classe para validação geral do sistema
class Validacao {
public:
    // Métodos gerais de validação
    static bool idValido(int id, const SistemaRH& sistema, int idAtual = -1);
    static bool idFormatoValido(const string& idStr);
    static bool funcionarioExiste(int id, const SistemaRH& sistema);
    
    // Métodos para entrada validada
    static string entradaString(const string& mensagem, bool (*validador)(const string&), bool obrigatorio = true);
    static int entradaID(const string& mensagem, const SistemaRH& sistema, int idAtual = -1);
    static int entradaIDExistente(const string& mensagem, const SistemaRH& sistema);
    static void limparEntrada();
    
private:
    Validacao() = default; // Classe utilitária - não deve ser instanciada
};

#endif