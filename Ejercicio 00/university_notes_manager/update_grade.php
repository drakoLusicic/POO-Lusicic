<?php
require_once __DIR__ . '/config/db.php';

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    header('Location: index.php');
    exit;
}

$notaId = (int)($_POST['nota_id'] ?? 0);
$materiaId = (int)($_POST['materia_id'] ?? 0);
$tipo = trim($_POST['tipo_evaluacion'] ?? '');
$nota = (float)($_POST['nota'] ?? 0);
$fecha = $_POST['fecha_nota'] ?? '';
$obs = trim($_POST['observaciones'] ?? '');

if ($notaId <= 0 || $materiaId <= 0 || $tipo === '' || $nota < 1 || $nota > 10 || $fecha === '') {
    header('Location: index.php?msg=' . urlencode('No se pudo actualizar la nota.'));
    exit;
}

$stmt = $pdo->prepare("UPDATE notas SET tipo_evaluacion = ?, nota = ?, fecha_nota = ?, observaciones = ? WHERE id = ? AND materia_id = ?");
$stmt->execute([$tipo, $nota, $fecha, $obs, $notaId, $materiaId]);

header('Location: index.php?msg=' . urlencode('Nota actualizada correctamente.'));
exit;
