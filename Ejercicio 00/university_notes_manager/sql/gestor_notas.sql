CREATE DATABASE IF NOT EXISTS gestor_notas CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE gestor_notas;

CREATE TABLE IF NOT EXISTS materias (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(150) NOT NULL,
    cuatrimestre VARCHAR(50) NOT NULL,
    anio INT NOT NULL,
    profesor VARCHAR(120) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS notas (
    id INT AUTO_INCREMENT PRIMARY KEY,
    materia_id INT NOT NULL,
    tipo_evaluacion VARCHAR(100) NOT NULL,
    nota DECIMAL(4,2) NOT NULL,
    fecha_nota DATE NOT NULL,
    observaciones TEXT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_notas_materias FOREIGN KEY (materia_id) REFERENCES materias(id) ON DELETE CASCADE
);

INSERT INTO materias (nombre, cuatrimestre, anio, profesor) VALUES
('Análisis Matemático III', '1° Cuatrimestre', 2026, 'Ing. Pérez'),
('Base de Datos II', '1° Cuatrimestre', 2026, 'Lic. Gómez');

INSERT INTO notas (materia_id, tipo_evaluacion, nota, fecha_nota, observaciones) VALUES
(1, 'Parcial 1', 8.50, '2026-03-01', 'Buen desempeño en complejas'),
(1, 'TP 1', 9.00, '2026-03-05', 'Entrega completa'),
(2, 'Parcial 1', 6.00, '2026-03-02', 'Hay que reforzar joins');
