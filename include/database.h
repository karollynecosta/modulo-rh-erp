#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <string>
#include <vector>
#include "funcionario.h"
#include "ponto.h"
#include "setor.h"

using namespace std;

class Database {
private:
    MYSQL* connection;
    string host;
    string user;
    string password;
    string database;
    int port;
    
    bool executarQuery(const string& query);
    MYSQL_RES* executarSelect(const string& query);
    
public:
    Database();
    ~Database();
    
    // Conexão
    bool conectar(const string& host, const string& user, const string& password, 
                  const string& database, int port = 3306);
    void desconectar();
    bool isConectado() const;
    
    // Inicialização do banco
    bool criarTabelas();
    
    // Funcionários
    bool inserirFuncionario(const Funcionario& func);
    bool atualizarFuncionario(const Funcionario& func);
    bool deletarFuncionario(int id);
    Funcionario buscarFuncionarioPorId(int id);
    vector<Funcionario> listarFuncionarios();
    bool funcionarioExiste(int id);
    
    // Pontos
    bool inserirPonto(const RegistroPonto& ponto);
    vector<RegistroPonto> listarPontos();
    vector<RegistroPonto> listarPontosPorFuncionario(int idFuncionario);
    
    // Setores (opcional - pode manter em memória ou salvar)
    bool inserirSetor(const Setor& setor);
    vector<Setor> listarSetores();
    
    // Buscar IDs por nome
    int buscarIdSetorPorNome(const string& nomeSetor);
    int buscarIdCargoPorNome(const string& nomeCargo, int idSetor);
    
    // Utilidades
    string escaparString(const string& str);
    string getUltimoErro();
};

#endif
