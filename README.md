# 🏢 Sistema de Recursos Humanos - Módulo ERP

Um sistema completo de gerenciamento de recursos humanos desenvolvido em **C++ moderno**, estruturado com arquitetura orientada a objetos para máxima eficiência e manutenibilidade.


## 📋 Funcionalidades

### 👥 Gestão de Funcionários
- **Cadastro** completo com validação robusta
- **Busca** por ID com formatação profissional  
- **Edição** interativa com seleção visual
- **Exclusão** com confirmação inteligente
- **Listagens** avançadas com múltiplos critérios de ordenação

### ⏰ Sistema de Ponto Eletrônico Avançado
- **Bater Ponto** automático com timestamp preciso
- **Registro Manual** para correções e ajustes históricos
- **Consultas** inteligentes por funcionário, data ou período
- **Relatórios** mensais detalhados com cálculos automáticos
- **Cálculo de Horas** trabalhadas com precisão de segundos
- **Dashboard** de horas diárias com totalizadores
- **Validação** de sequência entrada/saída e controle de consistência

### 📊 Tipos de Listagem Avançada
1. **Listagem Simples** - Interface limpa com dados essenciais
2. **Por Setor e Hierarquia** - Ordenação inteligente por cargo
3. **Alfabética Melhorada** - Formatação profissional com colunas
4. **Por Hierarquia Global** - Visualização de toda estrutura organizacional
5. **Por Setor e Cargo** - Filtros específicos com contadores
6. **Interface Polimórfica** - Exibição avançada com herança

### 💾 Persistência Robusta
- **Auto-save** na inicialização e encerramento
- **Import/Export** manual com tratamento de erros
- **Funcionários**: `funcionarios.txt` com formato delimitado
- **Pontos**: `pontos.txt` com timestamp completo
- **Exception handling** para operações de I/O
- **Backup automático** em operações críticas

## 🏗️ Arquitetura C++ Orientada a Objetos

### 📁 Estrutura Moderna do Projeto

```
modulo-rh-erp/
├── 📁 include/              # Headers C++ (.h)
│   ├── types.h              # Classes principais (Funcionario, SistemaRH, etc.)
│   ├── validacao.h          # Classe Validacao (métodos estáticos)
│   ├── ponto.h              # Classe GerenciadorPonto  
│   ├── operacoes.h          # Classe OperacoesRH
│   ├── persistencia.h       # Classe Persistencia (I/O)
│   ├── interface.h          # Classe Interface (UI avançada)
│   ├── funcionario.h        # [Deprecated - migrado para types.h]
│   ├── setor.h              # [Deprecated - migrado para types.h]
│   └── sistema_rh.h         # [Deprecated - migrado para types.h]
├── 📁 src/                  # Implementações C++ (.cpp)
│   ├── types.cpp            # Classes principais (core do sistema)
│   ├── validacao.cpp        # Validações com predicados
│   ├── ponto.cpp            # Sistema de ponto com STL
│   ├── operacoes.cpp        # CRUD operations com Interface melhorada
│   ├── persistencia.cpp     # I/O com exception handling
│   └── interface.cpp        # UI avançada com formatação profissional
├── main.cpp                 # Aplicação principal com polimorfismo
├── Makefile                 # Build system otimizado para C++17
├── README.md                # Documentação atualizada
├── ARQUITETURA.md           # Documentação técnica detalhada
├── funcionarios.txt         # Base de dados (auto-gerado)
└── pontos.txt               # Registros de ponto (auto-gerado)
```

### 🔍 **Arquitetura Orientada a Objetos**

#### 🏗️ **Classes Principais:**

```cpp
// Classe base para polimorfismo (main.cpp)
class Pessoa {
    virtual void exibirDados() const = 0;
};

// Classe modelo principal (types.h)
class Funcionario : public Pessoa {
private:
    string nome, cpf, setor, cargo, foto;
    int id;
public:
    // Métodos encapsulados com getters/setters
    string getNome() const;
    void setNome(const string& nome);
    void exibirDados() const override;
};

// Sistema gerencial (types.h)  
class SistemaRH {
private:
    vector<Funcionario> funcionarios;
    vector<Setor> setores; 
    vector<RegistroPonto> pontos;
public:
    void adicionarFuncionario(const Funcionario& func);
    Funcionario* buscarFuncionarioPorId(int id);
    void inicializarSetores();
};

// Utilitários estáticos (validacao.h)
class Validacao {
public:
    static bool nomeValido(const string& nome);
    static bool cpfValido(const string& cpf);
    static string entradaString(const string& msg, bool (*validador)(const string&));
};
```

#### 🔗 **Fluxo de Dados Moderno:**

```cpp
// Interface → Operações → Sistema → Persistência
Interface::exibirMenu() 
    ↓
OperacoesRH::cadastrarFuncionario(sistema)
    ↓  
sistema.adicionarFuncionario(funcionario)
    ↓
Persistencia::exportarParaArquivo(sistema)
```

#### 🎯 **Vantagens da Arquitetura C++:**

1. **Encapsulamento**: Dados protegidos com métodos controlados
2. **Polimorfismo**: Interface flexível com herança virtual
3. **STL Integration**: Uso de vector, string, algorithms
4. **Template Support**: Validação genérica com predicados
5. **Exception Safety**: Tratamento robusto de erros
6. **RAII**: Gerenciamento automático de recursos
7. **Const Correctness**: Métodos const para segurança

#### 💡 **Design Patterns Implementados:**
- **Static Factory**: Classes utilitárias (Validacao, Interface)
- **Template Method**: Validação genérica com predicados
- **Strategy Pattern**: Múltiplas interfaces (original + avançada)
- **Observer Pattern**: Sistema de mensagens (sucesso/erro/info)

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
void ponto_calcular_horas_trabalhadas(const SistemaRH *sistema, int id_funcionario, int mes, int ano);
void ponto_listar_horas_diarias(const SistemaRH *sistema, int id_funcionario, int mes, int ano);

// Cálculos
double ponto_converter_hora_para_decimal(const char *hora);
double ponto_calcular_horas_entre_horarios(const char *entrada, const char *saida);
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
4 - Calcular horas trabalhadas
5 - Detalhamento diario
6 - Todos os registros
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
