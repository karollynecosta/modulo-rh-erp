#ifndef SETOR_H
#define SETOR_H

#include "types.h"

// Funções para manipulação de setores
void setor_init(Setor *s, const char *nome);
void setor_adicionar_cargo(Setor *s, const char *nome_cargo);
const char* setor_get_nome(const Setor *s);
const Cargo* setor_get_cargos(const Setor *s);
int setor_get_num_cargos(const Setor *s);

// Funções para cargos
void cargo_init(Cargo *c, const char *nome);
const char* cargo_get_nome(const Cargo *c);

#endif