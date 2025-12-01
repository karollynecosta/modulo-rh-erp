#ifndef PONTO_H
#define PONTO_H

#include "types.h"
#include <time.h>

// Funções para manipulação de registros de ponto
void ponto_init(RegistroPonto *p, int id_funcionario, const char *data, 
               const char *hora, const char *tipo, const char *observacao);

// Funções para registrar ponto
void ponto_registrar_entrada(SistemaRH *sistema, int id_funcionario, const char *observacao);
void ponto_registrar_saida(SistemaRH *sistema, int id_funcionario, const char *observacao);
void ponto_registrar_manual(SistemaRH *sistema, int id_funcionario, const char *data, 
                           const char *hora, const char *tipo, const char *observacao);

// Funções de consulta
void ponto_listar_por_funcionario(const SistemaRH *sistema, int id_funcionario);
void ponto_listar_por_data(const SistemaRH *sistema, const char *data);
void ponto_listar_todos(const SistemaRH *sistema);
void ponto_relatorio_mensal(const SistemaRH *sistema, int id_funcionario, int mes, int ano);
void ponto_calcular_horas_trabalhadas(const SistemaRH *sistema, int id_funcionario, int mes, int ano);
void ponto_listar_horas_diarias(const SistemaRH *sistema, int id_funcionario, int mes, int ano);

// Funções auxiliares
void ponto_obter_data_atual(char *data);
void ponto_obter_hora_atual(char *hora);
int ponto_validar_data(const char *data);
int ponto_validar_hora(const char *hora);
const char* ponto_get_ultimo_tipo(const SistemaRH *sistema, int id_funcionario, const char *data);
double ponto_converter_hora_para_decimal(const char *hora);
double ponto_calcular_horas_entre_horarios(const char *entrada, const char *saida);

#endif