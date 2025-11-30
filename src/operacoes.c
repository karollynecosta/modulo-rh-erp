#include "../include/operacoes.h"
#include "../include/funcionario.h"
#include "../include/validacao.h"
#include "../include/interface.h"
#include "../include/sistema_rh.h"
#include "../include/ponto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Funções auxiliares para ordenação
static int comparar_por_nome(const void *a, const void *b) {
    const Funcionario *f1 = (const Funcionario *)a;
    const Funcionario *f2 = (const Funcionario *)b;
    return strcmp(f1->nome, f2->nome);
}

static int comparar_por_cargo(const void *a, const void *b) {
    const Funcionario *f1 = (const Funcionario *)a;
    const Funcionario *f2 = (const Funcionario *)b;
    
    int rank1 = sistema_rh_cargo_rank(f1->cargo);
    int rank2 = sistema_rh_cargo_rank(f2->cargo);
    
    if (rank1 != rank2) {
        return rank1 - rank2;
    }
    return strcmp(f1->nome, f2->nome);
}

void operacoes_cadastrar_funcionario(SistemaRH *sistema) {
    if (!sistema || sistema->num_funcionarios >= MAX_FUNCIONARIOS) {
        printf("Limite de funcionarios atingido.\n");
        return;
    }
    
    char nome[MAX_STRING_LENGTH];
    char cpf[MAX_STRING_LENGTH];
    char foto[MAX_STRING_LENGTH];
    int id;
    
    // Nome
    validacao_entrada_string("Nome (ou 0 para cancelar): ", nome, validacao_nome_valido, 1);
    if (strcmp(nome, "0") == 0) {
        printf("Cadastro cancelado.\n");
        return;
    }
    
    // CPF
    validacao_entrada_string("CPF (11 digitos, ou 0 para cancelar): ", cpf, validacao_cpf_valido, 1);
    if (strcmp(cpf, "0") == 0) {
        printf("Cadastro cancelado.\n");
        return;
    }
    
    // ID
    id = validacao_entrada_id("ID (00001 a 99999, 5 digitos, ou 0 para cancelar): ", sistema, -1);
    if (id == 0) {
        printf("Cadastro cancelado.\n");
        return;
    }
    
    // Setor
    int idx_setor = interface_selecionar_setor_interativo(sistema);
    if (idx_setor == -1) {
        printf("Cadastro cancelado.\n");
        return;
    }
    
    // Cargo
    int idx_cargo = interface_selecionar_cargo_interativo(&sistema->setores[idx_setor]);
    if (idx_cargo == -1) {
        printf("Cadastro cancelado.\n");
        return;
    }
    
    // Foto (opcional)
    printf("Caminho da foto (ou deixe vazio): ");
    fflush(stdout);
    if (fgets(foto, sizeof(foto), stdin) != NULL) {
        size_t len = strlen(foto);
        if (len > 0 && foto[len-1] == '\n') {
            foto[len-1] = '\0';
        }
    } else {
        foto[0] = '\0';
    }
    
    // Criar funcionário
    funcionario_init(&sistema->funcionarios[sistema->num_funcionarios],
                    nome, cpf, id,
                    sistema->setores[idx_setor].nome,
                    sistema->setores[idx_setor].cargos_permitidos[idx_cargo].nome,
                    foto);
    
    sistema->num_funcionarios++;
    printf("Funcionario cadastrado com sucesso!\n");
}

