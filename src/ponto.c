#include "../include/ponto.h"
#include "../include/funcionario.h"
#include "../include/sistema_rh.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void ponto_init(RegistroPonto *p, int id_funcionario, const char *data, 
               const char *hora, const char *tipo, const char *observacao) {
    if (!p) return;
    
    p->id_funcionario = id_funcionario;
    
    strncpy(p->data, data ? data : "", 10);
    p->data[10] = '\0';
    
    strncpy(p->hora, hora ? hora : "", 8);
    p->hora[8] = '\0';
    
    strncpy(p->tipo, tipo ? tipo : "", 9);
    p->tipo[9] = '\0';
    
    strncpy(p->observacao, observacao ? observacao : "", MAX_STRING_LENGTH - 1);
    p->observacao[MAX_STRING_LENGTH - 1] = '\0';
}

void ponto_obter_data_atual(char *data) {
    if (!data) return;
    
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(data, 11, "%d/%m/%Y", tm_info);
}

void ponto_obter_hora_atual(char *hora) {
    if (!hora) return;
    
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(hora, 9, "%H:%M:%S", tm_info);
}

int ponto_validar_data(const char *data) {
    if (!data || strlen(data) != 10) return 0;
    
    // Verificar formato DD/MM/AAAA
    if (data[2] != '/' || data[5] != '/') return 0;
    
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(data[i])) return 0;
    }
    
    // Validação básica de dia, mês
    int dia = (data[0] - '0') * 10 + (data[1] - '0');
    int mes = (data[3] - '0') * 10 + (data[4] - '0');
    
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12) return 0;
    
    return 1;
}

int ponto_validar_hora(const char *hora) {
    if (!hora || strlen(hora) != 8) return 0;
    
    // Verificar formato HH:MM:SS
    if (hora[2] != ':' || hora[5] != ':') return 0;
    
    for (int i = 0; i < 8; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(hora[i])) return 0;
    }
    
    // Validação básica de hora, minuto, segundo
    int h = (hora[0] - '0') * 10 + (hora[1] - '0');
    int m = (hora[3] - '0') * 10 + (hora[4] - '0');
    int s = (hora[6] - '0') * 10 + (hora[7] - '0');
    
    if (h > 23 || m > 59 || s > 59) return 0;
    
    return 1;
}

const char* ponto_get_ultimo_tipo(const SistemaRH *sistema, int id_funcionario, const char *data) {
    if (!sistema || !data) return "";
    
    const char *ultimo_tipo = "";
    char ultima_hora[9] = "00:00:00";
    
    for (int i = 0; i < sistema->num_pontos; i++) {
        if (sistema->pontos[i].id_funcionario == id_funcionario &&
            strcmp(sistema->pontos[i].data, data) == 0 &&
            strcmp(sistema->pontos[i].hora, ultima_hora) > 0) {
            strncpy(ultima_hora, sistema->pontos[i].hora, 8);
            ultima_hora[8] = '\0';
            ultimo_tipo = sistema->pontos[i].tipo;
        }
    }
    
    return ultimo_tipo;
}

void ponto_registrar_entrada(SistemaRH *sistema, int id_funcionario, const char *observacao) {
    if (!sistema || sistema->num_pontos >= MAX_PONTOS) return;
    
    // Verificar se funcionário existe
    if (!sistema_rh_buscar_funcionario_por_id_const(sistema, id_funcionario)) {
        printf("Funcionario nao encontrado!\n");
        return;
    }
    
    char data[11], hora[9];
    ponto_obter_data_atual(data);
    ponto_obter_hora_atual(hora);
    
    // Verificar último registro do dia
    const char *ultimo_tipo = ponto_get_ultimo_tipo(sistema, id_funcionario, data);
    if (strcmp(ultimo_tipo, "ENTRADA") == 0) {
        printf("AVISO: Ultimo registro foi de ENTRADA. Registrando nova entrada mesmo assim.\n");
    }
    
    ponto_init(&sistema->pontos[sistema->num_pontos], 
              id_funcionario, data, hora, "ENTRADA", observacao);
    sistema->num_pontos++;
    
    printf("Ponto de ENTRADA registrado com sucesso!\n");
    printf("Data: %s - Hora: %s\n", data, hora);
}

