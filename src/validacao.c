#include "../include/validacao.h"
#include "../include/sistema_rh.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int validacao_nome_valido(const char *nome) {
    if (!nome || strlen(nome) == 0) return 0;
    
    for (int i = 0; nome[i]; i++) {
        char c = nome[i];
        if (!(isalpha(c) || isspace(c) || c == '-' || c == '\'')) {
            return 0;
        }
    }
    return 1;
}

int validacao_cpf_valido(const char *cpf) {
    if (!cpf || strlen(cpf) != CPF_LENGTH) return 0;
    
    for (int i = 0; i < CPF_LENGTH; i++) {
        if (!isdigit(cpf[i])) return 0;
    }
    return 1;
}

int validacao_id_valido(int id, const SistemaRH *sistema, int id_atual) {
    if (id < ID_MIN || id > ID_MAX) return 0;
    
    if (!sistema) return 1;
    
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        if (sistema->funcionarios[i].id == id && id != id_atual) {
            return 0;
        }
    }
    return 1;
}

int validacao_id_formato_valido(const char *id_str) {
    if (!id_str || strlen(id_str) != ID_LENGTH) return 0;
    
    for (int i = 0; i < ID_LENGTH; i++) {
        if (!isdigit(id_str[i])) return 0;
    }
    return 1;
}

void validacao_entrada_string(const char *mensagem, char *buffer, 
                             int (*validador)(const char*), int obrigatorio) {
    if (!mensagem || !buffer) return;
    
    while (1) {
        printf("%s", mensagem);
        fflush(stdout);
        
        if (fgets(buffer, MAX_STRING_LENGTH, stdin) == NULL) {
            strcpy(buffer, "0");
            return;
        }
        
        // Remove quebra de linha
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (strcmp(buffer, "0") == 0) return;
        
        if (!obrigatorio && strlen(buffer) == 0) return;
        
        if (!validador || validador(buffer)) return;
        
        printf("Entrada invalida. Tente novamente ou digite 0 para cancelar.\n");
    }
}

int validacao_entrada_id(const char *mensagem, const SistemaRH *sistema, int id_atual) {
    char buffer[MAX_STRING_LENGTH];
    
    while (1) {
        printf("%s", mensagem);
        fflush(stdout);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0; // Cancelado
        }
        
        // Remove quebra de linha
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (strcmp(buffer, "0") == 0) return 0; // Cancelado
        
        if (validacao_id_formato_valido(buffer)) {
            int id = atoi(buffer);
            if (validacao_id_valido(id, sistema, id_atual)) {
                return id;
            }
        }
        
        printf("ID invalido. Tente novamente ou digite 0 para cancelar.\n");
    }
}

void validacao_limpar_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}