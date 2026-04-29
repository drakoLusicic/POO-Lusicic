<?php
require_once __DIR__ . '/config/db.php';

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    header('Location: index.php');
    exit;
}

$nombre = trim($_POST['nombre'] ?? '');
$cuatrimestre = trim($_POST['cuatrimestre'] ?? '');
$anio = (int)($_POST['anio'] ?? 0);
$profesor = trim($_POST['profesor'] ?? '');

if ($nombre === '' || $cuatrimestre === '' || $anio <= 0 || $profesor === '') {
    header('Location: index.php?msg=' . urlencode('Faltan datos para guardar la materia.'));
    exit;
}

$stmt = $pdo->prepare("INSERT INTO materias (nombre, cuatrimestre, anio, profesor) VALUES (?, ?, ?, ?)");
$stmt->execute([$nombre, $cuatrimestre, $anio, $profesor]);

header('Location: index.php?msg=' . urlencode('Materia agregada correctamente.'));
exit;
