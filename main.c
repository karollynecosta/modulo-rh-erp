#include "include/sistema_rh.h"
#include "include/operacoes.h"
#include "include/interface.h"
#include "include/persistencia.h"
#include "include/validacao.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    SistemaRH sistema;
    
    // Inicializar sistema
    sistema_rh_init(&sistema);
    sistema_rh_inicializar_setores(&sistema);
    
    // Importar dados existentes
    persistencia_importar_de_arquivo(&sistema, "funcionarios.txt");
    persistencia_importar_pontos(&sistema, "pontos.txt");
    
    int opcao;
    
    do {
        interface_exibir_menu();
        
        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida.\n");
            validacao_limpar_entrada();
            continue;
        }
        validacao_limpar_entrada();
        
        switch (opcao) {
            case 1:
                operacoes_cadastrar_funcionario(&sistema);
                break;
                
            case 2: {
                int subopcao;
                do {
                    interface_exibir_submenu_listagens();
                    
                    if (scanf("%d", &subopcao) != 1) {
                        printf("Opcao invalida.\n");
                        validacao_limpar_entrada();
                        continue;
                    }
                    validacao_limpar_entrada();
                    
                    switch (subopcao) {
                        case 1:
                            operacoes_exibir_funcionarios(&sistema);
                            break;
                        case 2:
                            operacoes_listar_por_setor_hierarquico(&sistema);
                            break;
                        case 3:
                            operacoes_listar_por_nome(&sistema);
                            break;
                        case 4:
                            operacoes_listar_por_cargo_hierarquico(&sistema);
                            break;
                        case 5:
                            operacoes_listar_por_setor_e_cargo(&sistema);
                            break;
                        case 0:
                            printf("Voltando ao menu principal...\n");
                            break;
                        default:
                            printf("Opcao invalida.\n");
                    }
                } while (subopcao != 0);
                break;
            }
            
            case 3:
                operacoes_buscar_funcionario(&sistema);
                break;
                
            case 4:
                operacoes_editar_funcionario(&sistema);
                break;
                
            case 5:
                operacoes_excluir_funcionario(&sistema);
                break;
                
            case 6:
                operacoes_bater_ponto(&sistema);
                break;
                
            case 7:
                operacoes_registrar_ponto_manual(&sistema);
                break;
                
            case 8:
                operacoes_consultar_pontos(&sistema);
                break;
                
            case 9: {
                char confirma;
                printf("Deseja realmente salvar os dados no arquivo? (s/n): ");
                scanf(" %c", &confirma);
                validacao_limpar_entrada();
                
                if (confirma == 's' || confirma == 'S') {
                    persistencia_exportar_para_arquivo(&sistema, "funcionarios.txt");
                    persistencia_exportar_pontos(&sistema, "pontos.txt");
                    printf("Dados salvos com sucesso.\n");
                } else {
                    printf("Salvamento cancelado.\n");
                }
                break;
            }
            
            case 0: {
                printf("Deseja salvar antes de sair? (s/n): ");
                char salvar;
                scanf(" %c", &salvar);
                validacao_limpar_entrada();
                
                if (salvar == 's' || salvar == 'S') {
                    persistencia_exportar_para_arquivo(&sistema, "funcionarios.txt");
                    persistencia_exportar_pontos(&sistema, "pontos.txt");
                    printf("Dados salvos.\n");
                }
                printf("Encerrando programa...\n");
                break;
            }
            
            default:
                printf("Opcao invalida.\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}