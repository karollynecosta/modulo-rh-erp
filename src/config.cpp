#include "../include/config.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Config::Config() {}

bool Config::carregar(const string& arquivo) {
    ifstream file(arquivo);
    if (!file.is_open()) {
        cerr << "Arquivo de configuração não encontrado: " << arquivo << endl;
        return false;
    }
    
    string linha;
    string secaoAtual = "";
    
    while (getline(file, linha)) {
        // Remover espaços
        linha.erase(0, linha.find_first_not_of(" \t\r\n"));
        linha.erase(linha.find_last_not_of(" \t\r\n") + 1);
        
        // Ignorar linhas vazias e comentários
        if (linha.empty() || linha[0] == '#' || linha[0] == ';') {
            continue;
        }
        
        // Detectar seção [secao]
        if (linha[0] == '[' && linha[linha.length()-1] == ']') {
            secaoAtual = linha.substr(1, linha.length() - 2);
            continue;
        }
        
        // Ler chave=valor
        size_t pos = linha.find('=');
        if (pos != string::npos && !secaoAtual.empty()) {
            string chave = linha.substr(0, pos);
            string valor = linha.substr(pos + 1);
            
            // Remover espaços
            chave.erase(0, chave.find_first_not_of(" \t"));
            chave.erase(chave.find_last_not_of(" \t") + 1);
            valor.erase(0, valor.find_first_not_of(" \t"));
            valor.erase(valor.find_last_not_of(" \t") + 1);
            
            dados[secaoAtual][chave] = valor;
        }
    }
    
    file.close();
    cout << "✅ Configuração carregada de: " << arquivo << endl;
    return true;
}

string Config::get(const string& secao, const string& chave, const string& padrao) {
    if (dados.find(secao) != dados.end()) {
        if (dados[secao].find(chave) != dados[secao].end()) {
            return dados[secao][chave];
        }
    }
    return padrao;
}

int Config::getInt(const string& secao, const string& chave, int padrao) {
    string valor = get(secao, chave);
    if (valor.empty()) {
        return padrao;
    }
    try {
        return stoi(valor);
    } catch (...) {
        return padrao;
    }
}

bool Config::getBool(const string& secao, const string& chave, bool padrao) {
    string valor = get(secao, chave);
    if (valor.empty()) {
        return padrao;
    }
    return valor == "true" || valor == "1" || valor == "yes";
}