void operacoes_buscar_funcionario(const SistemaRH *sistema) {
    if (!sistema) return;
    
    int id = validacao_entrada_id("Digite o ID do funcionario (5 digitos, ou 0 para cancelar): ", NULL, -1);
    if (id == 0) {
        printf("Busca cancelada.\n");
        return;
    }
    
    const Funcionario *f = sistema_rh_buscar_funcionario_por_id_const(sistema, id);
    if (f) {
        funcionario_exibir_dados(f);
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}

void operacoes_editar_funcionario(SistemaRH *sistema) {
    if (!sistema) return;
    
    int id = validacao_entrada_id("Digite o ID do funcionario a editar (5 digitos, ou 0 para cancelar): ", NULL, -1);
    if (id == 0) {
        printf("Edicao cancelada.\n");
        return;
    }
    
    Funcionario *f = sistema_rh_buscar_funcionario_por_id(sistema, id);
    if (!f) {
        printf("Funcionario nao encontrado.\n");
        return;
    }
    
    int opcao;
    char buffer[MAX_STRING_LENGTH];
    
    do {
        printf("\nEditar:\n");
        printf("1 - Nome\n");
        printf("2 - CPF\n");
        printf("3 - ID\n");
        printf("4 - Setor\n");
        printf("5 - Cargo\n");
        printf("6 - Foto\n");
        printf("0 - Concluir edicao\n");
        printf("Opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida.\n");
            validacao_limpar_entrada();
            continue;
        }
        validacao_limpar_entrada();
        
        switch (opcao) {
            case 1:
                validacao_entrada_string("Novo nome (ou 0 para cancelar): ", buffer, validacao_nome_valido, 1);
                if (strcmp(buffer, "0") != 0) {
                    funcionario_set_nome(f, buffer);
                    printf("Nome atualizado.\n");
                }
                break;
                
            case 2:
                validacao_entrada_string("Novo CPF (11 digitos, ou 0 para cancelar): ", buffer, validacao_cpf_valido, 1);
                if (strcmp(buffer, "0") != 0) {
                    funcionario_set_cpf(f, buffer);
                    printf("CPF atualizado.\n");
                }
                break;
                
            case 3: {
                int novo_id = validacao_entrada_id("Novo ID (5 digitos, ou 0 para cancelar): ", sistema, f->id);
                if (novo_id != 0) {
                    funcionario_set_id(f, novo_id);
                    printf("ID atualizado.\n");
                }
                break;
            }
            
            case 4: {
                int idx_setor = interface_selecionar_setor_interativo(sistema);
                if (idx_setor != -1) {
                    funcionario_set_setor(f, sistema->setores[idx_setor].nome);
                    printf("Setor atualizado.\n");
                }
                break;
            }
            
            case 5: {
                // Encontrar setor atual
                const Setor *setor_atual = NULL;
                for (int i = 0; i < sistema->num_setores; i++) {
                    if (strcmp(sistema->setores[i].nome, f->setor) == 0) {
                        setor_atual = &sistema->setores[i];
                        break;
                    }
                }
                
                if (setor_atual) {
                    int idx_cargo = interface_selecionar_cargo_interativo(setor_atual);
                    if (idx_cargo != -1) {
                        funcionario_set_cargo(f, setor_atual->cargos_permitidos[idx_cargo].nome);
                        printf("Cargo atualizado.\n");
                    }
                } else {
                    printf("Setor atual nao encontrado na lista.\n");
                }
                break;
            }
            
            case 6:
                printf("Novo caminho da foto (ou deixe vazio): ");
                fflush(stdout);
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    size_t len = strlen(buffer);
                    if (len > 0 && buffer[len-1] == '\n') {
                        buffer[len-1] = '\0';
                    }
                    funcionario_set_foto(f, buffer);
                    printf("Foto atualizada.\n");
                }
                break;
                
            case 0:
                printf("Edicao concluida.\n");
                break;
                
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

void operacoes_excluir_funcionario(SistemaRH *sistema) {
    if (!sistema) return;
    
    int id = validacao_entrada_id("Digite o ID do funcionario a excluir (5 digitos, ou 0 para cancelar): ", NULL, -1);
    if (id == 0) {
        printf("Exclusao cancelada.\n");
        return;
    }
    
    int pos = -1;
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        if (sistema->funcionarios[i].id == id) {
            pos = i;
            break;
        }
    }
    
    if (pos != -1) {
        printf("Funcionario encontrado:\n");
        funcionario_exibir_dados(&sistema->funcionarios[pos]);
        
        char confirma;
        printf("Tem certeza que deseja excluir este funcionario? (s/n): ");
        scanf(" %c", &confirma);
        validacao_limpar_entrada();
        
        if (confirma == 's' || confirma == 'S') {
            // Remove funcionário movendo todos os posteriores
            for (int i = pos; i < sistema->num_funcionarios - 1; i++) {
                sistema->funcionarios[i] = sistema->funcionarios[i + 1];
            }
            sistema->num_funcionarios--;
            printf("Funcionario excluido com sucesso.\n");
        } else {
            printf("Exclusao cancelada.\n");
        }
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}

void operacoes_exibir_funcionarios(const SistemaRH *sistema) {
    if (!sistema) return;
    
    if (sistema->num_funcionarios == 0) {
        printf("Nenhum funcionario cadastrado.\n");
        return;
    }
    
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        funcionario_exibir_dados(&sistema->funcionarios[i]);
    }
}

void operacoes_listar_por_nome(const SistemaRH *sistema) {
    if (!sistema || sistema->num_funcionarios == 0) {
        printf("Nenhum funcionario cadastrado.\n");
        return;
    }
    
    // Criar cópia para ordenação
    Funcionario *ordenados = malloc(sistema->num_funcionarios * sizeof(Funcionario));
    if (!ordenados) return;
    
    memcpy(ordenados, sistema->funcionarios, sistema->num_funcionarios * sizeof(Funcionario));
    qsort(ordenados, sistema->num_funcionarios, sizeof(Funcionario), comparar_por_nome);
    
    printf("\nFuncionarios em ordem alfabetica:\n");
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        funcionario_exibir_dados(&ordenados[i]);
    }
    
    free(ordenados);
}

