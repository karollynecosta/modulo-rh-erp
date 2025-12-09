#include "include/sistema_rh.h"
#include "include/servidor_http.h"
#include "include/persistencia.h"
#include "include/database.h"
#include "include/config.h"
#include <iostream>

using namespace std;

int main() {
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║  ⚡ tchERP - SISTEMA GAÚCHO DE RH 🧉      ║\n";
    cout << "╚════════════════════════════════════════════╝\n\n";
    
    // Carregar configurações
    Config config;
    bool configCarregada = config.carregar("config.ini");
    
    // Inicializar banco de dados
    Database db;
    bool usarBancoDados = false;
    
    if (configCarregada) {
        string host = config.get("database", "host", "localhost");
        string user = config.get("database", "user", "root");
        string password = config.get("database", "password", "");
        string database = config.get("database", "database", "tcherp_db");
        int port = config.getInt("database", "port", 3306);
        
        cout << "🔌 Tentando conectar ao MySQL..." << endl;
        cout << "   Host: " << host << ":" << port << endl;
        cout << "   Database: " << database << endl;
        cout << "   User: " << user << endl;
        
        if (db.conectar(host, user, password, database, port)) {
            usarBancoDados = true;
            cout << "✅ Banco de dados conectado e pronto!" << endl;
        } else {
            cout << "⚠️  Usando arquivos de texto como fallback" << endl;
        }
    }
    
    // Inicializar sistema
    SistemaRH sistema;
    sistema.inicializarSetores();
    
    // Carregar dados
    if (usarBancoDados) {
        cout << "\n📊 Carregando dados do MySQL..." << endl;
        
        // Carregar funcionários do banco
        auto funcionarios = db.listarFuncionarios();
        for (const auto& func : funcionarios) {
            sistema.adicionarFuncionario(func);
        }
        
        // Carregar pontos do banco
        auto pontos = db.listarPontos();
        for (const auto& ponto : pontos) {
            sistema.adicionarPonto(ponto);
        }
        
        cout << "   ✓ Funcionários: " << funcionarios.size() << endl;
        cout << "   ✓ Pontos: " << pontos.size() << endl;
    } else {
        cout << "\n📊 Carregando dados de arquivos..." << endl;
        Persistencia::importarDeArquivo(sistema, "funcionarios.txt");
        Persistencia::importarPontos(sistema, "pontos.txt");
        cout << "   ✓ Funcionários: " << sistema.getNumFuncionarios() << endl;
        cout << "   ✓ Pontos: " << sistema.getPontos().size() << endl;
    }
    
    cout << "   ✓ Setores: " << sistema.getSetores().size() << "\n\n";
    
    // Iniciar servidor HTTP
    int porta = config.getInt("application", "server_port", 8080);
    Database* dbPtr = db.isConectado() ? &db : nullptr;
    
    cout << "🔧 Debug - Passando DB para servidor:" << endl;
    cout << "   db.isConectado(): " << (db.isConectado() ? "sim" : "não") << endl;
    cout << "   dbPtr: " << (dbPtr ? "válido" : "nullptr") << endl;
    
    ServidorHTTP servidor(porta, sistema, dbPtr);
    servidor.registrarRotas();
    
    servidor.iniciar();
    
    return 0;
}
