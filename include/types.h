#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 100
#define MAX_FUNCIONARIOS 1000
#define MAX_SETORES 20
#define MAX_CARGOS 20
#define MAX_PONTOS 10000
#define ID_MIN 1
#define ID_MAX 99999
#define CPF_LENGTH 11
#define ID_LENGTH 5

// Estrutura para representar um cargo
typedef struct {
    char nome[MAX_STRING_LENGTH];
} Cargo;

// Estrutura para representar um setor
typedef struct {
    char nome[MAX_STRING_LENGTH];
    Cargo cargos_permitidos[MAX_CARGOS];
    int num_cargos;
} Setor;

// Estrutura para representar um funcionário
typedef struct {
    char nome[MAX_STRING_LENGTH];
    char cpf[CPF_LENGTH + 1];
    int id;
    char setor[MAX_STRING_LENGTH];
    char cargo[MAX_STRING_LENGTH];
    char foto[MAX_STRING_LENGTH];
} Funcionario;

// Estrutura para representar um registro de ponto
typedef struct {
    int id_funcionario;
    char data[11];      // formato DD/MM/AAAA
    char hora[9];       // formato HH:MM:SS
    char tipo[10];      // "ENTRADA" ou "SAIDA"
    char observacao[MAX_STRING_LENGTH];
} RegistroPonto;

// Estrutura para o sistema de RH
typedef struct {
    Funcionario funcionarios[MAX_FUNCIONARIOS];
    int num_funcionarios;
    Setor setores[MAX_SETORES];
    int num_setores;
    RegistroPonto pontos[MAX_PONTOS];
    int num_pontos;
} SistemaRH;

#endif