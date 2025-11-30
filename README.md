# 🏢 Sistema de Recursos Humanos - Módulo ERP

Um sistema completo de gerenciamento de recursos humanos desenvolvido em linguagem C, estruturado modularmente para fácil manutenção e extensão.

## 📋 Funcionalidades

### ✨ Gestão de Funcionários
- **Cadastro** completo com validação de dados
- **Busca** por ID com exibição detalhada
- **Edição** de informações individuais
- **Exclusão** com confirmação de segurança
- **Listagens** organizadas por diferentes critérios

### ⏰ Sistema de Ponto Eletrônico
- **Bater Ponto** automático com data/hora atual
- **Registro Manual** para correções e ajustes
- **Consultas** por funcionário, data ou período
- **Relatórios** mensais individualizados
- **Validação** de horários e controle de sequência

### 📊 Tipos de Listagem
1. **Listagem Simples** - Todos os funcionários
2. **Por Setor e Hierarquia** - Funcionários de um setor ordenados por cargo
3. **Alfabética** - Ordenação por nome
4. **Por Hierarquia** - Todos funcionários ordenados por cargo
5. **Por Setor e Cargo** - Filtro específico

### 💾 Persistência de Dados
- **Importação** automática na inicialização
- **Exportação** manual ou automática ao sair
- **Funcionários**: arquivo `funcionarios.txt`
- **Pontos**: arquivo `pontos.txt` 
- **Formato** de arquivo texto estruturado

## 🏗️ Arquitetura

### 📁 Estrutura do Projeto

```
modulo-rh-erp/
├── 📁 include/          # Arquivos de cabeçalho (.h)
│   ├── types.h          # Definições de tipos e estruturas
│   ├── funcionario.h    # Interface para funcionários
│   ├── setor.h          # Interface para setores e cargos
│   ├── ponto.h          # Interface para sistema de ponto
│   ├── validacao.h      # Funções de validação
│   ├── persistencia.h   # Salvar/carregar dados
│   ├── interface.h      # Menus e interface
│   ├── operacoes.h      # Operações CRUD
│   └── sistema_rh.h     # Gerenciamento do sistema
├── 📁 src/              # Implementações (.c)
│   ├── funcionario.c    # Lógica de funcionários
│   ├── setor.c          # Lógica de setores
│   ├── ponto.c          # Lógica do sistema de ponto
│   ├── validacao.c      # Validações de entrada
│   ├── persistencia.c   # I/O de arquivos
│   ├── interface.c      # Interface do usuário
│   ├── operacoes.c      # Operações principais
│   └── sistema_rh.c     # Core do sistema
├── 📁 build/            # Arquivos compilados
│   └── obj/             # Objetos intermediários
├── main.c               # Arquivo principal
├── compile.sh           # Script de compilação
├── Makefile             # Sistema de build
├── README.md            # Documentação
├── funcionarios.txt     # Base de dados funcionários (gerado)
└── pontos.txt           # Base de dados pontos (gerado)
```

### 🔍 **Entendendo a Organização include/ vs src/**

A separação entre `include/` e `src/` é uma **convenção padrão** em C que separa **declarações** de **implementações**:

#### 📁 **include/** - Arquivos de Cabeçalho (.h)
- **Contém**: Declarações de funções, estruturas, constantes, macros
- **Propósito**: Interface pública - o que outros arquivos podem usar
- **Exemplo**: `funcionario.h` declara que existe uma função `funcionario_init()`
- **Analogia**: É como um "cardápio" - mostra o que está disponível

#### 📁 **src/** - Código Fonte (.c)  
- **Contém**: Implementações reais das funções
- **Propósito**: Como as funções realmente funcionam por dentro
- **Exemplo**: `funcionario.c` tem o código real da função `funcionario_init()`
- **Analogia**: É como a "cozinha" - onde o trabalho real acontece

#### 🔗 **Como funciona:**

```c
// include/funcionario.h (DECLARAÇÃO)
void funcionario_init(Funcionario *f, ...);  // "Existe essa função"

// src/funcionario.c (IMPLEMENTAÇÃO)  
void funcionario_init(Funcionario *f, ...) { // "Aqui está o código real"
    // código real aqui
}

// main.c (USO)
#include "include/funcionario.h"  // Importa as declarações
// Agora pode usar funcionario_init()
```

#### 🎯 **Vantagens desta separação:**