void ponto_registrar_saida(SistemaRH *sistema, int id_funcionario, const char *observacao) {
    if (!sistema || sistema->num_pontos >= MAX_PONTOS) return;
    
    // Verificar se funcionário existe
    if (!sistema_rh_buscar_funcionario_por_id_const(sistema, id_funcionario)) {
        printf("Funcionario nao encontrado!\n");
        return;
    }
    
    char data[11], hora[9];
    ponto_obter_data_atual(data);
    ponto_obter_hora_atual(hora);
    
    // Verificar último registro do dia
    const char *ultimo_tipo = ponto_get_ultimo_tipo(sistema, id_funcionario, data);
    if (strcmp(ultimo_tipo, "SAIDA") == 0) {
        printf("AVISO: Ultimo registro foi de SAIDA. Registrando nova saida mesmo assim.\n");
    }
    
    ponto_init(&sistema->pontos[sistema->num_pontos], 
              id_funcionario, data, hora, "SAIDA", observacao);
    sistema->num_pontos++;
    
    printf("Ponto de SAIDA registrado com sucesso!\n");
    printf("Data: %s - Hora: %s\n", data, hora);
}

void ponto_registrar_manual(SistemaRH *sistema, int id_funcionario, const char *data, 
                           const char *hora, const char *tipo, const char *observacao) {
    if (!sistema || sistema->num_pontos >= MAX_PONTOS) return;
    
    // Verificar se funcionário existe
    if (!sistema_rh_buscar_funcionario_por_id_const(sistema, id_funcionario)) {
        printf("Funcionario nao encontrado!\n");
        return;
    }
    
    if (!ponto_validar_data(data)) {
        printf("Data invalida! Use o formato DD/MM/AAAA\n");
        return;
    }
    
    if (!ponto_validar_hora(hora)) {
        printf("Hora invalida! Use o formato HH:MM:SS\n");
        return;
    }
    
    if (strcmp(tipo, "ENTRADA") != 0 && strcmp(tipo, "SAIDA") != 0) {
        printf("Tipo invalido! Use 'ENTRADA' ou 'SAIDA'\n");
        return;
    }
    
    ponto_init(&sistema->pontos[sistema->num_pontos], 
              id_funcionario, data, hora, tipo, observacao);
    sistema->num_pontos++;
    
    printf("Ponto manual registrado com sucesso!\n");
    printf("Data: %s - Hora: %s - Tipo: %s\n", data, hora, tipo);
}

void ponto_listar_por_funcionario(const SistemaRH *sistema, int id_funcionario) {
    if (!sistema) return;
    
    const Funcionario *f = sistema_rh_buscar_funcionario_por_id_const(sistema, id_funcionario);
    if (!f) {
        printf("Funcionario nao encontrado!\n");
        return;
    }
    
    printf("\n========== REGISTROS DE PONTO ==========\n");
    printf("Funcionario: %s (ID: %05d)\n", f->nome, f->id);
    printf("=========================================\n");
    
    int encontrou = 0;
    for (int i = 0; i < sistema->num_pontos; i++) {
        if (sistema->pontos[i].id_funcionario == id_funcionario) {
            printf("Data: %s | Hora: %s | %s\n", 
                   sistema->pontos[i].data,
                   sistema->pontos[i].hora,
                   sistema->pontos[i].tipo);
            if (strlen(sistema->pontos[i].observacao) > 0) {
                printf("  Obs: %s\n", sistema->pontos[i].observacao);
            }
            printf("-----------------------------------------\n");
            encontrou = 1;
        }
    }
    
    if (!encontrou) {
        printf("Nenhum registro encontrado para este funcionario.\n");
    }
}

void ponto_listar_por_data(const SistemaRH *sistema, const char *data) {
    if (!sistema || !data) return;
    
    if (!ponto_validar_data(data)) {
        printf("Data invalida! Use o formato DD/MM/AAAA\n");
        return;
    }
    
    printf("\n========== REGISTROS DE PONTO - %s ==========\n", data);
    
    int encontrou = 0;
    for (int i = 0; i < sistema->num_pontos; i++) {
        if (strcmp(sistema->pontos[i].data, data) == 0) {
            const Funcionario *f = sistema_rh_buscar_funcionario_por_id_const(
                sistema, sistema->pontos[i].id_funcionario);
            
            printf("ID: %05d | %s | %s | %s\n",
                   sistema->pontos[i].id_funcionario,
                   f ? f->nome : "Funcionario nao encontrado",
                   sistema->pontos[i].hora,
                   sistema->pontos[i].tipo);
            
            if (strlen(sistema->pontos[i].observacao) > 0) {
                printf("  Obs: %s\n", sistema->pontos[i].observacao);
            }
            printf("-----------------------------------------\n");
            encontrou = 1;
        }
    }
    
    if (!encontrou) {
        printf("Nenhum registro encontrado para esta data.\n");
    }
}

