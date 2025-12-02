#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

// Constantes do sistema
const int MAX_FUNCIONARIOS = 1000;
const int MAX_SETORES = 20;
const int MAX_CARGOS = 20;
const int MAX_PONTOS = 10000;
const int ID_MIN = 1;
const int ID_MAX = 99999;
const int CPF_LENGTH = 11;
const int ID_LENGTH = 5;

// Forward declarations para evitar dependências circulares
class Funcionario;
class Setor;
class Cargo;
class RegistroPonto;

#endif