void operacoes_listar_por_cargo_hierarquico(const SistemaRH *sistema) {
    if (!sistema || sistema->num_funcionarios == 0) {
        printf("Nenhum funcionario cadastrado.\n");
        return;
    }
    
    // Criar cópia para ordenação
    Funcionario *ordenados = malloc(sistema->num_funcionarios * sizeof(Funcionario));
    if (!ordenados) return;
    
    memcpy(ordenados, sistema->funcionarios, sistema->num_funcionarios * sizeof(Funcionario));
    qsort(ordenados, sistema->num_funcionarios, sizeof(Funcionario), comparar_por_cargo);
    
    printf("\nFuncionarios ordenados por cargo (hierarquia crescente):\n");
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        funcionario_exibir_dados(&ordenados[i]);
    }
    
    free(ordenados);
}

void operacoes_listar_por_setor_hierarquico(const SistemaRH *sistema) {
    if (!sistema) return;
    
    int idx_setor = interface_selecionar_setor_interativo(sistema);
    if (idx_setor == -1) {
        printf("Listagem cancelada.\n");
        return;
    }
    
    const char *setor_alvo = sistema->setores[idx_setor].nome;
    
    // Contar funcionários do setor
    int count = 0;
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        if (strcmp(sistema->funcionarios[i].setor, setor_alvo) == 0) {
            count++;
        }
    }
    
    if (count == 0) {
        printf("Nenhum funcionario encontrado no setor %s.\n", setor_alvo);
        return;
    }
    
    // Criar array filtrado
    Funcionario *filtrados = malloc(count * sizeof(Funcionario));
    if (!filtrados) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }
    int j = 0;
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        if (strcmp(sistema->funcionarios[i].setor, setor_alvo) == 0) {
            filtrados[j++] = sistema->funcionarios[i];
        }
    }
    
    qsort(filtrados, count, sizeof(Funcionario), comparar_por_cargo);
    
    printf("\nFuncionarios do setor %s ordenados por hierarquia crescente:\n", setor_alvo);
    for (int i = 0; i < count; i++) {
        funcionario_exibir_dados(&filtrados[i]);
    }
    
    free(filtrados);
}

void operacoes_listar_por_setor_e_cargo(const SistemaRH *sistema) {
    if (!sistema) return;
    
    int idx_setor = interface_selecionar_setor_interativo(sistema);
    if (idx_setor == -1) {
        printf("Listagem cancelada.\n");
        return;
    }
    
    int idx_cargo = interface_selecionar_cargo_interativo(&sistema->setores[idx_setor]);
    if (idx_cargo == -1) {
        printf("Listagem cancelada.\n");
        return;
    }
    
    const char *setor_alvo = sistema->setores[idx_setor].nome;
    const char *cargo_alvo = sistema->setores[idx_setor].cargos_permitidos[idx_cargo].nome;
    
    // Contar funcionários do setor e cargo
    int count = 0;
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        if (strcmp(sistema->funcionarios[i].setor, setor_alvo) == 0 &&
            strcmp(sistema->funcionarios[i].cargo, cargo_alvo) == 0) {
            count++;
        }
    }
    
    if (count == 0) {
        printf("Nenhum funcionario encontrado no setor %s com cargo %s.\n", setor_alvo, cargo_alvo);
        return;
    }
    
    printf("\nFuncionarios do setor %s com cargo %s:\n", setor_alvo, cargo_alvo);
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        if (strcmp(sistema->funcionarios[i].setor, setor_alvo) == 0 &&
            strcmp(sistema->funcionarios[i].cargo, cargo_alvo) == 0) {
            funcionario_exibir_dados(&sistema->funcionarios[i]);
        }
    }
}

// ---------------------- Operações de Ponto ----------------------

void operacoes_bater_ponto(SistemaRH *sistema) {
    if (!sistema) return;
    
    int id = validacao_entrada_id("Digite o ID do funcionario (5 digitos, ou 0 para cancelar): ", NULL, -1);
    if (id == 0) {
        printf("Operacao cancelada.\n");
        return;
    }
    
    int tipo_opcao;
    printf("\nTipo de ponto:\n");
    printf("1 - ENTRADA\n");
    printf("2 - SAIDA\n");
    printf("Opcao: ");
    
    if (scanf("%d", &tipo_opcao) != 1) {
        printf("Opcao invalida.\n");
        validacao_limpar_entrada();
        return;
    }
    validacao_limpar_entrada();
    
    char observacao[MAX_STRING_LENGTH] = "";
    printf("Observacao (opcional, ou deixe vazio): ");
    fflush(stdout);
    if (fgets(observacao, sizeof(observacao), stdin) != NULL) {
        size_t len = strlen(observacao);
        if (len > 0 && observacao[len-1] == '\n') {
            observacao[len-1] = '\0';
        }
    }
    
    switch (tipo_opcao) {
        case 1:
            ponto_registrar_entrada(sistema, id, observacao);
            break;
        case 2:
            ponto_registrar_saida(sistema, id, observacao);
            break;
        default:
            printf("Opcao invalida.\n");
    }
}

