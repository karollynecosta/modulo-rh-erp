-- Script de inicialização do banco de dados tchERP
-- Execute este script antes de iniciar o sistema

-- Criar banco de dados
CREATE DATABASE IF NOT EXISTS tcherp CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE tcherp;

-- Criar usuário (opcional - ajuste conforme necessário)
-- CREATE USER IF NOT EXISTS 'tcherp_user'@'localhost' IDENTIFIED BY 'tcherp_senha';
-- GRANT ALL PRIVILEGES ON tcherp.* TO 'tcherp_user'@'localhost';
-- FLUSH PRIVILEGES;

-- Tabela de setores (criar primeiro - não tem dependências)
CREATE TABLE IF NOT EXISTS setores (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(100) UNIQUE NOT NULL,
    descricao TEXT,
    INDEX idx_nome (nome)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Tabela de cargos (depende de setores)
CREATE TABLE IF NOT EXISTS cargos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    id_setor INT NOT NULL,
    nome VARCHAR(100) NOT NULL,
    salario_base DECIMAL(10,2),
    FOREIGN KEY (id_setor) REFERENCES setores(id) ON DELETE CASCADE,
    UNIQUE KEY unique_cargo_setor (id_setor, nome),
    INDEX idx_nome (nome)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Tabela de funcionários - rh (depende de setores e cargos)
CREATE TABLE IF NOT EXISTS funcionarios (
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
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Tabela de pontos (depende de rh)
CREATE TABLE IF NOT EXISTS pontos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    id_funcionario INT NOT NULL,
    data VARCHAR(10) NOT NULL,
    hora VARCHAR(8) NOT NULL,
    tipo ENUM('ENTRADA', 'SAIDA') NOT NULL,
    observacao VARCHAR(255),
    created_at TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_funcionario) REFERENCES funcionarios(id) ON DELETE CASCADE,
    INDEX idx_funcionario (id_funcionario),
    INDEX idx_data (data)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ===== DADOS INICIAIS =====

-- Inserir setores padrão
INSERT INTO setores (nome, descricao) VALUES
    ('TI', 'Tecnologia da Informação'),
    ('RH', 'Recursos Humanos'),
    ('Financeiro', 'Setor Financeiro'),
    ('Comercial', 'Setor Comercial'),
    ('Operacional', 'Setor Operacional')
ON DUPLICATE KEY UPDATE nome=nome;

-- Inserir cargos padrão para cada setor
INSERT INTO cargos (id_setor, nome, salario_base) VALUES
    -- TI (id=1)
    ((SELECT id FROM setores WHERE nome='TI'), 'Desenvolvedor', 5000.00),
    ((SELECT id FROM setores WHERE nome='TI'), 'Analista', 6000.00),
    ((SELECT id FROM setores WHERE nome='TI'), 'Coordenador', 8000.00),
    ((SELECT id FROM setores WHERE nome='TI'), 'Gerente', 10000.00),
    
    -- RH (id=2)
    ((SELECT id FROM setores WHERE nome='RH'), 'Assistente', 3000.00),
    ((SELECT id FROM setores WHERE nome='RH'), 'Analista', 5000.00),
    ((SELECT id FROM setores WHERE nome='RH'), 'Coordenador', 7000.00),
    ((SELECT id FROM setores WHERE nome='RH'), 'Gerente', 9000.00),
    
    -- Financeiro (id=3)
    ((SELECT id FROM setores WHERE nome='Financeiro'), 'Auxiliar', 2500.00),
    ((SELECT id FROM setores WHERE nome='Financeiro'), 'Analista', 5500.00),
    ((SELECT id FROM setores WHERE nome='Financeiro'), 'Coordenador', 7500.00),
    ((SELECT id FROM setores WHERE nome='Financeiro'), 'Gerente', 10000.00),
    
    -- Comercial (id=4)
    ((SELECT id FROM setores WHERE nome='Comercial'), 'Vendedor', 2000.00),
    ((SELECT id FROM setores WHERE nome='Comercial'), 'Supervisor', 4000.00),
    ((SELECT id FROM setores WHERE nome='Comercial'), 'Coordenador', 6000.00),
    ((SELECT id FROM setores WHERE nome='Comercial'), 'Gerente', 8000.00),
    
    -- Operacional (id=5)
    ((SELECT id FROM setores WHERE nome='Operacional'), 'Operador', 2000.00),
    ((SELECT id FROM setores WHERE nome='Operacional'), 'Técnico', 3500.00),
    ((SELECT id FROM setores WHERE nome='Operacional'), 'Supervisor', 5000.00),
    ((SELECT id FROM setores WHERE nome='Operacional'), 'Coordenador', 7000.00)
ON DUPLICATE KEY UPDATE nome=nome;

SELECT 'Banco de dados tchERP criado com sucesso!' as Status;
SELECT CONCAT('Setores cadastrados: ', COUNT(*)) as Total FROM setores;
SELECT CONCAT('Cargos cadastrados: ', COUNT(*)) as Total FROM cargos;
