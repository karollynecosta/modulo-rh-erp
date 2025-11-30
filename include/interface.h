#ifndef INTERFACE_H
#define INTERFACE_H

#include "types.h"

// Funções para interface do usuário
void interface_exibir_menu(void);
void interface_exibir_submenu_listagens(void);
int interface_selecionar_setor_interativo(const SistemaRH *sistema);
int interface_selecionar_cargo_interativo(const Setor *setor);

#endif