void ponto_listar_todos(const SistemaRH *sistema) {
    if (!sistema) return;
    
    if (sistema->num_pontos == 0) {
        printf("Nenhum registro de ponto encontrado.\n");
        return;
    }
    
    printf("\n========== TODOS OS REGISTROS DE PONTO ==========\n");
    
    for (int i = 0; i < sistema->num_pontos; i++) {
        const Funcionario *f = sistema_rh_buscar_funcionario_por_id_const(
            sistema, sistema->pontos[i].id_funcionario);
        
        printf("ID: %05d | %s | %s | %s | %s\n",
               sistema->pontos[i].id_funcionario,
               f ? f->nome : "Funcionario nao encontrado",
               sistema->pontos[i].data,
               sistema->pontos[i].hora,
               sistema->pontos[i].tipo);
        
        if (strlen(sistema->pontos[i].observacao) > 0) {
            printf("  Obs: %s\n", sistema->pontos[i].observacao);
        }
        printf("-----------------------------------------\n");
    }
}

void ponto_relatorio_mensal(const SistemaRH *sistema, int id_funcionario, int mes, int ano) {
    if (!sistema || mes < 1 || mes > 12 || ano < 2000) return;
    
    const Funcionario *f = sistema_rh_buscar_funcionario_por_id_const(sistema, id_funcionario);
    if (!f) {
        printf("Funcionario nao encontrado!\n");
        return;
    }
    
    printf("\n========== RELATORIO MENSAL ==========\n");
    printf("Funcionario: %s (ID: %05d)\n", f->nome, f->id);
    printf("Periodo: %02d/%04d\n", mes, ano);
    printf("=======================================\n");
    
    char mes_str[3], ano_str[5];
    snprintf(mes_str, sizeof(mes_str), "%02d", mes);
    snprintf(ano_str, sizeof(ano_str), "%04d", ano);
    
    int encontrou = 0;
    for (int i = 0; i < sistema->num_pontos; i++) {
        if (sistema->pontos[i].id_funcionario == id_funcionario) {
            // Verificar se é do mês/ano correto (formato DD/MM/AAAA)
            const char *data = sistema->pontos[i].data;
            if (strlen(data) == 10 && 
                data[3] == mes_str[0] && data[4] == mes_str[1] &&
                data[6] == ano_str[0] && data[7] == ano_str[1] && 
                data[8] == ano_str[2] && data[9] == ano_str[3]) {
                
                printf("Data: %s | Hora: %s | %s\n", 
                       sistema->pontos[i].data,
                       sistema->pontos[i].hora,
                       sistema->pontos[i].tipo);
                
                if (strlen(sistema->pontos[i].observacao) > 0) {
                    printf("  Obs: %s\n", sistema->pontos[i].observacao);
                }
                printf("-----------------------------------------\n");
                encontrou = 1;
            }
        }
    }
    
    if (!encontrou) {
        printf("Nenhum registro encontrado para este periodo.\n");
    }
}

double ponto_converter_hora_para_decimal(const char *hora) {
    if (!hora || !ponto_validar_hora(hora)) return 0.0;
    
    int h = (hora[0] - '0') * 10 + (hora[1] - '0');
    int m = (hora[3] - '0') * 10 + (hora[4] - '0');
    int s = (hora[6] - '0') * 10 + (hora[7] - '0');
    
    return h + (m / 60.0) + (s / 3600.0);
}

double ponto_calcular_horas_entre_horarios(const char *entrada, const char *saida) {
    if (!entrada || !saida) return 0.0;
    
    double h_entrada = ponto_converter_hora_para_decimal(entrada);
    double h_saida = ponto_converter_hora_para_decimal(saida);
    
    if (h_saida < h_entrada) return 0.0; // Hora inválida
    
    return h_saida - h_entrada;
}

