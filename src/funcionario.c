#include "../include/funcionario.h"
#include <stdio.h>
#include <string.h>

void funcionario_init(Funcionario *f, const char *nome, const char *cpf, int id, 
                     const char *setor, const char *cargo, const char *foto) {
    if (!f) return;
    
    strncpy(f->nome, nome ? nome : "", MAX_STRING_LENGTH - 1);
    f->nome[MAX_STRING_LENGTH - 1] = '\0';
    
    strncpy(f->cpf, cpf ? cpf : "", CPF_LENGTH);
    f->cpf[CPF_LENGTH] = '\0';
    
    f->id = id;
    
    strncpy(f->setor, setor ? setor : "", MAX_STRING_LENGTH - 1);
    f->setor[MAX_STRING_LENGTH - 1] = '\0';
    
    strncpy(f->cargo, cargo ? cargo : "", MAX_STRING_LENGTH - 1);
    f->cargo[MAX_STRING_LENGTH - 1] = '\0';
    
    strncpy(f->foto, foto ? foto : "", MAX_STRING_LENGTH - 1);
    f->foto[MAX_STRING_LENGTH - 1] = '\0';
}

void funcionario_set_nome(Funcionario *f, const char *nome) {
    if (!f || !nome || strlen(nome) == 0) return;
    strncpy(f->nome, nome, MAX_STRING_LENGTH - 1);
    f->nome[MAX_STRING_LENGTH - 1] = '\0';
}

void funcionario_set_cpf(Funcionario *f, const char *cpf) {
    if (!f || !cpf || strlen(cpf) != CPF_LENGTH) return;
    
    // Verifica se todos são dígitos
    for (int i = 0; i < CPF_LENGTH; i++) {
        if (!isdigit(cpf[i])) return;
    }
    
    strncpy(f->cpf, cpf, CPF_LENGTH);
    f->cpf[CPF_LENGTH] = '\0';
}

void funcionario_set_id(Funcionario *f, int id) {
    if (!f || id < ID_MIN || id > ID_MAX) return;
    f->id = id;
}

void funcionario_set_setor(Funcionario *f, const char *setor) {
    if (!f || !setor || strlen(setor) == 0) return;
    strncpy(f->setor, setor, MAX_STRING_LENGTH - 1);
    f->setor[MAX_STRING_LENGTH - 1] = '\0';
}

void funcionario_set_cargo(Funcionario *f, const char *cargo) {
    if (!f || !cargo || strlen(cargo) == 0) return;
    strncpy(f->cargo, cargo, MAX_STRING_LENGTH - 1);
    f->cargo[MAX_STRING_LENGTH - 1] = '\0';
}

void funcionario_set_foto(Funcionario *f, const char *foto) {
    if (!f) return;
    strncpy(f->foto, foto ? foto : "", MAX_STRING_LENGTH - 1);
    f->foto[MAX_STRING_LENGTH - 1] = '\0';
}

const char* funcionario_get_nome(const Funcionario *f) {
    return f ? f->nome : "";
}

const char* funcionario_get_cpf(const Funcionario *f) {
    return f ? f->cpf : "";
}

int funcionario_get_id(const Funcionario *f) {
    return f ? f->id : 0;
}

const char* funcionario_get_setor(const Funcionario *f) {
    return f ? f->setor : "";
}

const char* funcionario_get_cargo(const Funcionario *f) {
    return f ? f->cargo : "";
}

const char* funcionario_get_foto(const Funcionario *f) {
    return f ? f->foto : "";
}

void funcionario_exibir_dados(const Funcionario *f) {
    if (!f) return;
    
    printf("-----------------------------\n");
    printf("Nome   : %s\n", f->nome);
    printf("CPF    : %s\n", f->cpf);
    printf("ID     : %05d\n", f->id);
    printf("Setor  : %s\n", f->setor);
    printf("Cargo  : %s\n", f->cargo);
    printf("Foto   : %s\n", strlen(f->foto) > 0 ? f->foto : "Nao cadastrada");
    printf("-----------------------------\n");
}

void funcionario_formatar_para_arquivo(const Funcionario *f, char *buffer) {
    if (!f || !buffer) return;
    
    snprintf(buffer, 500, "%s;%s;%d;%s;%s;%s", 
             f->nome, f->cpf, f->id, f->setor, f->cargo, f->foto);
}