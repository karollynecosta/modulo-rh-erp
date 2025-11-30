#include "../include/setor.h"
#include <string.h>

void setor_init(Setor *s, const char *nome) {
    if (!s || !nome) return;
    
    strncpy(s->nome, nome, MAX_STRING_LENGTH - 1);
    s->nome[MAX_STRING_LENGTH - 1] = '\0';
    s->num_cargos = 0;
}

void setor_adicionar_cargo(Setor *s, const char *nome_cargo) {
    if (!s || !nome_cargo || s->num_cargos >= MAX_CARGOS) return;
    
    cargo_init(&s->cargos_permitidos[s->num_cargos], nome_cargo);
    s->num_cargos++;
}

const char* setor_get_nome(const Setor *s) {
    return s ? s->nome : "";
}

const Cargo* setor_get_cargos(const Setor *s) {
    return s ? s->cargos_permitidos : NULL;
}

int setor_get_num_cargos(const Setor *s) {
    return s ? s->num_cargos : 0;
}

void cargo_init(Cargo *c, const char *nome) {
    if (!c || !nome) return;
    
    strncpy(c->nome, nome, MAX_STRING_LENGTH - 1);
    c->nome[MAX_STRING_LENGTH - 1] = '\0';
}

const char* cargo_get_nome(const Cargo *c) {
    return c ? c->nome : "";
}