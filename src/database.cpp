#include "../include/database.h"
#include <iostream>
#include <sstream>
#include <cstring>

using namespace std;

Database::Database() : connection(nullptr), port(3306) {
    connection = mysql_init(nullptr);
    if (!connection) {
        cerr << "Erro ao inicializar MySQL" << endl;
    }
}

Database::~Database() {
    desconectar();
}

bool Database::conectar(const string& host, const string& user, 
                       const string& password, const string& database, int port) {
    this->host = host;
    this->user = user;
    this->password = password;
    this->database = database;
    this->port = port;
    
    // Configurar opções antes da conexão
    // Desabilitar carregamento de plugins externos completamente
    mysql_options(connection, MYSQL_ENABLE_CLEARTEXT_PLUGIN, (char *)"1");
    
    // Usar apenas autenticação built-in
    mysql_options(connection, MYSQL_DEFAULT_AUTH, "mysql_native_password");
    
    // Definir diretório de plugins do Homebrew MySQL
    const char* plugin_dir = "/opt/homebrew/Cellar/mysql/9.5.0_2/lib/plugin";
    mysql_options(connection, MYSQL_PLUGIN_DIR, plugin_dir);
    
    if (!mysql_real_connect(connection, host.c_str(), user.c_str(), 
                           password.c_str(), database.c_str(), port, nullptr, 0)) {
        cerr << "❌ Erro ao conectar ao MySQL: " << mysql_error(connection) << endl;
        return false;
    }
    
    // Configurar UTF-8
    mysql_set_character_set(connection, "utf8mb4");
    
    cout << "✅ Conectado ao MySQL com sucesso!" << endl;
    return true;
}

void Database::desconectar() {
    if (connection) {
        mysql_close(connection);
        connection = nullptr;
    }
}

bool Database::isConectado() const {
    return connection != nullptr && mysql_ping(connection) == 0;
}

bool Database::executarQuery(const string& query) {
    if (!isConectado()) {
        cerr << "❌ Não conectado ao banco de dados" << endl;
        return false;
    }
    
    if (mysql_query(connection, query.c_str())) {
        cerr << "❌ Erro na query: " << mysql_error(connection) << endl;
        cerr << "Query: " << query << endl;
        return false;
    }
    
    return true;
}

MYSQL_RES* Database::executarSelect(const string& query) {
    if (!executarQuery(query)) {
        return nullptr;
    }
    return mysql_store_result(connection);
}

string Database::escaparString(const string& str) {
    if (!connection) return str;
    
    char* buffer = new char[str.length() * 2 + 1];
    mysql_real_escape_string(connection, buffer, str.c_str(), str.length());
    string result(buffer);
    delete[] buffer;
    return result;
}

string Database::getUltimoErro() {
    if (connection) {
        return string(mysql_error(connection));
    }
    return "Sem conexão";
}

