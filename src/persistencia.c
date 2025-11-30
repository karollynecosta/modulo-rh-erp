#include "../include/persistencia.h"
#include "../include/funcionario.h"
#include <stdio.h>
#include <string.h>

void persistencia_importar_de_arquivo(SistemaRH *sistema, const char *arquivo) {
    if (!sistema || !arquivo) return;
    
    FILE *fp = fopen(arquivo, "r");
    if (!fp) return;
    
    char linha[500];
    sistema->num_funcionarios = 0;
    
    while (fgets(linha, sizeof(linha), fp) && sistema->num_funcionarios < MAX_FUNCIONARIOS) {
        // Remove quebra de linha
        size_t len = strlen(linha);
        if (len > 0 && linha[len-1] == '\n') {
            linha[len-1] = '\0';
        }
        
        char *nome = strtok(linha, ";");
        char *cpf = strtok(NULL, ";");
        char *id_str = strtok(NULL, ";");
        char *setor = strtok(NULL, ";");
        char *cargo = strtok(NULL, ";");
        char *foto = strtok(NULL, ";");
        
        if (nome && cpf && id_str && setor && cargo) {
            int id = atoi(id_str);
            funcionario_init(&sistema->funcionarios[sistema->num_funcionarios],
                           nome, cpf, id, setor, cargo, foto ? foto : "");
            sistema->num_funcionarios++;
        }
    }
    
    fclose(fp);
}

void persistencia_exportar_para_arquivo(const SistemaRH *sistema, const char *arquivo) {
    if (!sistema || !arquivo) return;
    
    FILE *fp = fopen(arquivo, "w");
    if (!fp) return;
    
    char buffer[500];
    for (int i = 0; i < sistema->num_funcionarios; i++) {
        funcionario_formatar_para_arquivo(&sistema->funcionarios[i], buffer);
        fprintf(fp, "%s\n", buffer);
    }
    
    fclose(fp);
}

void persistencia_importar_pontos(SistemaRH *sistema, const char *arquivo) {
    if (!sistema || !arquivo) return;
    
    FILE *fp = fopen(arquivo, "r");
    if (!fp) return;
    
    char linha[300];
    sistema->num_pontos = 0;
    
    while (fgets(linha, sizeof(linha), fp) && sistema->num_pontos < MAX_PONTOS) {
        // Remove quebra de linha
        size_t len = strlen(linha);
        if (len > 0 && linha[len-1] == '\n') {
            linha[len-1] = '\0';
        }
        
        // Formato: ID_FUNCIONARIO;DATA;HORA;TIPO;OBSERVACAO
        char *id_str = strtok(linha, ";");
        char *data = strtok(NULL, ";");
        char *hora = strtok(NULL, ";");
        char *tipo = strtok(NULL, ";");
        char *observacao = strtok(NULL, ";");
        
        if (id_str && data && hora && tipo) {
            int id_funcionario = atoi(id_str);
            
            sistema->pontos[sistema->num_pontos].id_funcionario = id_funcionario;
            
            strncpy(sistema->pontos[sistema->num_pontos].data, data, 10);
            sistema->pontos[sistema->num_pontos].data[10] = '\0';
            
            strncpy(sistema->pontos[sistema->num_pontos].hora, hora, 8);
            sistema->pontos[sistema->num_pontos].hora[8] = '\0';
            
            strncpy(sistema->pontos[sistema->num_pontos].tipo, tipo, 9);
            sistema->pontos[sistema->num_pontos].tipo[9] = '\0';
            
            strncpy(sistema->pontos[sistema->num_pontos].observacao, 
                   observacao ? observacao : "", MAX_STRING_LENGTH - 1);
            sistema->pontos[sistema->num_pontos].observacao[MAX_STRING_LENGTH - 1] = '\0';
            
            sistema->num_pontos++;
        }
    }
    
    fclose(fp);
}

void persistencia_exportar_pontos(const SistemaRH *sistema, const char *arquivo) {
    if (!sistema || !arquivo) return;
    
    FILE *fp = fopen(arquivo, "w");
    if (!fp) return;
    
    for (int i = 0; i < sistema->num_pontos; i++) {
        fprintf(fp, "%d;%s;%s;%s;%s\n",
                sistema->pontos[i].id_funcionario,
                sistema->pontos[i].data,
                sistema->pontos[i].hora,
                sistema->pontos[i].tipo,
                sistema->pontos[i].observacao);
    }
    
    fclose(fp);
}