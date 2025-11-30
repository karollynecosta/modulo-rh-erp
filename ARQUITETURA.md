# 🏗️ Diagrama de Arquitetura - Sistema de RH

## 📋 Visão Geral da Arquitetura

```mermaid
graph TB
    subgraph "🖥️ INTERFACE DO USUÁRIO"
        A[main.c] --> B[interface.h/c]
        B --> C[Menu Principal]
        B --> D[Submenu Listagens]
        B --> E[Seleção Interativa]
    end

    subgraph "🔧 CAMADA DE OPERAÇÕES"
        F[operacoes.h/c] --> G[Cadastrar]
        F --> H[Buscar]
        F --> I[Editar]
        F --> J[Excluir]
        F --> K[Listar]
    end

    subgraph "🏢 CAMADA DE NEGÓCIO"
        L[sistema_rh.h/c] --> M[Gerenciar Sistema]
        L --> N[Inicializar Setores]
        L --> O[Buscar por ID]
        
        P[funcionario.h/c] --> Q[CRUD Funcionário]
        R[setor.h/c] --> S[Gestão Setores/Cargos]
    end

    subgraph "✅ CAMADA DE VALIDAÇÃO"
        T[validacao.h/c] --> U[Validar Nome]
        T --> V[Validar CPF]
        T --> W[Validar ID]
        T --> X[Entrada Segura]
    end

    subgraph "💾 CAMADA DE PERSISTÊNCIA"
        Y[persistencia.h/c] --> Z[Importar Arquivo]
        Y --> AA[Exportar Arquivo]
    end

    subgraph "📊 CAMADA DE DADOS"
        AB[types.h] --> AC[Estruturas]
        AC --> AD[Funcionario]
        AC --> AE[Setor]
        AC --> AF[Cargo]
        AC --> AG[SistemaRH]
    end

    %% Fluxo de dados
    A --> F
    F --> L
    F --> P
    F --> R
    F --> T
    F --> Y
    L --> AB
    P --> AB
    R --> AB
    T --> AB
    Y --> AB

    %% Estilos
    classDef interface fill:#e1f5fe,stroke:#01579b,stroke-width:2px
    classDef operations fill:#f3e5f5,stroke:#4a148c,stroke-width:2px
    classDef business fill:#e8f5e8,stroke:#1b5e20,stroke-width:2px
    classDef validation fill:#fff3e0,stroke:#e65100,stroke-width:2px
    classDef persistence fill:#fce4ec,stroke:#880e4f,stroke-width:2px
    classDef data fill:#f1f8e9,stroke:#33691e,stroke-width:2px

    class A,B,C,D,E interface
    class F,G,H,I,J,K operations
    class L,M,N,O,P,Q,R,S business
    class T,U,V,W,X validation
    class Y,Z,AA persistence
    class AB,AC,AD,AE,AF,AG data
```

## 🔄 Fluxo de Dados

```mermaid
sequenceDiagram
    participant U as 👤 Usuário
    participant I as 🖥️ Interface
    participant O as 🔧 Operações
    participant V as ✅ Validação
    participant S as 🏢 Sistema
    participant P as 💾 Persistência
    participant D as 📊 Dados

    U->>I: Seleciona opção do menu
    I->>O: Chama operação correspondente
    O->>V: Solicita validação de entrada
    V-->>O: Retorna dados válidos
    O->>S: Processa lógica de negócio
    S->>D: Manipula estruturas de dados
    D-->>S: Retorna dados processados
    S-->>O: Confirma operação
    O->>P: Persiste alterações (se necessário)
    P-->>O: Confirma persistência
    O->>I: Retorna resultado
    I->>U: Exibe feedback
```

## 🗂️ Estrutura de Módulos

```mermaid
classDiagram
    class SistemaRH {
        +Funcionario funcionarios[]
        +int num_funcionarios
        +Setor setores[]
        +int num_setores
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
        +entrada_string()
        +entrada_id()
    }

    class Persistencia {
        +importar_de_arquivo()
        +exportar_para_arquivo()
    }

    class Interface {
        +exibir_menu()
        +exibir_submenu_listagens()
        +selecionar_setor_interativo()
        +selecionar_cargo_interativo()
    }

    SistemaRH "1" *-- "0..*" Funcionario : contém
    SistemaRH "1" *-- "1..*" Setor : gerencia
    Setor "1" *-- "1..*" Cargo : possui
    Operacoes ..> SistemaRH : usa
    Operacoes ..> Validacao : usa
    Operacoes ..> Persistencia : usa
    Operacoes ..> Interface : usa
    Interface ..> SistemaRH : consulta
```

## 📋 Dependências entre Módulos

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
    
    sistema --> types
    sistema --> setor
    
    ops --> types
    ops --> func
    ops --> setor
    ops --> valid
    ops --> persist
    ops --> interface
    ops --> sistema
    
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
    class func,setor,valid,persist,interface foundation
    class sistema,ops business
    class main application
```

## ⚡ Fluxo de Compilação

```mermaid
flowchart TD
    A[📝 Código Fonte] --> B{🔍 Makefile}
    
    B -->|release| C[🚀 Otimização -O2]
    B -->|debug| D[🐛 Debug -g]
    
    C --> E[📁 Compilar src/*.c]
    D --> E
    
    E --> F[📦 Gerar obj/*.o]
    F --> G[🔗 main.c]
    G --> H[🏗️ Linkar objetos]
    H --> I[⚡ sistema_rh executável]
    
    I --> J{🎯 Target}
    J -->|run| K[🚀 Executar programa]
    J -->|clean| L[🧹 Limpar build/]
    
    style A fill:#e3f2fd
    style I fill:#c8e6c9
    style K fill:#fff3e0
    style L fill:#ffebee
```

---

## 📝 Notas de Arquitetura

### 🎯 Princípios Aplicados

1. **Modularidade**: Cada módulo tem responsabilidade específica
2. **Baixo Acoplamento**: Módulos interagem através de interfaces bem definidas
3. **Alta Coesão**: Funcionalidades relacionadas agrupadas
4. **Reutilização**: Código compartilhado em módulos comuns
5. **Manutenibilidade**: Estrutura clara facilita modificações

### 🛡️ Camadas de Segurança

- **Validação de Entrada**: Todos os dados são validados
- **Proteção de Memória**: Uso seguro de strings e arrays
- **Verificação de Limites**: Prevenção de overflow
- **Confirmação de Ações**: Operações críticas requerem confirmação