bool Database::criarTabelas() {
    cout << "📋 Criando tabelas no banco de dados..." << endl;
    
    // 1. Tabela de setores (sem dependências)
    string createSetores = R"(
        CREATE TABLE IF NOT EXISTS setores (
            id INT AUTO_INCREMENT PRIMARY KEY,
            nome VARCHAR(100) UNIQUE NOT NULL,
            descricao TEXT,
            INDEX idx_nome (nome)
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
    )";
    
    if (!executarQuery(createSetores)) {
        return false;
    }
    cout << "  ✓ Tabela 'setores' criada" << endl;
    
    // 2. Tabela de cargos (depende de setores)
    string createCargos = R"(
        CREATE TABLE IF NOT EXISTS cargos (
            id INT AUTO_INCREMENT PRIMARY KEY,
            id_setor INT NOT NULL,
            nome VARCHAR(100) NOT NULL,
            salario_base DECIMAL(10,2),
            FOREIGN KEY (id_setor) REFERENCES setores(id) ON DELETE CASCADE,
            UNIQUE KEY unique_cargo_setor (id_setor, nome),
            INDEX idx_nome (nome)
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
    )";
    
    if (!executarQuery(createCargos)) {
        return false;
    }
    cout << "  ✓ Tabela 'cargos' criada" << endl;
    
    // 3. Tabela de funcionários (depende de setores e cargos)
    string createFuncionarios = R"(
        CREATE TABLE IF NOT EXISTS rh (
            id INT AUTO_INCREMENT PRIMARY KEY,
            nome VARCHAR(100) NOT NULL,
            cpf VARCHAR(11) UNIQUE NOT NULL,
            id_setor INT NOT NULL,
            id_cargo INT NOT NULL,
            foto VARCHAR(255),
            created_at TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
            FOREIGN KEY (id_setor) REFERENCES setores(id) ON DELETE RESTRICT,
            FOREIGN KEY (id_cargo) REFERENCES cargos(id) ON DELETE RESTRICT,
            INDEX idx_nome (nome),
            INDEX idx_cpf (cpf),
            INDEX idx_setor (id_setor),
            INDEX idx_cargo (id_cargo)
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
    )";
    
    if (!executarQuery(createFuncionarios)) {
        return false;
    }
    cout << "  ✓ Tabela 'rh' criada" << endl;
    
    // Tabela de pontos
    string createPontos = R"(
        CREATE TABLE IF NOT EXISTS pontos (
            id INT AUTO_INCREMENT PRIMARY KEY,
            id_funcionario INT NOT NULL,
            data VARCHAR(10) NOT NULL,
            hora VARCHAR(8) NOT NULL,
            tipo ENUM('ENTRADA', 'SAIDA') NOT NULL,
            observacao VARCHAR(255),
            created_at TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (id_funcionario) REFERENCES rh(id) ON DELETE CASCADE,
            INDEX idx_funcionario (id_funcionario),
            INDEX idx_data (data)
        ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
    )";
    
    if (!executarQuery(createPontos)) {
        return false;
    }
    cout << "  ✓ Tabela 'pontos' criada" << endl;
    
    cout << "✅ Todas as tabelas foram criadas com sucesso!" << endl;
    return true;
}

// Funcionários
bool Database::inserirFuncionario(const Funcionario& func) {
    // Buscar IDs do setor e cargo
    int idSetor = buscarIdSetorPorNome(func.getSetor());
    int idCargo = buscarIdCargoPorNome(func.getCargo(), idSetor);
    
    if (idSetor == -1 || idCargo == -1) {
        cout << "Erro: Setor ou cargo não encontrado no banco!" << endl;
        cout << "Setor: " << func.getSetor() << " (ID: " << idSetor << ")" << endl;
        cout << "Cargo: " << func.getCargo() << " (ID: " << idCargo << ")" << endl;
        return false;
    }
    
    ostringstream query;
    query << "INSERT INTO rh (nome, cpf, id_setor, id_cargo, foto) VALUES ("
          << "'" << escaparString(func.getNome()) << "', "
          << "'" << escaparString(func.getCPF()) << "', "
          << idSetor << ", "
          << idCargo << ", "
          << "'" << escaparString(func.getFoto()) << "')";
    
    return executarQuery(query.str());
}

bool Database::atualizarFuncionario(const Funcionario& func) {
    // Buscar IDs do setor e cargo
    int idSetor = buscarIdSetorPorNome(func.getSetor());
    int idCargo = buscarIdCargoPorNome(func.getCargo(), idSetor);
    
    if (idSetor == -1 || idCargo == -1) {
        return false;
    }
    
    ostringstream query;
    query << "UPDATE rh SET "
          << "nome = '" << escaparString(func.getNome()) << "', "
          << "cpf = '" << escaparString(func.getCPF()) << "', "
          << "id_setor = " << idSetor << ", "
          << "id_cargo = " << idCargo << ", "
          << "foto = '" << escaparString(func.getFoto()) << "' "
          << "WHERE id = " << func.getID();
    
    return executarQuery(query.str());
}

