#include "../include/servidor_http.h"
#include "../include/persistencia.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

using namespace std;

ServidorHTTP::ServidorHTTP(int porta, SistemaRH& sistema, Database* db) 
    : porta(porta), sistema(sistema), db(db) {}

string ServidorHTTP::lerArquivo(const string& caminho) {
    ifstream file(caminho);
    if (!file.is_open()) {
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

string ServidorHTTP::getContentType(const string& caminho) {
    if (caminho.find(".html") != string::npos) return "text/html";
    if (caminho.find(".css") != string::npos) return "text/css";
    if (caminho.find(".js") != string::npos) return "application/javascript";
    if (caminho.find(".json") != string::npos) return "application/json";
    if (caminho.find(".png") != string::npos) return "image/png";
    if (caminho.find(".jpg") != string::npos || caminho.find(".jpeg") != string::npos) return "image/jpeg";
    return "text/plain";
}

HttpRequest ServidorHTTP::parseRequest(const string& requestStr) {
    HttpRequest req;
    istringstream stream(requestStr);
    string line;
    
    // Primeira linha: METHOD PATH HTTP/VERSION
    if (getline(stream, line)) {
        istringstream firstLine(line);
        string httpVersion;
        firstLine >> req.method >> req.path >> httpVersion;
    }
    
    // Headers
    while (getline(stream, line) && line != "\r") {
        size_t pos = line.find(": ");
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 2);
            if (!value.empty() && value.back() == '\r') {
                value.pop_back();
            }
            req.headers[key] = value;
        }
    }
    
    // Body
    string body;
    while (getline(stream, line)) {
        body += line + "\n";
    }
    req.body = body;
    
    return req;
}

string ServidorHTTP::buildResponse(const HttpResponse& response) {
    ostringstream oss;
    
    // Status line
    oss << "HTTP/1.1 " << response.statusCode << " ";
    switch (response.statusCode) {
        case 200: oss << "OK"; break;
        case 201: oss << "Created"; break;
        case 400: oss << "Bad Request"; break;
        case 404: oss << "Not Found"; break;
        case 500: oss << "Internal Server Error"; break;
        default: oss << "Unknown";
    }
    oss << "\r\n";
    
    // Headers
    oss << "Content-Type: " << response.contentType << "\r\n";
    oss << "Content-Length: " << response.body.length() << "\r\n";
    oss << "Access-Control-Allow-Origin: *\r\n";
    oss << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
    oss << "Access-Control-Allow-Headers: Content-Type\r\n";
    oss << "Connection: close\r\n";
    oss << "\r\n";
    
    // Body
    oss << response.body;
    
    return oss.str();
}

// Conversores JSON
string ServidorHTTP::funcionarioToJson(const Funcionario& func) {
    ostringstream json;
    json << "{"
         << "\"id\":" << func.getID() << ","
         << "\"nome\":\"" << func.getNome() << "\","
         << "\"cpf\":\"" << func.getCPF() << "\","
         << "\"setor\":\"" << func.getSetor() << "\","
         << "\"cargo\":\"" << func.getCargo() << "\","
         << "\"foto\":\"" << func.getFoto() << "\""
         << "}";
    return json.str();
}