void ponto_calcular_horas_trabalhadas(const SistemaRH *sistema, int id_funcionario, int mes, int ano) {
    if (!sistema || mes < 1 || mes > 12 || ano < 2000) return;
    
    const Funcionario *f = sistema_rh_buscar_funcionario_por_id_const(sistema, id_funcionario);
    if (!f) {
        printf("Funcionario nao encontrado!\n");
        return;
    }
    
    printf("\n========== HORAS TRABALHADAS ==========\n");
    printf("Funcionario: %s (ID: %05d)\n", f->nome, f->id);
    printf("Periodo: %02d/%04d\n", mes, ano);
    printf("=======================================\n");
    
    char mes_str[3], ano_str[5];
    snprintf(mes_str, sizeof(mes_str), "%02d", mes);
    snprintf(ano_str, sizeof(ano_str), "%04d", ano);
    
    double total_horas = 0.0;
    int dias_trabalhados = 0;
    
    // Organizar pontos por dia
    for (int dia = 1; dia <= 31; dia++) {
        char data_busca[12];
        snprintf(data_busca, sizeof(data_busca), "%02d/%s/%s", dia, mes_str, ano_str);
        
        char entrada[9] = "";
        char saida[9] = "";
        int tem_entrada = 0, tem_saida = 0;
        
        // Buscar primeira entrada e última saída do dia
        for (int i = 0; i < sistema->num_pontos; i++) {
            if (sistema->pontos[i].id_funcionario == id_funcionario &&
                strcmp(sistema->pontos[i].data, data_busca) == 0) {
                
                if (strcmp(sistema->pontos[i].tipo, "ENTRADA") == 0) {
                    if (!tem_entrada || strcmp(sistema->pontos[i].hora, entrada) < 0) {
                        strcpy(entrada, sistema->pontos[i].hora);
                        tem_entrada = 1;
                    }
                } else if (strcmp(sistema->pontos[i].tipo, "SAIDA") == 0) {
                    if (!tem_saida || strcmp(sistema->pontos[i].hora, saida) > 0) {
                        strcpy(saida, sistema->pontos[i].hora);
                        tem_saida = 1;
                    }
                }
            }
        }
        
        if (tem_entrada && tem_saida) {
            double horas_dia = ponto_calcular_horas_entre_horarios(entrada, saida);
            if (horas_dia > 0) {
                printf("Dia %02d: %s - %s = %.2f horas\n", dia, entrada, saida, horas_dia);
                total_horas += horas_dia;
                dias_trabalhados++;
            }
        } else if (tem_entrada || tem_saida) {
            printf("Dia %02d: Registro incompleto (%s%s %s%s)\n", 
                   dia, 
                   tem_entrada ? "Entrada:" : "", 
                   tem_entrada ? entrada : "",
                   tem_saida ? "Saida:" : "",
                   tem_saida ? saida : "");
        }
    }
    
    printf("=======================================\n");
    printf("Total de dias trabalhados: %d\n", dias_trabalhados);
    printf("Total de horas trabalhadas: %.2f horas\n", total_horas);
    if (dias_trabalhados > 0) {
        printf("Media de horas por dia: %.2f horas\n", total_horas / dias_trabalhados);
    }
}

void ponto_listar_horas_diarias(const SistemaRH *sistema, int id_funcionario, int mes, int ano) {
    if (!sistema || mes < 1 || mes > 12 || ano < 2000) return;
    
    const Funcionario *f = sistema_rh_buscar_funcionario_por_id_const(sistema, id_funcionario);
    if (!f) {
        printf("Funcionario nao encontrado!\n");
        return;
    }
    
    printf("\n========== DETALHAMENTO DIARIO ==========\n");
    printf("Funcionario: %s (ID: %05d)\n", f->nome, f->id);
    printf("Periodo: %02d/%04d\n", mes, ano);
    printf("==========================================\n");
    
    char mes_str[3], ano_str[5];
    snprintf(mes_str, sizeof(mes_str), "%02d", mes);
    snprintf(ano_str, sizeof(ano_str), "%04d", ano);
    
    for (int dia = 1; dia <= 31; dia++) {
        char data_busca[12];
        snprintf(data_busca, sizeof(data_busca), "%02d/%s/%s", dia, mes_str, ano_str);
        
        printf("\n--- Dia %02d (%s) ---\n", dia, data_busca);
        
        int encontrou_registros = 0;
        char primeira_entrada[9] = "";
        char ultima_saida[9] = "";
        int num_entradas = 0, num_saidas = 0;
        
        // Listar todos os pontos do dia
        for (int i = 0; i < sistema->num_pontos; i++) {
            if (sistema->pontos[i].id_funcionario == id_funcionario &&
                strcmp(sistema->pontos[i].data, data_busca) == 0) {
                
                printf("  %s - %s", sistema->pontos[i].hora, sistema->pontos[i].tipo);
                if (strlen(sistema->pontos[i].observacao) > 0) {
                    printf(" (%s)", sistema->pontos[i].observacao);
                }
                printf("\n");
                
                encontrou_registros = 1;
                
                if (strcmp(sistema->pontos[i].tipo, "ENTRADA") == 0) {
                    num_entradas++;
                    if (strlen(primeira_entrada) == 0 || 
                        strcmp(sistema->pontos[i].hora, primeira_entrada) < 0) {
                        strcpy(primeira_entrada, sistema->pontos[i].hora);
                    }
                } else if (strcmp(sistema->pontos[i].tipo, "SAIDA") == 0) {
                    num_saidas++;
                    if (strlen(ultima_saida) == 0 || 
                        strcmp(sistema->pontos[i].hora, ultima_saida) > 0) {
                        strcpy(ultima_saida, sistema->pontos[i].hora);
                    }
                }
            }
        }
        
        if (encontrou_registros) {
            if (strlen(primeira_entrada) > 0 && strlen(ultima_saida) > 0) {
                double horas = ponto_calcular_horas_entre_horarios(primeira_entrada, ultima_saida);
                printf("  TOTAL: %.2f horas (%s - %s)\n", horas, primeira_entrada, ultima_saida);
            }
            printf("  Entradas: %d | Saidas: %d\n", num_entradas, num_saidas);
        } else {
            printf("  Sem registros\n");
        }
    }
}