<?php
require_once __DIR__ . '/config/db.php';

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    header('Location: index.php');
    exit;
}

$notaId = (int)($_POST['nota_id'] ?? 0);
$materiaId = (int)($_POST['materia_id'] ?? 0);

if ($notaId <= 0 || $materiaId <= 0) {
    header('Location: index.php?msg=' . urlencode('No se pudo eliminar la nota.'));
    exit;
}

$stmt = $pdo->prepare("DELETE FROM notas WHERE id = ? AND materia_id = ?");
$stmt->execute([$notaId, $materiaId]);

header('Location: index.php?msg=' . urlencode('Nota eliminada correctamente.'));
exit;