string ServidorHTTP::funcionariosToJson(const vector<Funcionario>& funcs) {
    ostringstream json;
    json << "[";
    for (size_t i = 0; i < funcs.size(); i++) {
        json << funcionarioToJson(funcs[i]);
        if (i < funcs.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

string ServidorHTTP::pontoToJson(const RegistroPonto& ponto) {
    ostringstream json;
    json << "{"
         << "\"idFuncionario\":" << ponto.getIdFuncionario() << ","
         << "\"data\":\"" << ponto.getData() << "\","
         << "\"hora\":\"" << ponto.getHora() << "\","
         << "\"tipo\":\"" << ponto.getTipo() << "\","
         << "\"observacao\":\"" << ponto.getObservacao() << "\""
         << "}";
    return json.str();
}

string ServidorHTTP::pontosToJson(const vector<RegistroPonto>& pontos) {
    ostringstream json;
    json << "[";
    for (size_t i = 0; i < pontos.size(); i++) {
        json << pontoToJson(pontos[i]);
        if (i < pontos.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

string ServidorHTTP::setoresToJson(const vector<Setor>& setores) {
    ostringstream json;
    json << "[";
    for (size_t i = 0; i < setores.size(); i++) {
        json << "{\"nome\":\"" << setores[i].getNome() << "\",\"cargos\":[";
        const auto& cargos = setores[i].getCargos();
        for (size_t j = 0; j < cargos.size(); j++) {
            json << "\"" << cargos[j].getNome() << "\"";
            if (j < cargos.size() - 1) json << ",";
        }
        json << "]}";
        if (i < setores.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

// Handlers da API
HttpResponse ServidorHTTP::handleGetFuncionarios(const HttpRequest& req) {
    (void)req; // Parâmetro não utilizado
    HttpResponse response;
    response.body = funcionariosToJson(sistema.getFuncionarios());
    return response;
}

HttpResponse ServidorHTTP::handlePostFuncionario(const HttpRequest& req) {
    HttpResponse response;
    
    try {
        // Parse JSON simples do body
        string body = req.body;
        
        // Extrair campos do JSON (implementação simples)
        auto extrair = [](const string& json, const string& campo) -> string {
            size_t pos = json.find("\"" + campo + "\"");
            if (pos == string::npos) return "";
            pos = json.find(":", pos);
            if (pos == string::npos) return "";
            pos = json.find_first_not_of(" \t\n\r\"", pos + 1);
            size_t end = json.find_first_of(",}\"\n\r", pos);
            return json.substr(pos, end - pos);
        };
        
        int id = stoi(extrair(body, "id"));
        string nome = extrair(body, "nome");
        string cpf = extrair(body, "cpf");
        string setor = extrair(body, "setor");
        string cargo = extrair(body, "cargo");
        string foto = extrair(body, "foto");
        
        Funcionario novoFunc(id, nome, cpf, setor, cargo, foto);
        sistema.adicionarFuncionario(novoFunc);
        
        // Salvar no banco de dados se conectado
        if (db && db->isConectado()) {
            cout << "💾 Salvando funcionário no MySQL..." << endl;
            if (!db->inserirFuncionario(novoFunc)) {
                cout << "❌ Erro ao salvar no banco de dados!" << endl;
                response.statusCode = 500;
                response.body = "{\"error\":\"Erro ao salvar no banco de dados\"}";
                return response;
            }
            cout << "✅ Funcionário salvo no MySQL com sucesso!" << endl;
        } else {
            cout << "⚠️  Banco não conectado. Salvando em arquivo..." << endl;
            cout << "   db pointer: " << (db ? "válido" : "nullptr") << endl;
            if (db) cout << "   isConectado: " << (db->isConectado() ? "sim" : "não") << endl;
            // Fallback para arquivo
            Persistencia::exportarParaArquivo(sistema, "funcionarios.txt");
        }
        
        response.statusCode = 201;
        response.body = "{\"message\":\"Funcionário cadastrado com sucesso\",\"funcionario\":" 
                       + funcionarioToJson(novoFunc) + "}";
    } catch (const exception& e) {
        response.statusCode = 400;
        response.body = "{\"error\":\"" + string(e.what()) + "\"}";
    }
    
    return response;
}

HttpResponse ServidorHTTP::handleGetFuncionario(const HttpRequest& req) {
    HttpResponse response;
    
    try {
        // Extrair ID da URL (/api/funcionarios/123)
        size_t pos = req.path.find_last_of('/');
        int id = stoi(req.path.substr(pos + 1));
        
        const auto& funcs = sistema.getFuncionarios();
        for (const auto& func : funcs) {
            if (func.getID() == id) {
                response.body = funcionarioToJson(func);
                return response;
            }
        }
        
        response.statusCode = 404;
        response.body = "{\"error\":\"Funcionário não encontrado\"}";
    } catch (...) {
        response.statusCode = 400;
        response.body = "{\"error\":\"ID inválido\"}";
    }
    
    return response;
}

HttpResponse ServidorHTTP::handleDeleteFuncionario(const HttpRequest& req) {
    HttpResponse response;
    
    try {
        size_t pos = req.path.find_last_of('/');
        int id = stoi(req.path.substr(pos + 1));
        
        sistema.removerFuncionario(id);
        
        // Deletar do banco de dados se conectado
        if (db && db->isConectado()) {
            if (!db->deletarFuncionario(id)) {
                response.statusCode = 500;
                response.body = "{\"error\":\"Erro ao deletar do banco de dados\"}";
                return response;
            }
        } else {
            // Fallback para arquivo
            Persistencia::exportarParaArquivo(sistema, "funcionarios.txt");
        }
        
        response.body = "{\"message\":\"Funcionário removido com sucesso\"}";
    } catch (const exception& e) {
        response.statusCode = 400;
        response.body = "{\"error\":\"" + string(e.what()) + "\"}";
    }
    
    return response;
}

HttpResponse ServidorHTTP::handlePutFuncionario(const HttpRequest& req) {
    HttpResponse response;
    
    try {
        // Extrair ID da URL
        size_t pos = req.path.find_last_of('/');
        int id = stoi(req.path.substr(pos + 1));
        
        auto extrair = [](const string& json, const string& campo) -> string {
            size_t pos = json.find("\"" + campo + "\"");
            if (pos == string::npos) return "";
            pos = json.find(":", pos);
            if (pos == string::npos) return "";
            pos = json.find_first_not_of(" \t\n\r\"", pos + 1);
            size_t end = json.find_first_of(",}\"\n\r", pos);
            return json.substr(pos, end - pos);
        };
        
        string nome = extrair(req.body, "nome");
        string cpf = extrair(req.body, "cpf");
        string setor = extrair(req.body, "setor");
        string cargo = extrair(req.body, "cargo");
        string foto = extrair(req.body, "foto");
        
        // Buscar funcionário existente
        auto& funcs = sistema.getFuncionarios();
        bool encontrado = false;
        
        for (auto& func : funcs) {
            if (func.getID() == id) {
                // Atualizar dados
                func = Funcionario(id, nome, cpf, setor, cargo, foto);
                encontrado = true;
                
                // Salvar no banco de dados se conectado
                if (db && db->isConectado()) {
                    cout << "💾 Atualizando funcionário no MySQL..." << endl;
                    if (!db->atualizarFuncionario(func)) {
                        cout << "❌ Erro ao atualizar no banco de dados!" << endl;
                        response.statusCode = 500;
                        response.body = "{\"error\":\"Erro ao atualizar no banco de dados\"}";
                        return response;
                    }
                    cout << "✅ Funcionário atualizado no MySQL!" << endl;
                } else {
                    // Fallback para arquivo
                    Persistencia::exportarParaArquivo(sistema, "funcionarios.txt");
                }
                
                response.body = "{\"message\":\"Funcionário atualizado com sucesso\",\"funcionario\":" 
                               + funcionarioToJson(func) + "}";
                break;
            }
        }
        
        if (!encontrado) {
            response.statusCode = 404;
            response.body = "{\"error\":\"Funcionário não encontrado\"}";
        }
        
    } catch (const exception& e) {
        response.statusCode = 400;
        response.body = "{\"error\":\"" + string(e.what()) + "\"}";
    }
    
    return response;
}

HttpResponse ServidorHTTP::handleGetPontos(const HttpRequest& req) {
    (void)req; // Parâmetro não utilizado
    HttpResponse response;
    response.body = pontosToJson(sistema.getPontos());
    return response;
}

HttpResponse ServidorHTTP::handlePostPonto(const HttpRequest& req) {
    HttpResponse response;
    
    try {
        auto extrair = [](const string& json, const string& campo) -> string {
            size_t pos = json.find("\"" + campo + "\"");
            if (pos == string::npos) return "";
            pos = json.find(":", pos);
            if (pos == string::npos) return "";
            pos = json.find_first_not_of(" \t\n\r\"", pos + 1);
            size_t end = json.find_first_of(",}\"\n\r", pos);
            return json.substr(pos, end - pos);
        };
        
        int idFunc = stoi(extrair(req.body, "idFuncionario"));
        string data = extrair(req.body, "data");
        string hora = extrair(req.body, "hora");
        string tipo = extrair(req.body, "tipo");
        string obs = extrair(req.body, "observacao");
        
        RegistroPonto novoPonto(idFunc, data, hora, tipo, obs);
        sistema.adicionarPonto(novoPonto);
        
        // Salvar no banco de dados se conectado
        if (db && db->isConectado()) {
            cout << "💾 Salvando ponto no MySQL..." << endl;
            if (!db->inserirPonto(novoPonto)) {
                cout << "❌ Erro ao salvar ponto no banco de dados!" << endl;
                response.statusCode = 500;
                response.body = "{\"error\":\"Erro ao salvar ponto no banco de dados\"}";
                return response;
            }
            cout << "✅ Ponto salvo no MySQL com sucesso!" << endl;
        } else {
            cout << "⚠️  Banco não conectado. Salvando ponto em arquivo..." << endl;
            // Fallback para arquivo
            Persistencia::exportarPontos(sistema, "pontos.txt");
        }
        
        response.statusCode = 201;
        response.body = "{\"message\":\"Ponto registrado com sucesso\"}";
    } catch (const exception& e) {
        response.statusCode = 400;
        response.body = "{\"error\":\"" + string(e.what()) + "\"}";
    }
    
    return response;
}

HttpResponse ServidorHTTP::handleGetSetores(const HttpRequest& req) {
    (void)req; // Parâmetro não utilizado
    HttpResponse response;
    response.body = setoresToJson(sistema.getSetores());
    return response;
}

void ServidorHTTP::registrarRotas() {
    rotas["GET /api/funcionarios"] = [this](const HttpRequest& req) { 
        return handleGetFuncionarios(req); 
    };
    rotas["POST /api/funcionarios"] = [this](const HttpRequest& req) { 
        return handlePostFuncionario(req); 
    };
    rotas["GET /api/funcionarios/"] = [this](const HttpRequest& req) { 
        return handleGetFuncionario(req); 
    };
    rotas["DELETE /api/funcionarios/"] = [this](const HttpRequest& req) { 
        return handleDeleteFuncionario(req); 
    };
    rotas["PUT /api/funcionarios/"] = [this](const HttpRequest& req) { 
        return handlePutFuncionario(req); 
    };
    rotas["GET /api/pontos"] = [this](const HttpRequest& req) { 
        return handleGetPontos(req); 
    };
    rotas["POST /api/pontos"] = [this](const HttpRequest& req) { 
        return handlePostPonto(req); 
    };
    rotas["GET /api/setores"] = [this](const HttpRequest& req) { 
        return handleGetSetores(req); 
    };
}

void ServidorHTTP::iniciar() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Erro ao criar socket" << endl;
        return;
    }
    
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(porta);
    
    if (::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Erro no bind" << endl;
        close(serverSocket);
        return;
    }
    
    if (listen(serverSocket, 10) < 0) {
        cerr << "Erro no listen" << endl;
        close(serverSocket);
        return;
    }
    
    cout << "\n==============================================\n";
    cout << "⚡ tchERP - Servidor HTTP iniciado na porta " << porta << endl;
    cout << "🏠 Página Inicial: http://localhost:" << porta << "/" << endl;
    cout << "📂 Frontend: http://localhost:" << porta << "/frontend/" << endl;
    cout << "🔌 API: http://localhost:" << porta << "/api/" << endl;
    cout << "==============================================\n";
    cout << "🌾 Bah, tchê! Sistema gaúcho rodando! 🧉\n";
    cout << "==============================================\n\n";
    
    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (clientSocket < 0) continue;
        
        char buffer[4096] = {0};
        read(clientSocket, buffer, 4096);
        
        HttpRequest request = parseRequest(string(buffer));
        HttpResponse response;
        
        cout << "📥 " << request.method << " " << request.path << endl;
        
        // Tratar OPTIONS (CORS preflight)
        if (request.method == "OPTIONS") {
            response.statusCode = 200;
            response.body = "";
        }
        // Servir arquivos estáticos
        else if (request.path.find("/frontend/") == 0 || 
                 request.path.find("/fotos/") == 0 ||
                 request.path == "/") {
            string filePath = request.path == "/" ? "./frontend/index.html" : "." + request.path;
            string conteudo = lerArquivo(filePath);
            
            if (!conteudo.empty()) {
                response.body = conteudo;
                response.contentType = getContentType(filePath);
            } else {
                response.statusCode = 404;
                response.body = "Arquivo não encontrado";
            }
        }
        // Rotas da API
        else {
            string routeKey = request.method + " " + request.path;
            
            // Buscar rota exata ou com parâmetro
            auto it = rotas.find(routeKey);
            if (it == rotas.end()) {
                // Tentar rotas com parâmetros (ex: /api/funcionarios/123)
                for (const auto& [key, handler] : rotas) {
                    if (key.find(request.method) == 0 && 
                        request.path.find(key.substr(request.method.length() + 1)) == 0) {
                        response = handler(request);
                        goto found;
                    }
                }
                
                response.statusCode = 404;
                response.body = "{\"error\":\"Rota não encontrada\"}";
            } else {
                response = it->second(request);
            }
            found:;
        }
        
        string responseStr = buildResponse(response);
        send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
        close(clientSocket);
    }
    
    close(serverSocket);
}
