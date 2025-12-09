# ⚡ tchERP - Sistema Gaúcho de Gestão de RH

Sistema completo de gerenciamento de recursos humanos desenvolvido em **C++17** com interface web moderna, banco de dados MySQL e API REST.

![tchERP Banner](https://img.shields.io/badge/tchERP-Sistema_Ga%C3%BAcho-green?style=for-the-badge)
![C++](https://img.shields.io/badge/C++-17-blue?style=for-the-badge&logo=c%2B%2B)
![MySQL](https://img.shields.io/badge/MySQL-9.5-orange?style=for-the-badge&logo=mysql)

---

## 🌟 Características Principais

### 🎨 Interface Web Moderna
- **Design Responsivo** com tema das cores do Rio Grande do Sul (verde e amarelo)
- **Single Page Application** com navegação intuitiva
- **Upload de Fotos** com preview em tempo real (base64)
- **Avatares Dinâmicos** com iniciais quando não há foto

### 👥 Gestão de Funcionários
- ✅ **Cadastro** completo com validação de CPF
- ✅ **Edição** de dados com interface dedicada
- ✅ **Listagem** com fotos e informações organizadas
- ✅ **Exclusão** com confirmação
- ✅ **Integração MySQL** com fallback para arquivos

### ⏰ Sistema de Ponto Eletrônico
- 📍 **Registro de Entrada/Saída** com timestamp automático
- 📊 **Relatórios Detalhados** com cálculo de horas trabalhadas
- 📅 **Filtros por Período** e funcionário
- 📈 **Estatísticas** (total de registros, entradas, saídas)
- 💾 **Persistência MySQL** com fallback para arquivos

### 🏢 Organização Hierárquica
- **Setores** cadastrados dinamicamente
- **Cargos** vinculados a setores específicos
- **Estrutura Relacional** com chaves estrangeiras no MySQL

---

## 🚀 Tecnologias Utilizadas

### Backend
- **C++17** - Linguagem principal
- **POO** - Programação Orientada a Objetos
- **Servidor HTTP Customizado** com sockets POSIX
- **API REST** com JSON
- **MySQL Connector/C** - Integração com banco de dados
- **Fallback System** - Arquivos texto quando MySQL indisponível

### Frontend
- **HTML5** com semântica moderna
- **CSS3** com gradientes e animações
- **JavaScript Vanilla** com Fetch API
- **Design Responsivo** sem frameworks

### Banco de Dados
- **MySQL 9.5** com utf8mb4
- **Estrutura Relacional** normalizada
- **Foreign Keys** para integridade referencial
- **Timestamps Automáticos** para auditoria

---

## 📁 Estrutura do Projeto

```
modulo-rh-erp/
├── 📁 include/              # Headers C++ (.h)
│   ├── funcionario.h        # Classe Funcionario
│   ├── sistema_rh.h         # Classe SistemaRH (gerenciador principal)
│   ├── servidor_http.h      # Servidor HTTP e API REST
│   ├── database.h           # Camada de acesso ao MySQL
│   ├── config.h             # Parser de configuração INI
│   ├── registro_ponto.h     # Classe RegistroPonto
│   ├── setor.h              # Classe Setor e Cargo
│   ├── persistencia.h       # I/O de arquivos (fallback)
│   └── validacao.h          # Validações (CPF, etc)
│
├── 📁 src/                  # Implementações C++ (.cpp)
│   ├── funcionario.cpp
│   ├── sistema_rh.cpp
│   ├── servidor_http.cpp    # Rotas API e handlers HTTP
│   ├── database.cpp         # Queries MySQL e conexão
│   ├── config.cpp
│   ├── registro_ponto.cpp
│   ├── setor.cpp
│   ├── persistencia.cpp
│   └── validacao.cpp
│
├── 📁 frontend/             # Interface Web
│   ├── index.html           # Página inicial
│   ├── cadastro.html        # Formulário de cadastro
│   ├── edicao.html          # Edição de funcionários
│   ├── listagem.html        # Tabela de funcionários
│   ├── pontos.html          # Registro de pontos
│   ├── relatorios.html      # Análises e estatísticas
│   └── estilo.css           # Estilos (tema RS)
│
├── 📁 database/             # Scripts SQL
│   └── setup.sql            # Criação de tabelas
│
├── 📁 fotos/                # Imagens de funcionários
│   └── diego.png
│
├── main.cpp                 # CLI (modo terminal)
├── main_web.cpp             # Servidor Web (modo produção)
├── config.ini               # Configurações (MySQL, porta, etc)
├── config.ini.example       # Template de configuração
└── README.md                # Esta documentação
```

---

## ⚙️ Instalação e Configuração

### Pré-requisitos

- **macOS / Linux**
- **C++ Compiler** com suporte a C++17 (g++, clang++)
- **MySQL 8.0+** ou **MySQL 9.x**
- **Homebrew** (macOS) ou gerenciador de pacotes equivalente

### 1. Instalar Dependências

**macOS:**
```bash
# Instalar MySQL
brew install mysql

# Iniciar serviço MySQL
brew services start mysql

# Instalar MySQL Client (se necessário para compatibilidade)
brew install mysql-client@8.0
```

**Linux (Debian/Ubuntu):**
```bash
sudo apt update
sudo apt install g++ mysql-server libmysqlclient-dev
sudo systemctl start mysql
```

### 2. Configurar Banco de Dados

```bash
# Criar banco de dados
mysql -u root < database/setup.sql

# Ou executar manualmente:
mysql -u root
CREATE DATABASE tcherp CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE tcherp;
```

### 3. Configurar Aplicação

```bash
# Copiar template de configuração
cp config.ini.example config.ini

# Editar configurações (se necessário)
nano config.ini
```

**Exemplo de `config.ini`:**
```ini
[database]
host=localhost
port=3306
user=root
password=
database=tcherp

[application]
server_port=8080
enable_file_backup=true
backup_directory=./backups/
```

### 4. Compilar e Executar

```bash
# Compilar servidor web
g++ -std=c++17 -Wall -Wextra \
  -I./include \
  -I/opt/homebrew/Cellar/mysql/9.5.0_2/include/mysql \
  main_web.cpp src/*.cpp -o servidor_web \
  -L/opt/homebrew/Cellar/mysql/9.5.0_2/lib \
  -lmysqlclient

# Executar servidor
./servidor_web
```

**Ajuste o caminho do MySQL conforme sua instalação:**
- macOS Homebrew: `/opt/homebrew/Cellar/mysql/[VERSION]/`
- Linux: `/usr/include/mysql/` e `-lmysqlclient`

---

## 🌐 Usando a Aplicação

### Acessar Interface Web

Após iniciar o servidor, abra seu navegador em:

```
http://localhost:8080/
```

### Páginas Disponíveis

| Página | URL | Descrição |
|--------|-----|-----------|
| 🏠 **Início** | `/` | Dashboard com links principais |
| ➕ **Cadastro** | `/frontend/cadastro.html` | Formulário de novo funcionário |
| ✏️ **Edição** | `/frontend/edicao.html?id=X` | Editar dados de funcionário |
| 📋 **Listagem** | `/frontend/listagem.html` | Tabela com todos os funcionários |
| ⏰ **Pontos** | `/frontend/pontos.html` | Registrar entrada/saída |
| 📊 **Relatórios** | `/frontend/relatorios.html` | Análises e estatísticas |

---

## 🔌 API REST

### Endpoints Disponíveis

#### **Funcionários**

```http
GET    /api/funcionarios          # Listar todos
GET    /api/funcionarios/:id      # Buscar por ID
POST   /api/funcionarios          # Cadastrar novo
PUT    /api/funcionarios/:id      # Atualizar dados
DELETE /api/funcionarios/:id      # Excluir
```

**Exemplo POST:**
```json
{
  "id": 1,
  "nome": "João Silva",
  "cpf": "12345678901",
  "setor": "TI",
  "cargo": "Desenvolvedor",
  "foto": "data:image/png;base64,..."
}
```

#### **Pontos**

```http
GET    /api/pontos                # Listar todos
POST   /api/pontos                # Registrar ponto
```

**Exemplo POST:**
```json
{
  "idFuncionario": 1,
  "data": "09/12/2025",
  "hora": "08:30:00",
  "tipo": "ENTRADA",
  "observacao": "Início do expediente"
}
```

#### **Setores**

```http
GET    /api/setores               # Listar setores e cargos
```

**Resposta:**
```json
[
  {
    "nome": "TI",
    "cargos": ["Desenvolvedor", "Analista", "Gerente"]
  }
]
```

---

## 🗄️ Estrutura do Banco de Dados

### Tabelas

#### **setores**
```sql
id INT AUTO_INCREMENT PRIMARY KEY
nome VARCHAR(100) UNIQUE NOT NULL
descricao TEXT
```

#### **cargos**
```sql
id INT AUTO_INCREMENT PRIMARY KEY
id_setor INT NOT NULL (FK -> setores.id)
nome VARCHAR(100) NOT NULL
salario_base DECIMAL(10,2)
```

#### **rh** (funcionários)
```sql
id INT AUTO_INCREMENT PRIMARY KEY
nome VARCHAR(100) NOT NULL
cpf VARCHAR(11) UNIQUE NOT NULL
id_setor INT NOT NULL (FK -> setores.id)
id_cargo INT NOT NULL (FK -> cargos.id)
foto VARCHAR(255)
created_at TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP
updated_at TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
```

#### **pontos**
```sql
id INT AUTO_INCREMENT PRIMARY KEY
id_funcionario INT NOT NULL (FK -> rh.id)
data VARCHAR(10) NOT NULL
hora VARCHAR(8) NOT NULL
tipo ENUM('ENTRADA', 'SAIDA') NOT NULL
observacao VARCHAR(255)
created_at TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP
```

---

## 🐛 Troubleshooting

### Erro: "Can't connect to MySQL server"

**Solução:**
```bash
# Verificar se MySQL está rodando
brew services list | grep mysql

# Iniciar MySQL
brew services start mysql

# Verificar conexão
mysql -u root -e "SELECT 1;"
```

### Erro: "Authentication plugin 'mysql_native_password' cannot be loaded"

**Solução:**
```bash
# Criar link simbólico para o plugin
cd /opt/homebrew/Cellar/mysql/9.5.0_2/lib/plugin/
ln -s auth.so mysql_native_password.so

# Ou instalar MySQL Client 8.0
brew install mysql-client@8.0
brew link mysql-client@8.0 --force
```

### Erro: "Port 8080 already in use"

**Solução:**
```bash
# Alterar porta em config.ini
[application]
server_port=8081

# Ou matar processo na porta 8080
lsof -ti:8080 | xargs kill -9
```

---

## 👥 Equipe de Desenvolvimento

- **Jesse Oliveira**
- **Diego Cardenal**
- **Karollyne Costa**
- **Guilherme B.**

**Instituição:** UERGS (Universidade Estadual do Rio Grande do Sul)  
**Disciplina:** Programação Orientada a Objetos (POO)  
**Professor:** André Borin  
**Período:** 2025/2

---

## 📝 Licença

Este projeto foi desenvolvido para fins acadêmicos.

---

## 🧉 Bah, tchê!

Sistema desenvolvido com orgulho gaúcho! 🌾⚡

---

**© 2025 tchERP - Sistema Gaúcho de Gestão de RH**