bool Database::deletarFuncionario(int id) {
    ostringstream query;
    query << "DELETE FROM rh WHERE id = " << id;
    return executarQuery(query.str());
}

Funcionario Database::buscarFuncionarioPorId(int id) {
    ostringstream query;
    query << "SELECT rh.id, rh.nome, rh.cpf, setores.nome, cargos.nome, rh.foto "
          << "FROM rh "
          << "INNER JOIN setores ON rh.id_setor = setores.id "
          << "INNER JOIN cargos ON rh.id_cargo = cargos.id "
          << "WHERE rh.id = " << id;
    
    MYSQL_RES* result = executarSelect(query.str());
    if (!result) {
        throw runtime_error("Funcionário não encontrado");
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        mysql_free_result(result);
        throw runtime_error("Funcionário não encontrado");
    }
    
    Funcionario func(
        atoi(row[0]),
        row[1] ? row[1] : "",
        row[2] ? row[2] : "",
        row[3] ? row[3] : "",
        row[4] ? row[4] : "",
        row[5] ? row[5] : ""
    );
    
    mysql_free_result(result);
    return func;
}

vector<Funcionario> Database::listarFuncionarios() {
    vector<Funcionario> funcionarios;
    
    string query = "SELECT rh.id, rh.nome, rh.cpf, setores.nome, cargos.nome, rh.foto "
                   "FROM rh "
                   "INNER JOIN setores ON rh.id_setor = setores.id "
                   "INNER JOIN cargos ON rh.id_cargo = cargos.id "
                   "ORDER BY rh.nome";
    MYSQL_RES* result = executarSelect(query);
    
    if (!result) {
        return funcionarios;
    }
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        try {
            Funcionario func(
                atoi(row[0]),
                row[1] ? row[1] : "",
                row[2] ? row[2] : "",
                row[3] ? row[3] : "",
                row[4] ? row[4] : "",
                row[5] ? row[5] : ""
            );
            funcionarios.push_back(func);
        } catch (const exception& e) {
            cerr << "Erro ao carregar funcionário: " << e.what() << endl;
        }
    }
    
    mysql_free_result(result);
    return funcionarios;
}

bool Database::funcionarioExiste(int id) {
    ostringstream query;
    query << "SELECT COUNT(*) FROM rh WHERE id = " << id;
    
    MYSQL_RES* result = executarSelect(query.str());
    if (!result) {
        return false;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    bool existe = row && atoi(row[0]) > 0;
    
    mysql_free_result(result);
    return existe;
}

// Pontos
bool Database::inserirPonto(const RegistroPonto& ponto) {
    ostringstream query;
    query << "INSERT INTO pontos (id_funcionario, data, hora, tipo, observacao) VALUES ("
          << ponto.getIdFuncionario() << ", "
          << "'" << escaparString(ponto.getData()) << "', "
          << "'" << escaparString(ponto.getHora()) << "', "
          << "'" << escaparString(ponto.getTipo()) << "', "
          << "'" << escaparString(ponto.getObservacao()) << "')";
    
    return executarQuery(query.str());
}

vector<RegistroPonto> Database::listarPontos() {
    vector<RegistroPonto> pontos;
    
    string query = "SELECT id_funcionario, data, hora, tipo, observacao FROM pontos ORDER BY data DESC, hora DESC";
    MYSQL_RES* result = executarSelect(query);
    
    if (!result) {
        return pontos;
    }
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        try {
            RegistroPonto ponto(
                atoi(row[0]),
                row[1] ? row[1] : "",
                row[2] ? row[2] : "",
                row[3] ? row[3] : "",
                row[4] ? row[4] : ""
            );
            pontos.push_back(ponto);
        } catch (const exception& e) {
            cerr << "Erro ao carregar ponto: " << e.what() << endl;
        }
    }
    
    mysql_free_result(result);
    return pontos;
}

