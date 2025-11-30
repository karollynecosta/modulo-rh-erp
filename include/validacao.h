#ifndef VALIDACAO_H
#define VALIDACAO_H

#include "types.h"

// Funções de validação
int validacao_nome_valido(const char *nome);
int validacao_cpf_valido(const char *cpf);
int validacao_id_valido(int id, const SistemaRH *sistema, int id_atual);
int validacao_id_formato_valido(const char *id_str);

// Funções para entrada validada
void validacao_entrada_string(const char *mensagem, char *buffer, 
                             int (*validador)(const char*), int obrigatorio);
int validacao_entrada_id(const char *mensagem, const SistemaRH *sistema, int id_atual);
void validacao_limpar_entrada(void);

#endif