1. **Organização**: Código mais limpo e estruturado
2. **Reutilização**: Headers podem ser incluídos em múltiplos arquivos
3. **Compilação**: Compilador sabe o que existe antes de compilar tudo
4. **Manutenção**: Mudanças na implementação não afetam quem usa
5. **Colaboração**: Diferentes pessoas podem trabalhar em diferentes partes

#### 💡 **Analogia Completa:**
- **include/** = Placa do restaurante (menu do que está disponível)
- **src/** = Cozinha do restaurante (onde a comida é realmente feita)
- **main.c** = Cliente (usa o menu para pedir, não precisa saber como cozinhar)

### 🏢 Setores e Hierarquia

**Setores Disponíveis:**
- Recursos Humanos
- Financeiro  
- Produção
- Estoque
- Compras
- Vendas
- TI
- Manutenção
- Controle de Qualidade
- Garantia de Qualidade

**Hierarquia de Cargos:**
1. Estagiário
2. Auxiliar
3. Assistente 2
4. Assistente 1
5. Analista Júnior
6. Analista Pleno
7. Analista Sênior
8. Coordenador
9. Gerente

## 🚀 Instalação e Uso

### ⚡ Início Rápido

```bash
# 1. Clone o repositório
git clone <URL_DO_REPOSITORIO>
cd modulo-rh-erp

# 2. Compilar (método mais simples)
gcc -Wall -Wextra -std=c99 -O2 -Iinclude src/*.c main.c -o sistema_rh

# 3. Executar
./sistema_rh
```

### 📋 Pré-requisitos
- Compilador GCC
- Sistema Linux/macOS/Windows com MinGW

### ⚡ Compilação via Linha de Comando

#### 🚀 Método Simples (um comando)
```bash
# Compilar tudo de uma vez
gcc -Wall -Wextra -std=c99 -Iinclude src/*.c main.c -o sistema_rh

# Com otimização (release)
gcc -Wall -Wextra -std=c99 -O2 -Iinclude src/*.c main.c -o sistema_rh

# Com debug
gcc -Wall -Wextra -std=c99 -g -Iinclude src/*.c main.c -o sistema_rh
```

#### 🏗️ Método Manual (passo a passo)
```bash
# 1. Criar diretório de build
mkdir -p build/obj

# 2. Compilar arquivos do src
gcc -Wall -Wextra -std=c99 -Iinclude -c src/funcionario.c -o build/obj/funcionario.o
gcc -Wall -Wextra -std=c99 -Iinclude -c src/setor.c -o build/obj/setor.o
gcc -Wall -Wextra -std=c99 -Iinclude -c src/validacao.c -o build/obj/validacao.o
gcc -Wall -Wextra -std=c99 -Iinclude -c src/persistencia.c -o build/obj/persistencia.o
gcc -Wall -Wextra -std=c99 -Iinclude -c src/interface.c -o build/obj/interface.o
gcc -Wall -Wextra -std=c99 -Iinclude -c src/sistema_rh.c -o build/obj/sistema_rh.o
gcc -Wall -Wextra -std=c99 -Iinclude -c src/operacoes.c -o build/obj/operacoes.o

# 3. Compilar main
gcc -Wall -Wextra -std=c99 -Iinclude -c main.c -o build/obj/main.o

# 4. Linkar tudo
gcc build/obj/*.o -o build/sistema_rh
```

#### ⚡ Script de Compilação Rápida
```bash
# Tornar o script executável (apenas uma vez)
chmod +x compile.sh

# Usar o script
./compile.sh           # Compilar em modo release
./compile.sh debug     # Compilar em modo debug  
./compile.sh run       # Compilar e executar
./compile.sh clean     # Limpar arquivos
./compile.sh help      # Mostrar ajuda
```

#### 🔧 Comandos Manuais Alternativos
```bash
# Criar script de build personalizado
echo '#!/bin/bash
mkdir -p build
gcc -Wall -Wextra -std=c99 -O2 -Iinclude src/*.c main.c -o build/sistema_rh
echo "Compilação concluída! Execute: ./build/sistema_rh"' > my_build.sh

# Usar o script personalizado
chmod +x my_build.sh
./my_build.sh
```

### 🎯 Execução

```bash
# Método 1: Se compilou na raiz
./sistema_rh

# Método 2: Se compilou no build/
./build/sistema_rh

# Método 3: Script completo
./compile.sh && ./build/sistema_rh
```

### 🧹 Limpeza Manual
```bash
# Remover arquivos compilados
rm -rf build/
rm -f sistema_rh
```

## 🔧 Funcionalidades Técnicas

### 🛡️ Validações Implementadas

- **Nome**: Apenas letras, espaços, hífen e apóstrofo
- **CPF**: Exatamente 11 dígitos numéricos
- **ID**: Formato 00001-99999, únicos no sistema
- **Entrada**: Proteção contra buffer overflow
- **Dados**: Validação em tempo real

### 💽 Formato de Dados

Arquivo `funcionarios.txt` com estrutura:
```
Nome;CPF;ID;Setor;Cargo;Foto
```

### 🏗️ Padrões Arquiteturais

- **Modularização**: Separação por responsabilidades
- **Encapsulamento**: Funções get/set para estruturas
- **Abstração**: Interfaces bem definidas
- **Reutilização**: Código compartilhado entre módulos

## 📚 API Principal

### 👤 Funcionários
```c
// Criação e configuração
void funcionario_init(Funcionario *f, ...);
void funcionario_set_nome(Funcionario *f, const char *nome);
const char* funcionario_get_nome(const Funcionario *f);

// Exibição e persistência
void funcionario_exibir_dados(const Funcionario *f);
void funcionario_formatar_para_arquivo(const Funcionario *f, char *buffer);
```

### 🏢 Sistema
```c
// Inicialização
void sistema_rh_init(SistemaRH *sistema);
void sistema_rh_inicializar_setores(SistemaRH *sistema);

// Busca
Funcionario* sistema_rh_buscar_funcionario_por_id(SistemaRH *sistema, int id);
```

### ⏰ Ponto Eletrônico
```c
// Registro de ponto
void ponto_registrar_entrada(SistemaRH *sistema, int id_funcionario, const char *observacao);
void ponto_registrar_saida(SistemaRH *sistema, int id_funcionario, const char *observacao);
void ponto_registrar_manual(SistemaRH *sistema, int id_funcionario, const char *data, 
                           const char *hora, const char *tipo, const char *observacao);

// Consultas
void ponto_listar_por_funcionario(const SistemaRH *sistema, int id_funcionario);
void ponto_relatorio_mensal(const SistemaRH *sistema, int id_funcionario, int mes, int ano);
```

### ✅ Validações
```c
// Validadores
int validacao_nome_valido(const char *nome);
int validacao_cpf_valido(const char *cpf);
int validacao_id_valido(int id, const SistemaRH *sistema, int id_atual);

// Entrada interativa
void validacao_entrada_string(const char *mensagem, char *buffer, 
                             int (*validador)(const char*), int obrigatorio);
```

## 🎯 Menu Principal

```
========== MENU ==========
1 - Cadastrar novo funcionario
2 - Listar funcionarios  
3 - Buscar funcionario por ID
4 - Editar funcionario
5 - Excluir funcionario
6 - Bater ponto
7 - Registrar ponto manual
8 - Consultar pontos
9 - Salvar dados
0 - Sair
==========================
```

### ⏰ Submenu de Pontos

```
Consultar pontos:
1 - Por funcionario
2 - Por data  
3 - Relatorio mensal
4 - Todos os registros
0 - Voltar
```

## 📄 Formato dos Arquivos

### 📊 funcionarios.txt
```
Nome;CPF;ID;Setor;Cargo;Foto
```

### ⏰ pontos.txt
```
ID_FUNCIONARIO;DATA;HORA;TIPO;OBSERVACAO
```

Exemplo:
```
12345;30/11/2025;08:15:00;ENTRADA;Chegada normal
12345;30/11/2025;12:00:00;SAIDA;Almoço
12345;30/11/2025;13:00:00;ENTRADA;Volta do almoço
```

## 📈 Melhorias Futuras

- [ ] Interface gráfica (GTK/Qt)
- [ ] Base de dados SQL
- [ ] Relatórios em PDF
- [ ] Sistema de permissões
- [ ] Backup automático
- [ ] Testes automatizados
- [ ] Logs do sistema

## 🤝 Contribuição

Contribuições são bem-vindas! Para contribuir:

1. **Fork** o projeto
2. **Crie** uma branch para sua feature
3. **Commit** suas mudanças
4. **Push** para a branch
5. **Abra** um Pull Request


## 👥 Autores

- **Equipe de Desenvolvimento** - Sistema de RH Modular
Karollyne 
Jesse 
Diego
Guilheme 

---

⭐ **Se este projeto foi útil, considere dar uma estrela!**
