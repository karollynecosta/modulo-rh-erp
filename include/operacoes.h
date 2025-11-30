#ifndef OPERACOES_H
#define OPERACOES_H

#include "types.h"

// Operações principais do sistema
void operacoes_cadastrar_funcionario(SistemaRH *sistema);
void operacoes_buscar_funcionario(const SistemaRH *sistema);
void operacoes_editar_funcionario(SistemaRH *sistema);
void operacoes_excluir_funcionario(SistemaRH *sistema);

// Operações de ponto
void operacoes_bater_ponto(SistemaRH *sistema);
void operacoes_registrar_ponto_manual(SistemaRH *sistema);
void operacoes_consultar_pontos(const SistemaRH *sistema);

// Funções de listagem
void operacoes_exibir_funcionarios(const SistemaRH *sistema);
void operacoes_listar_por_nome(const SistemaRH *sistema);
void operacoes_listar_por_cargo_hierarquico(const SistemaRH *sistema);
void operacoes_listar_por_setor_hierarquico(const SistemaRH *sistema);
void operacoes_listar_por_setor_e_cargo(const SistemaRH *sistema);

#endif