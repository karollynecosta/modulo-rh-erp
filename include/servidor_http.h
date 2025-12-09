#ifndef SERVIDOR_HTTP_H
#define SERVIDOR_HTTP_H

#include <string>
#include <map>
#include <functional>
#include "sistema_rh.h"
#include "database.h"

using namespace std;

// Estrutura para representar uma requisição HTTP
struct HttpRequest {
    string method;      // GET, POST, etc.
    string path;        // /api/funcionarios
    string body;        // Corpo da requisição
    map<string, string> headers;
    map<string, string> params;
};

// Estrutura para representar uma resposta HTTP
struct HttpResponse {
    int statusCode;
    string contentType;
    string body;
    
    HttpResponse() : statusCode(200), contentType("application/json") {}
};

class ServidorHTTP {
private:
    int porta;
    SistemaRH& sistema;
    Database* db;
    map<string, function<HttpResponse(const HttpRequest&)>> rotas;
    
    // Métodos auxiliares
    string lerArquivo(const string& caminho);
    string getContentType(const string& caminho);
    HttpRequest parseRequest(const string& requestStr);
    string buildResponse(const HttpResponse& response);
    
    // Handlers de rotas da API
    HttpResponse handleGetFuncionarios(const HttpRequest& req);
    HttpResponse handlePostFuncionario(const HttpRequest& req);
    HttpResponse handleGetFuncionario(const HttpRequest& req);
    HttpResponse handlePutFuncionario(const HttpRequest& req);
    HttpResponse handleDeleteFuncionario(const HttpRequest& req);
    HttpResponse handleGetPontos(const HttpRequest& req);
    HttpResponse handlePostPonto(const HttpRequest& req);
    HttpResponse handleGetSetores(const HttpRequest& req);
    
    // Utilitários JSON
    string funcionarioToJson(const Funcionario& func);
    string funcionariosToJson(const vector<Funcionario>& funcs);
    string pontoToJson(const RegistroPonto& ponto);
    string pontosToJson(const vector<RegistroPonto>& pontos);
    string setoresToJson(const vector<Setor>& setores);
    
public:
    ServidorHTTP(int porta, SistemaRH& sistema, Database* db = nullptr);
    void registrarRotas();
    void iniciar();
};

#endif
