# 🏗️ Diagrama de Arquitetura - Sistema de RH

## 📋 Visão Geral da Arquitetura

```mermaid
graph TB
    subgraph "🖥️ INTERFACE DO USUÁRIO"
        A[main.c] --> B[interface.h/c]
        B --> C[Menu Principal]
        B --> D[Submenu Listagens]
        B --> E[Submenu Pontos]
        B --> F[Seleção Interativa]
    end

    subgraph "🔧 CAMADA DE OPERAÇÕES"
        G[operacoes.h/c] --> H[Cadastrar]
        G --> I[Buscar]
        G --> J[Editar]
        G --> K[Excluir]
        G --> L[Listar]
        G --> M[Bater Ponto]
        G --> N[Consultar Pontos]
    end

    subgraph "🏢 CAMADA DE NEGÓCIO"
        O[sistema_rh.h/c] --> P[Gerenciar Sistema]
        O --> Q[Inicializar Setores]
        O --> R[Buscar por ID]
        
        S[funcionario.h/c] --> T[CRUD Funcionário]
        U[setor.h/c] --> V[Gestão Setores/Cargos]
        W[ponto.h/c] --> X[Sistema de Ponto]
        W --> Y[Cálculo de Horas]
        W --> Z[Relatórios]
    end

    subgraph "✅ CAMADA DE VALIDAÇÃO"
        AA[validacao.h/c] --> BB[Validar Nome]
        AA --> CC[Validar CPF]
        AA --> DD[Validar ID]
        AA --> EE[Validar Data/Hora]
        AA --> FF[Entrada Segura]
    end

    subgraph "💾 CAMADA DE PERSISTÊNCIA"
        GG[persistencia.h/c] --> HH[Importar Funcionários]
        GG --> II[Exportar Funcionários]
        GG --> JJ[Importar Pontos]
        GG --> KK[Exportar Pontos]
    end

    subgraph "📊 CAMADA DE DADOS"
        LL[types.h] --> MM[Estruturas]
        MM --> NN[Funcionario]
        MM --> OO[Setor]
        MM --> PP[Cargo]
        MM --> QQ[RegistroPonto]
        MM --> RR[SistemaRH]
    end

    %% Fluxo de dados
    A --> G
    G --> O
    G --> S
    G --> U
    G --> W
    G --> AA
    G --> GG
    O --> LL
    S --> LL
    U --> LL
    W --> LL
    AA --> LL
    GG --> LL

    %% Estilos
    classDef interface fill:#e1f5fe,stroke:#01579b,stroke-width:2px
    classDef operations fill:#f3e5f5,stroke:#4a148c,stroke-width:2px
    classDef business fill:#e8f5e8,stroke:#1b5e20,stroke-width:2px
    classDef validation fill:#fff3e0,stroke:#e65100,stroke-width:2px
    classDef persistence fill:#fce4ec,stroke:#880e4f,stroke-width:2px
    classDef data fill:#f1f8e9,stroke:#33691e,stroke-width:2px

    class A,B,C,D,E,F interface
    class G,H,I,J,K,L,M,N operations
    class O,P,Q,R,S,T,U,V,W,X,Y,Z business
    class AA,BB,CC,DD,EE,FF validation
    class GG,HH,II,JJ,KK persistence
    class LL,MM,NN,OO,PP,QQ,RR data
```

## 🔄 Fluxo de Dados - Sistema de Ponto