void operacoes_registrar_ponto_manual(SistemaRH *sistema) {
    if (!sistema) return;
    
    int id = validacao_entrada_id("Digite o ID do funcionario (5 digitos, ou 0 para cancelar): ", NULL, -1);
    if (id == 0) {
        printf("Operacao cancelada.\n");
        return;
    }
    
    char data[12], hora[10], tipo[10], observacao[MAX_STRING_LENGTH];
    
    // Data
    printf("Data (DD/MM/AAAA): ");
    fflush(stdout);
    if (fgets(data, sizeof(data), stdin) == NULL) {
        printf("Erro na entrada de dados.\n");
        return;
    }
    size_t len = strlen(data);
    if (len > 0 && data[len-1] == '\n') {
        data[len-1] = '\0';
    }
    
    // Hora
    printf("Hora (HH:MM:SS): ");
    fflush(stdout);
    if (fgets(hora, sizeof(hora), stdin) == NULL) {
        printf("Erro na entrada de dados.\n");
        return;
    }
    len = strlen(hora);
    if (len > 0 && hora[len-1] == '\n') {
        hora[len-1] = '\0';
    }
    
    // Tipo
    int tipo_opcao;
    printf("Tipo:\n");
    printf("1 - ENTRADA\n");
    printf("2 - SAIDA\n");
    printf("Opcao: ");
    
    if (scanf("%d", &tipo_opcao) != 1) {
        printf("Opcao invalida.\n");
        validacao_limpar_entrada();
        return;
    }
    validacao_limpar_entrada();
    
    if (tipo_opcao == 1) {
        strcpy(tipo, "ENTRADA");
    } else if (tipo_opcao == 2) {
        strcpy(tipo, "SAIDA");
    } else {
        printf("Tipo invalido.\n");
        return;
    }
    
    // Observação
    printf("Observacao (opcional): ");
    fflush(stdout);
    if (fgets(observacao, sizeof(observacao), stdin) != NULL) {
        len = strlen(observacao);
        if (len > 0 && observacao[len-1] == '\n') {
            observacao[len-1] = '\0';
        }
    }
    
    ponto_registrar_manual(sistema, id, data, hora, tipo, observacao);
}

void operacoes_consultar_pontos(const SistemaRH *sistema) {
    if (!sistema) return;
    
    int opcao;
    
    printf("\nConsultar pontos:\n");
    printf("1 - Por funcionario\n");
    printf("2 - Por data\n");
    printf("3 - Relatorio mensal\n");
    printf("4 - Todos os registros\n");
    printf("0 - Voltar\n");
    printf("Opcao: ");
    
    if (scanf("%d", &opcao) != 1) {
        printf("Opcao invalida.\n");
        validacao_limpar_entrada();
        return;
    }
    validacao_limpar_entrada();
    
    char buffer[MAX_STRING_LENGTH];
    int id, mes, ano;
    
    switch (opcao) {
        case 1:
            id = validacao_entrada_id("Digite o ID do funcionario: ", NULL, -1);
            if (id != 0) {
                ponto_listar_por_funcionario(sistema, id);
            }
            break;
            
        case 2:
            printf("Digite a data (DD/MM/AAAA): ");
            fflush(stdout);
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len-1] == '\n') {
                    buffer[len-1] = '\0';
                }
                ponto_listar_por_data(sistema, buffer);
            }
            break;
            
        case 3:
            id = validacao_entrada_id("Digite o ID do funcionario: ", NULL, -1);
            if (id == 0) break;
            
            printf("Digite o mes (1-12): ");
            if (scanf("%d", &mes) != 1) {
                printf("Mes invalido.\n");
                validacao_limpar_entrada();
                break;
            }
            
            printf("Digite o ano: ");
            if (scanf("%d", &ano) != 1) {
                printf("Ano invalido.\n");
                validacao_limpar_entrada();
                break;
            }
            validacao_limpar_entrada();
            
            ponto_relatorio_mensal(sistema, id, mes, ano);
            break;
            
        case 4:
            ponto_listar_todos(sistema);
            break;
            
        case 0:
            printf("Voltando...\n");
            break;
            
        default:
            printf("Opcao invalida.\n");
    }
}