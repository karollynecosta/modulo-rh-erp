#include "../include/interface.h"
#include "../include/validacao.h"
#include <stdio.h>

void interface_exibir_menu(void) {
    printf("\n========== MENU ==========\n");
    printf("1 - Cadastrar novo funcionario\n");
    printf("2 - Listar funcionarios\n");
    printf("3 - Buscar funcionario por ID\n");
    printf("4 - Editar funcionario\n");
    printf("5 - Excluir funcionario\n");
    printf("6 - Bater ponto\n");
    printf("7 - Registrar ponto manual\n");
    printf("8 - Consultar pontos\n");
    printf("9 - Salvar dados\n");
    printf("0 - Sair\n");
    printf("==========================\n");
    printf("Selecione a opcao desejada: ");
}

void interface_exibir_submenu_listagens(void) {
    printf("\n--- LISTAGENS ---\n");
    printf("1 - Listar todos (simples)\n");
    printf("2 - Listar por setor e hierarquia\n");
    printf("3 - Listar por nome (alfabetica)\n");
    printf("4 - Listar por cargo (hierarquia)\n");
    printf("5 - Listar por setor e cargo\n");
    printf("0 - Voltar\n");
    printf("==================\n");
    printf("Selecione a opcao: ");
}

int interface_selecionar_setor_interativo(const SistemaRH *sistema) {
    if (!sistema) return -1;
    
    int escolha;
    
    while (1) {
        printf("\nSelecione o setor (ou 0 para cancelar):\n");
        for (int i = 0; i < sistema->num_setores; i++) {
            printf("%d - %s\n", i + 1, sistema->setores[i].nome);
        }
        printf("Opcao: ");
        
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada invalida. Digite um numero.\n");
            validacao_limpar_entrada();
            continue;
        }
        validacao_limpar_entrada();
        
        if (escolha == 0) return -1;
        
        if (escolha < 1 || escolha > sistema->num_setores) {
            printf("Opcao fora do intervalo. Tente novamente.\n");
            continue;
        }
        
        return escolha - 1;
    }
}

int interface_selecionar_cargo_interativo(const Setor *setor) {
    if (!setor) return -1;
    
    int escolha;
    
    while (1) {
        printf("\nSelecione o cargo (ou 0 para cancelar):\n");
        for (int i = 0; i < setor->num_cargos; i++) {
            printf("%d - %s\n", i + 1, setor->cargos_permitidos[i].nome);
        }
        printf("Opcao: ");
        
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada invalida. Digite um numero.\n");
            validacao_limpar_entrada();
            continue;
        }
        validacao_limpar_entrada();
        
        if (escolha == 0) return -1;
        
        if (escolha < 1 || escolha > setor->num_cargos) {
            printf("Opcao fora do intervalo. Tente novamente.\n");
            continue;
        }
        
        return escolha - 1;
    }
}