```mermaid
sequenceDiagram
    participant U as 👤 Usuário
    participant I as 🖥️ Interface
    participant O as 🔧 Operações
    participant P as ⏰ Ponto
    participant V as ✅ Validação
    participant S as 🏢 Sistema
    participant D as 📊 Dados

    Note over U,D: Fluxo de Bater Ponto
    U->>I: Seleciona "6 - Bater ponto"
    I->>O: operacoes_bater_ponto()
    O->>V: validacao_entrada_id()
    V-->>O: ID válido
    O->>P: ponto_registrar_entrada/saida()
    P->>S: sistema_rh_buscar_funcionario()
    S->>D: Busca funcionário
    D-->>S: Funcionário encontrado
    P->>P: ponto_obter_data_atual()
    P->>P: ponto_obter_hora_atual()
    P->>D: Adiciona registro de ponto
    P-->>O: Ponto registrado com sucesso
    O->>I: Exibe confirmação
    I->>U: "Ponto registrado!"

    Note over U,D: Fluxo de Calcular Horas
    U->>I: Seleciona "8 - Consultar pontos"
    I->>I: Submenu de pontos
    U->>I: Seleciona "4 - Calcular horas"
    I->>O: operacoes_consultar_pontos()
    O->>P: ponto_calcular_horas_trabalhadas()
    P->>D: Busca registros do período
    P->>P: ponto_calcular_horas_entre_horarios()
    P->>P: ponto_converter_hora_para_decimal()
    P-->>O: Relatório de horas
    O->>I: Exibe relatório
    I->>U: Total de horas trabalhadas
```

## 🗂️ Estrutura de Módulos Atualizada

```mermaid
classDiagram
    class SistemaRH {
        +Funcionario funcionarios[]
        +int num_funcionarios
        +Setor setores[]
        +int num_setores
        +RegistroPonto pontos[]
        +int num_pontos
        +init()
        +inicializar_setores()
        +buscar_funcionario_por_id()
    }

    class Funcionario {
        +char nome[]
        +char cpf[]
        +int id
        +char setor[]
        +char cargo[]
        +char foto[]
        +init()
        +set_nome()
        +get_nome()
        +exibir_dados()
        +formatar_para_arquivo()
    }

    class RegistroPonto {
        +int id_funcionario
        +char data[]
        +char hora[]
        +char tipo[]
        +char observacao[]
        +init()
    }

    class Setor {
        +char nome[]
        +Cargo cargos_permitidos[]
        +int num_cargos
        +init()
        +adicionar_cargo()
        +get_nome()
        +get_cargos()
    }

    class Cargo {
        +char nome[]
        +init()
        +get_nome()
    }

    class OperacoesPonto {
        +bater_ponto()
        +registrar_manual()
        +consultar_pontos()
        +calcular_horas_trabalhadas()
        +listar_horas_diarias()
    }

    class SistemaPonto {
        +registrar_entrada()
        +registrar_saida()
        +registrar_manual()
        +listar_por_funcionario()
        +relatorio_mensal()
        +calcular_horas_trabalhadas()
        +converter_hora_para_decimal()
        +calcular_horas_entre_horarios()
    }

    class Operacoes {
        +cadastrar_funcionario()
        +buscar_funcionario()
        +editar_funcionario()
        +excluir_funcionario()
        +listar_por_nome()
        +listar_por_cargo()
    }

    class Validacao {
        +nome_valido()
        +cpf_valido()
        +id_valido()
        +validar_data()
        +validar_hora()
        +entrada_string()
        +entrada_id()
    }

    class Persistencia {
        +importar_de_arquivo()
        +exportar_para_arquivo()
        +importar_pontos()
        +exportar_pontos()
    }

    class Interface {
        +exibir_menu()
        +exibir_submenu_listagens()
        +exibir_submenu_pontos()
        +selecionar_setor_interativo()
        +selecionar_cargo_interativo()
    }

    SistemaRH "1" *-- "0..*" Funcionario : contém
    SistemaRH "1" *-- "1..*" Setor : gerencia
    SistemaRH "1" *-- "0..*" RegistroPonto : armazena
    Setor "1" *-- "1..*" Cargo : possui
    RegistroPonto "1" --> "1" Funcionario : referencia
    Operacoes ..> SistemaRH : usa
    OperacoesPonto ..> SistemaRH : usa
    OperacoesPonto ..> SistemaPonto : usa
    SistemaPonto ..> RegistroPonto : manipula
    Operacoes ..> Validacao : usa
    OperacoesPonto ..> Validacao : usa
    Operacoes ..> Persistencia : usa
    Operacoes ..> Interface : usa
    Interface ..> SistemaRH : consulta
```

## 📋 Dependências entre Módulos Atualizada

