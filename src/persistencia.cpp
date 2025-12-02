#include "../include/persistencia.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void Persistencia::importarDeArquivo(SistemaRH& sistema, const string& arquivo) {
    ifstream file(arquivo);
    if (!file.is_open()) {
        cout << "Não foi possível abrir o arquivo: " << arquivo << endl;
        return;
    }
    
    string linha;
    int count = 0;
    
    while (getline(file, linha)) {
        if (linha.empty()) continue;
        
        // Formato: id;nome;cpf;setor;cargo;foto
        stringstream ss(linha);
        string idStr, nome, cpf, setor, cargo, foto;
        
        if (getline(ss, idStr, ';') &&
            getline(ss, nome, ';') &&
            getline(ss, cpf, ';') &&
            getline(ss, setor, ';') &&
            getline(ss, cargo, ';') &&
            getline(ss, foto)) {
            
            try {
                int id = stoi(idStr);
                Funcionario func(id, nome, cpf, setor, cargo, foto);
                sistema.adicionarFuncionario(func);
                count++;
            } catch (const exception& e) {
                cout << "Erro ao processar linha: " << linha << endl;
            }
        }
    }
    
    file.close();
    cout << "Importados " << count << " funcionários de " << arquivo << endl;
}

void Persistencia::exportarParaArquivo(const SistemaRH& sistema, const string& arquivo) {
    ofstream file(arquivo);
    if (!file.is_open()) {
        cout << "Não foi possível criar o arquivo: " << arquivo << endl;
        return;
    }
    
    const auto& funcionarios = sistema.getFuncionarios();
    int count = 0;
    
    for (const auto& func : funcionarios) {
        file << func.formatarParaArquivo() << endl;
        count++;
    }
    
    file.close();
    cout << "Exportados " << count << " funcionários para " << arquivo << endl;
}

void Persistencia::importarPontos(SistemaRH& sistema, const string& arquivo) {
    ifstream file(arquivo);
    if (!file.is_open()) {
        cout << "Não foi possível abrir o arquivo: " << arquivo << endl;
        return;
    }
    
    string linha;
    int count = 0;
    
    while (getline(file, linha)) {
        if (linha.empty()) continue;
        
        // Formato: idFuncionario|data|hora|tipo|observacao
        stringstream ss(linha);
        string idStr, data, hora, tipo, observacao;
        
        if (getline(ss, idStr, '|') &&
            getline(ss, data, '|') &&
            getline(ss, hora, '|') &&
            getline(ss, tipo, '|') &&
            getline(ss, observacao)) {
            
            try {
                int idFuncionario = stoi(idStr);
                RegistroPonto ponto;
                ponto.setIdFuncionario(idFuncionario);
                ponto.setData(data);
                ponto.setHora(hora);
                ponto.setTipo(tipo);
                ponto.setObservacao(observacao);
                
                sistema.adicionarPonto(ponto);
                count++;
            } catch (const exception& e) {
                cout << "Erro ao processar linha: " << linha << endl;
            }
        }
    }
    
    file.close();
    cout << "Importados " << count << " registros de ponto de " << arquivo << endl;
}

void Persistencia::exportarPontos(const SistemaRH& sistema, const string& arquivo) {
    ofstream file(arquivo);
    if (!file.is_open()) {
        cout << "Não foi possível criar o arquivo: " << arquivo << endl;
        return;
    }
    
    const auto& pontos = sistema.getPontos();
    int count = 0;
    
    for (const auto& ponto : pontos) {
        file << ponto.getIdFuncionario() << ";"
             << ponto.getData() << ";"
             << ponto.getHora() << ";"
             << ponto.getTipo() << ";"
             << ponto.getObservacao() << endl;
        count++;
    }
    
    file.close();
    cout << "Exportados " << count << " registros de ponto para " << arquivo << endl;
}