vector<RegistroPonto> Database::listarPontosPorFuncionario(int idFuncionario) {
    vector<RegistroPonto> pontos;
    
    ostringstream query;
    query << "SELECT id_funcionario, data, hora, tipo, observacao FROM pontos "
          << "WHERE id_funcionario = " << idFuncionario
          << " ORDER BY data DESC, hora DESC";
    
    MYSQL_RES* result = executarSelect(query.str());
    
    if (!result) {
        return pontos;
    }
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        try {
            RegistroPonto ponto(
                atoi(row[0]),
                row[1] ? row[1] : "",
                row[2] ? row[2] : "",
                row[3] ? row[3] : "",
                row[4] ? row[4] : ""
            );
            pontos.push_back(ponto);
        } catch (const exception& e) {
            cerr << "Erro ao carregar ponto: " << e.what() << endl;
        }
    }
    
    mysql_free_result(result);
    return pontos;
}

bool Database::inserirSetor(const Setor& setor) {
    // Primeiro insere o setor
    ostringstream querySetor;
    querySetor << "INSERT INTO setores (nome) VALUES ('" 
               << escaparString(setor.getNome()) << "') "
               << "ON DUPLICATE KEY UPDATE id=LAST_INSERT_ID(id)";
    
    if (!executarQuery(querySetor.str())) {
        return false;
    }
    
    // Pega o ID do setor
    MYSQL_RES* result = executarSelect("SELECT LAST_INSERT_ID()");
    if (!result) return false;
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int idSetor = row ? atoi(row[0]) : 0;
    mysql_free_result(result);
    
    if (idSetor == 0) return false;
    
    // Insere os cargos
    const auto& cargos = setor.getCargos();
    for (const auto& cargo : cargos) {
        ostringstream queryCargo;
        queryCargo << "INSERT IGNORE INTO cargos (id_setor, nome) VALUES ("
                   << idSetor << ", '" << escaparString(cargo.getNome()) << "')";
        
        if (!executarQuery(queryCargo.str())) {
            cerr << "Aviso: Erro ao inserir cargo " << cargo.getNome() << endl;
        }
    }
    
    return true;
}

vector<Setor> Database::listarSetores() {
    vector<Setor> setores;
    
    string query = "SELECT id, nome FROM setores ORDER BY nome";
    MYSQL_RES* result = executarSelect(query);
    
    if (!result) {
        return setores;
    }
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        int idSetor = atoi(row[0]);
        string nomeSetor = row[1] ? row[1] : "";
        
        Setor setor(nomeSetor);
        
        // Buscar cargos do setor
        ostringstream queryCargos;
        queryCargos << "SELECT nome FROM cargos WHERE id_setor = " << idSetor << " ORDER BY nome";
        
        MYSQL_RES* resultCargos = executarSelect(queryCargos.str());
        if (resultCargos) {
            MYSQL_ROW rowCargo;
            while ((rowCargo = mysql_fetch_row(resultCargos))) {
                if (rowCargo[0]) {
                    setor.adicionarCargo(Cargo(rowCargo[0]));
                }
            }
            mysql_free_result(resultCargos);
        }
        
        setores.push_back(setor);
    }
    
    mysql_free_result(result);
    return setores;
}

int Database::buscarIdSetorPorNome(const string& nomeSetor) {
    ostringstream query;
    query << "SELECT id FROM setores WHERE nome = '" << escaparString(nomeSetor) << "'";
    
    MYSQL_RES* result = executarSelect(query.str());
    if (!result) {
        return -1;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int idSetor = -1;
    if (row && row[0]) {
        idSetor = atoi(row[0]);
    }
    
    mysql_free_result(result);
    return idSetor;
}

int Database::buscarIdCargoPorNome(const string& nomeCargo, int idSetor) {
    ostringstream query;
    query << "SELECT id FROM cargos WHERE nome = '" << escaparString(nomeCargo) 
          << "' AND id_setor = " << idSetor;
    
    MYSQL_RES* result = executarSelect(query.str());
    if (!result) {
        return -1;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int idCargo = -1;
    if (row && row[0]) {
        idCargo = atoi(row[0]);
    }
    
    mysql_free_result(result);
    return idCargo;
}
