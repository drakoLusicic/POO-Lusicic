<?php
require_once __DIR__ . '/config/db.php';

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    header('Location: index.php');
    exit;
}

$materiaId = (int)($_POST['materia_id'] ?? 0);
$tipo = trim($_POST['tipo_evaluacion'] ?? '');
$nota = (float)($_POST['nota'] ?? 0);
$fecha = $_POST['fecha_nota'] ?? '';
$obs = trim($_POST['observaciones'] ?? '');

if ($materiaId <= 0 || $tipo === '' || $nota < 1 || $nota > 10 || $fecha === '') {
    header('Location: index.php?msg=' . urlencode('No se pudo guardar la nota. Revisá los datos.'));
    exit;
}

$stmt = $pdo->prepare("INSERT INTO notas (materia_id, tipo_evaluacion, nota, fecha_nota, observaciones) VALUES (?, ?, ?, ?, ?)");
$stmt->execute([$materiaId, $tipo, $nota, $fecha, $obs]);

header('Location: index.php?msg=' . urlencode('Nota agregada correctamente.'));
exit;