```mermaid
graph LR
    subgraph "🎯 CORE"
        types[types.h]
    end

    subgraph "🏗️ FOUNDATION"
        func[funcionario.h/c]
        setor[setor.h/c]
        valid[validacao.h/c]
        persist[persistencia.h/c]
        interface[interface.h/c]
        ponto[ponto.h/c]
    end

    subgraph "📊 BUSINESS"
        sistema[sistema_rh.h/c]
        ops[operacoes.h/c]
    end

    subgraph "🚀 APPLICATION"
        main[main.c]
    end

    %% Dependências
    func --> types
    setor --> types
    valid --> types
    persist --> types
    interface --> types
    ponto --> types
    
    sistema --> types
    sistema --> setor
    
    ops --> types
    ops --> func
    ops --> setor
    ops --> valid
    ops --> persist
    ops --> interface
    ops --> sistema
    ops --> ponto
    
    ponto --> sistema
    ponto --> func
    
    main --> sistema
    main --> ops
    main --> interface
    main --> persist
    main --> valid

    %% Estilos
    classDef core fill:#ffeb3b,stroke:#f57f17,stroke-width:3px
    classDef foundation fill:#81c784,stroke:#388e3c,stroke-width:2px
    classDef business fill:#64b5f6,stroke:#1976d2,stroke-width:2px
    classDef application fill:#f48fb1,stroke:#c2185b,stroke-width:2px

    class types core
    class func,setor,valid,persist,interface,ponto foundation
    class sistema,ops business
    class main application
```

## 🚀 Funcionalidades do Sistema de Ponto

```mermaid
mindmap
  root((Sistema de Ponto))
    Registro
      Bater Ponto Automático
        Data/Hora Atual
        Entrada/Saída
        Observações
      Registro Manual
        Data Personalizada
        Hora Personalizada
        Correções
    Consultas
      Por Funcionário
        Histórico Completo
        Filtros por Período
      Por Data
        Todos os Funcionários
        Dia Específico
      Relatório Mensal
        Resumo Executivo
        Dados Consolidados
    Cálculos
      Horas Trabalhadas
        Total Mensal
        Média Diária
        Dias Trabalhados
      Detalhamento Diário
        Registros por Dia
        Horas por Dia
        Registros Incompletos
    Validações
      Formato Data/Hora
        DD/MM/AAAA
        HH:MM:SS
      Consistência
        Entrada antes Saída
        Funcionário Existe
        Sequência Lógica
```

## 📊 Métricas e Relatórios

### 🕒 Cálculo de Horas
- **Conversão**: HH:MM:SS → Decimal (8.75h)
- **Método**: Primeira entrada × última saída do dia
- **Validação**: Ignora registros inválidos
- **Precisão**: Até segundos

### 📈 Tipos de Relatório
1. **Horas Trabalhadas**: Total mensal com média
2. **Detalhamento Diário**: Todos os pontos do dia
3. **Registros Incompletos**: Identificação automática
4. **Histórico Completo**: Por funcionário/período

---

## 📝 Notas Arquiteturais

### 🎯 Novos Princípios Aplicados (Sistema de Ponto)

1. **Separação de Responsabilidades**: Módulo ponto.h/c independente
2. **Cálculos Centralizados**: Funções específicas para conversões
3. **Relatórios Flexíveis**: Múltiplas formas de visualização
4. **Validação Robusta**: Data/hora com formatos específicos
5. **Persistência Dupla**: Funcionários e pontos em arquivos separados

### 🛡️ Camadas de Validação (Ponto)

- **Formato**: Validação de data DD/MM/AAAA e hora HH:MM:SS
- **Lógica**: Saída não pode ser antes da entrada
- **Existência**: Funcionário deve existir no sistema
- **Consistência**: Avisos para sequências incomuns

### 🔄 Padrões de Design (Sistema de Ponto)

- **Calculator Pattern**: Funções específicas para cálculos
- **Report Builder**: Diferentes tipos de relatório
- **Validation Chain**: Múltiplas validações em sequência
- **Data Transformation**: Conversão de formatos de hora