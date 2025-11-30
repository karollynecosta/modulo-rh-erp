#ifndef SISTEMA_RH_H
#define SISTEMA_RH_H

#include "types.h"

// Funções do sistema principal
void sistema_rh_init(SistemaRH *sistema);
void sistema_rh_inicializar_setores(SistemaRH *sistema);
int sistema_rh_cargo_rank(const char *cargo);
Funcionario* sistema_rh_buscar_funcionario_por_id(SistemaRH *sistema, int id);
const Funcionario* sistema_rh_buscar_funcionario_por_id_const(const SistemaRH *sistema, int id);

#endif