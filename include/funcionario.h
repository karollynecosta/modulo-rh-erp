#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include "types.h"

// Funções para manipulação de funcionários
void funcionario_init(Funcionario *f, const char *nome, const char *cpf, int id, 
                     const char *setor, const char *cargo, const char *foto);
void funcionario_set_nome(Funcionario *f, const char *nome);
void funcionario_set_cpf(Funcionario *f, const char *cpf);
void funcionario_set_id(Funcionario *f, int id);
void funcionario_set_setor(Funcionario *f, const char *setor);
void funcionario_set_cargo(Funcionario *f, const char *cargo);
void funcionario_set_foto(Funcionario *f, const char *foto);

const char* funcionario_get_nome(const Funcionario *f);
const char* funcionario_get_cpf(const Funcionario *f);
int funcionario_get_id(const Funcionario *f);
const char* funcionario_get_setor(const Funcionario *f);
const char* funcionario_get_cargo(const Funcionario *f);
const char* funcionario_get_foto(const Funcionario *f);

void funcionario_exibir_dados(const Funcionario *f);
void funcionario_formatar_para_arquivo(const Funcionario *f, char *buffer);

#endif