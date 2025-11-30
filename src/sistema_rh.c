#include "../include/sistema_rh.h"
#include "../include/setor.h"
#include <string.h>

static const char* hierarquia[] = {
    "Estagiario", "Auxiliar", "Assistente 2", "Assistente 1",
    "Analista Junior", "Analista Pleno", "Analista Senior",
    "Coordenador", "Gerente"
};

static const int NUM_HIERARQUIA = sizeof(hierarquia) / sizeof(hierarquia[0]);

void sistema_rh_init(SistemaRH *sistema) {
    if (!sistema) return;
    
    sistema->num_funcionarios = 0;
    sistema->num_setores = 0;
    sistema->num_pontos = 0;
}

void sistema_rh_inicializar_setores(SistemaRH *sistema) {
    if (!sistema) return;
    
    const char* nomes_setores[] = {
        "Recursos Humanos", "Financeiro", "Producao", "Estoque",
        "Compras", "Vendas", "TI", "Manutencao",
        "Controle de Qualidade", "Garantia de Qualidade"
    };
    
    int num_setores_base = sizeof(nomes_setores) / sizeof(nomes_setores[0]);
    
    sistema->num_setores = 0;
    
    for (int i = 0; i < num_setores_base && i < MAX_SETORES; i++) {
        setor_init(&sistema->setores[i], nomes_setores[i]);
        
        // Adiciona todos os cargos da hierarquia a cada setor
        for (int j = 0; j < NUM_HIERARQUIA; j++) {
            setor_adicionar_cargo(&sistema->setores[i], hierarquia[j]);
        }
        
        sistema->num_setores++;
    }
}

int sistema_rh_cargo_rank(const char *cargo) {
    if (!cargo) return NUM_HIERARQUIA;
    
    for (int i = 0; i < NUM_HIERARQUIA; i++) {
        if (strcmp(hierarquia[i], cargo) == 0) {
            return i;
        }
    }
    return NUM_HIERARQUIA;
}

Funcionario* sistema_rh_buscar_funcionario_por_id(SistemaRH *sistema, int id) {
    if (!sistema) return NULL;
    
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        if (sistema->funcionarios[i].id == id) {
            return &sistema->funcionarios[i];
        }
    }
    return NULL;
}

const Funcionario* sistema_rh_buscar_funcionario_por_id_const(const SistemaRH *sistema, int id) {
    if (!sistema) return NULL;
    
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        if (sistema->funcionarios[i].id == id) {
            return &sistema->funcionarios[i];
        }
    }
    return NULL;
}