#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

using namespace std;

class Config {
private:
    map<string, map<string, string>> dados;
    
public:
    Config();
    bool carregar(const string& arquivo);
    string get(const string& secao, const string& chave, const string& padrao = "");
    int getInt(const string& secao, const string& chave, int padrao = 0);
    bool getBool(const string& secao, const string& chave, bool padrao = false);
};

#endif
