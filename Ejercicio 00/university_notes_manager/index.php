<?php
require_once __DIR__ . '/config/db.php';
require_once __DIR__ . '/functions.php';

$mensaje = $_GET['msg'] ?? '';

$materiasStmt = $pdo->query("SELECT * FROM materias ORDER BY anio DESC, cuatrimestre ASC, nombre ASC");
$materias = $materiasStmt->fetchAll();

$notasPorMateria = [];
$notasStmt = $pdo->query("SELECT * FROM notas ORDER BY fecha_nota ASC, id ASC");
foreach ($notasStmt->fetchAll() as $nota) {
    $notasPorMateria[$nota['materia_id']][] = $nota;
}

$totalMaterias = count($materias);
$promocionadas = 0;
$regulares = 0;
$desaprobadas = 0;
$sinNotas = 0;
?>
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gestor de Notas Universitarias</title>
    <script src="https://cdn.tailwindcss.com"></script>
</head>
<body class="bg-slate-100 text-slate-800 min-h-screen">
    <div class="max-w-7xl mx-auto px-4 py-8">
        <header class="mb-8">
            <div class="bg-white rounded-3xl shadow-sm border border-slate-200 p-6 md:p-8">
                <div class="flex flex-col gap-6 md:flex-row md:items-center md:justify-between">
                    <div>
                        <p class="text-sm font-semibold uppercase tracking-[0.2em] text-sky-600">Sistema web local</p>
                        <h1 class="text-3xl md:text-4xl font-bold mt-2">Gestor de notas por materia</h1>
                        <p class="text-slate-500 mt-2">Cargá materias, agregá parciales y controlá tu estado académico por promedio.</p>
                    </div>
                    <button
                        type="button"
                        onclick="openCreateSubjectModal()"
                        class="px-5 py-3 rounded-2xl bg-sky-600 text-white font-semibold hover:bg-sky-700 transition"
                    >
                        + Agregar materia
                    </button>
                </div>
            </div>
        </header>

        <?php if ($mensaje): ?>
            <div class="mb-6 rounded-2xl border border-green-200 bg-green-50 text-green-700 px-4 py-3">
                <?= h($mensaje) ?>
            </div>
        <?php endif; ?>

        <section class="grid grid-cols-1 md:grid-cols-4 gap-4 mb-8">
            <?php foreach ($materias as $materia): ?>
                <?php
                    $notasMateria = $notasPorMateria[$materia['id']] ?? [];
                    $promedio = calcularPromedio($notasMateria);
                    $estado = obtenerEstado($promedio);
                    if ($estado === 'Promocionado') $promocionadas++;
                    elseif ($estado === 'Regular') $regulares++;
                    elseif ($estado === 'Desaprobado') $desaprobadas++;
                    else $sinNotas++;
                ?>
            <?php endforeach; ?>

            <div class="bg-white border border-slate-200 rounded-3xl p-5 shadow-sm">
                <p class="text-slate-500 text-sm">Total materias</p>
                <p class="text-3xl font-bold mt-1"><?= $totalMaterias ?></p>
            </div>
            <div class="bg-white border border-slate-200 rounded-3xl p-5 shadow-sm">
                <p class="text-slate-500 text-sm">Promocionadas</p>
                <p class="text-3xl font-bold mt-1 text-green-600"><?= $promocionadas ?></p>
            </div>
            <div class="bg-white border border-slate-200 rounded-3xl p-5 shadow-sm">
                <p class="text-slate-500 text-sm">Regulares</p>
                <p class="text-3xl font-bold mt-1 text-yellow-500"><?= $regulares ?></p>
            </div>
            <div class="bg-white border border-slate-200 rounded-3xl p-5 shadow-sm">
                <p class="text-slate-500 text-sm">Desaprobadas / sin notas</p>
                <p class="text-3xl font-bold mt-1 text-red-500"><?= ($desaprobadas + $sinNotas) ?></p>
            </div>
        </section>

        <section>
            <div class="flex items-center justify-between mb-4">
                <h2 class="text-2xl font-bold">Tus materias</h2>
            </div>

            <?php if (!$materias): ?>
                <div class="bg-white border border-dashed border-slate-300 rounded-3xl p-10 text-center text-slate-500">
                    Todavía no hay materias cargadas. Empezá agregando la primera.
                </div>
            <?php else: ?>
                <div class="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3 gap-6">
                    <?php foreach ($materias as $materia): ?>
                        <?php
                            $notasMateria = $notasPorMateria[$materia['id']] ?? [];
                            $promedio = calcularPromedio($notasMateria);
                            $estado = obtenerEstado($promedio);
                            $estilo = obtenerClaseEstado($promedio);
                            $payload = [
                                'id' => (int)$materia['id'],
                                'nombre' => $materia['nombre'],
                                'profesor' => $materia['profesor'],
                                'cuatrimestre' => $materia['cuatrimestre'],
                                'anio' => $materia['anio'],
                                'notas' => array_map(function($n) {
                                    return [
                                        'id' => (int)$n['id'],
                                        'tipo_evaluacion' => $n['tipo_evaluacion'],
                                        'nota' => (float)$n['nota'],
                                        'fecha_nota' => $n['fecha_nota'],
                                        'observaciones' => $n['observaciones'],
                                    ];
                                }, $notasMateria),
                            ];
                        ?>
                        <button
                            type="button"
                            onclick='openSubjectModal(<?= json_encode($payload, JSON_UNESCAPED_UNICODE | JSON_HEX_APOS | JSON_HEX_QUOT) ?>)'
                            class="text-left bg-white rounded-3xl border border-slate-200 shadow-sm p-6 hover:-translate-y-1 hover:shadow-md transition"
                        >
                            <div class="flex items-start justify-between gap-4 mb-5">
                                <div>
                                    <h3 class="text-xl font-bold leading-tight"><?= h($materia['nombre']) ?></h3>
                                    <p class="text-slate-500 mt-1"><?= h($materia['cuatrimestre']) ?> · <?= h($materia['anio']) ?></p>
                                </div>
                                <span class="px-3 py-1 rounded-full border text-xs font-semibold <?= h($estilo['badge']) ?>">
                                    <?= h($estado) ?>
                                </span>
                            </div>

                            <div class="space-y-3">
                                <div>
                                    <p class="text-sm text-slate-500">Profesor</p>
                                    <p class="font-semibold"><?= h($materia['profesor']) ?></p>
                                </div>

                                <div>
                                    <div class="flex items-center justify-between text-sm mb-2">
                                        <span class="text-slate-500">Promedio actual</span>
                                        <span class="font-bold js-card-average"><?= number_format($promedio, 2) ?></span>
                                    </div>
                                    <div class="w-full h-3 bg-slate-200 rounded-full overflow-hidden">
                                        <div class="h-full <?= h($estilo['barra']) ?> transition-all duration-500" style="width: <?= h($estilo['porcentaje']) ?>%"></div>
                                    </div>
                                </div>

                                <div class="flex items-center justify-between text-sm text-slate-500">
                                    <span><?= count($notasMateria) ?> evaluación(es)</span>
                                    <span>Click para ver detalle</span>
                                </div>
                            </div>
                        </button>
                    <?php endforeach; ?>
                </div>
            <?php endif; ?>
        </section>
    </div>

    <div id="subjectModal" class="hidden fixed inset-0 z-50">
        <div class="absolute inset-0 bg-slate-950/50" onclick="closeSubjectModal()"></div>
        <div class="relative z-10 max-w-4xl mx-auto my-8 bg-white rounded-3xl shadow-xl border border-slate-200 overflow-hidden">
            <div class="p-6 md:p-8 border-b border-slate-200 flex items-start justify-between gap-4">
                <div>
                    <p class="text-sm uppercase tracking-[0.2em] text-slate-400">Detalle de materia</p>
                    <h3 id="modalSubjectName" class="text-2xl font-bold mt-2">Materia</h3>
                    <p id="modalSubjectMeta" class="text-slate-500 mt-1"></p>
                </div>
                <button type="button" onclick="closeSubjectModal()" class="w-10 h-10 rounded-full bg-slate-100 hover:bg-slate-200 text-xl">×</button>
            </div>

            <div class="p-6 md:p-8 space-y-8 max-h-[75vh] overflow-y-auto">
                <div class="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div class="rounded-2xl bg-slate-50 border border-slate-200 p-4">
                        <p class="text-sm text-slate-500">Profesor</p>
                        <p id="modalProfessor" class="text-lg font-semibold mt-1"></p>
                    </div>
                    <div class="rounded-2xl bg-slate-50 border border-slate-200 p-4">
                        <p class="text-sm text-slate-500">Promedio</p>
                        <p id="modalAverage" class="text-lg font-semibold mt-1"></p>
                    </div>
                    <div class="rounded-2xl bg-slate-50 border border-slate-200 p-4">
                        <p class="text-sm text-slate-500">Estado</p>
                        <p id="modalStatus" class="text-lg font-semibold mt-1"></p>
                    </div>
                </div>

                <div>
                    <div class="flex items-center justify-between mb-3">
                        <h4 class="text-xl font-bold">Evaluaciones</h4>
                    </div>
                    <div id="gradesContainer" class="space-y-3"></div>
                </div>

                <div class="grid grid-cols-1 lg:grid-cols-2 gap-6">
                    <form action="save_grade.php" method="POST" class="rounded-3xl border border-slate-200 p-5 bg-slate-50 space-y-4">
                        <h5 class="text-lg font-bold">Agregar nueva nota</h5>
                        <input type="hidden" name="materia_id" id="addGradeSubjectId">

                        <div>
                            <label class="block text-sm font-medium mb-1">Tipo de evaluación</label>
                            <input type="text" name="tipo_evaluacion" required placeholder="Ej: Parcial 1, TP, Final" class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                        </div>

                        <div>
                            <label class="block text-sm font-medium mb-1">Nota</label>
                            <input type="number" name="nota" min="1" max="10" step="0.01" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                        </div>

                        <div>
                            <label class="block text-sm font-medium mb-1">Fecha</label>
                            <input type="date" name="fecha_nota" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                        </div>

                        <div>
                            <label class="block text-sm font-medium mb-1">Observaciones</label>
                            <textarea name="observaciones" rows="3" class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500"></textarea>
                        </div>

                        <button type="submit" class="px-5 py-3 rounded-2xl bg-sky-600 text-white font-semibold hover:bg-sky-700 transition">Guardar nota</button>
                    </form>

                    <form action="update_grade.php" method="POST" class="rounded-3xl border border-slate-200 p-5 bg-white space-y-4">
                        <h5 class="text-lg font-bold">Modificar nota existente</h5>
                        <input type="hidden" name="materia_id" id="editGradeSubjectId">

                        <div>
                            <label class="block text-sm font-medium mb-1">Nota a editar</label>
                            <select name="nota_id" id="editGradeSelect" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500"></select>
                        </div>

                        <div>
                            <label class="block text-sm font-medium mb-1">Nuevo tipo</label>
                            <input type="text" name="tipo_evaluacion" id="editTipo" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                        </div>

                        <div>
                            <label class="block text-sm font-medium mb-1">Nueva nota</label>
                            <input type="number" name="nota" id="editNota" min="1" max="10" step="0.01" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                        </div>

                        <div>
                            <label class="block text-sm font-medium mb-1">Nueva fecha</label>
                            <input type="date" name="fecha_nota" id="editFecha" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                        </div>

                        <div>
                            <label class="block text-sm font-medium mb-1">Observaciones</label>
                            <textarea name="observaciones" rows="3" id="editObservaciones" class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500"></textarea>
                        </div>

                        <button type="submit" class="px-5 py-3 rounded-2xl bg-amber-500 text-white font-semibold hover:bg-amber-600 transition">Actualizar nota</button>
                    </form>
                </div>

                <form action="delete_grade.php" method="POST" class="rounded-3xl border border-red-200 p-5 bg-red-50 space-y-4">
                    <h5 class="text-lg font-bold text-red-700">Eliminar nota</h5>
                    <input type="hidden" name="materia_id" id="deleteGradeSubjectId">
                    <div>
                        <label class="block text-sm font-medium mb-1">Seleccioná la nota a eliminar</label>
                        <select name="nota_id" id="deleteGradeSelect" required class="w-full rounded-2xl border border-red-300 px-4 py-3 outline-none focus:ring-2 focus:ring-red-500"></select>
                    </div>
                    <button type="submit" onclick="return confirm('¿Seguro que querés eliminar esta nota?');" class="px-5 py-3 rounded-2xl bg-red-600 text-white font-semibold hover:bg-red-700 transition">Eliminar nota seleccionada</button>
                </form>
            </div>
        </div>
    </div>

    <div id="createSubjectModal" class="hidden fixed inset-0 z-50">
        <div class="absolute inset-0 bg-slate-950/50" onclick="closeCreateSubjectModal()"></div>
        <div class="relative z-10 max-w-2xl mx-auto my-10 bg-white rounded-3xl shadow-xl border border-slate-200 overflow-hidden">
            <div class="p-6 border-b border-slate-200 flex items-center justify-between">
                <div>
                    <p class="text-sm uppercase tracking-[0.2em] text-slate-400">Nueva materia</p>
                    <h3 class="text-2xl font-bold mt-2">Agregar materia</h3>
                </div>
                <button type="button" onclick="closeCreateSubjectModal()" class="w-10 h-10 rounded-full bg-slate-100 hover:bg-slate-200 text-xl">×</button>
            </div>
            <form action="save_subject.php" method="POST" class="p-6 space-y-4">
                <div>
                    <label class="block text-sm font-medium mb-1">Nombre de la materia</label>
                    <input type="text" name="nombre" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                </div>
                <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
                    <div>
                        <label class="block text-sm font-medium mb-1">Cuatrimestre</label>
                        <select name="cuatrimestre" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                            <option value="1° Cuatrimestre">1° Cuatrimestre</option>
                            <option value="2° Cuatrimestre">2° Cuatrimestre</option>
                            <option value="Anual">Anual</option>
                            <option value="Curso de verano">Curso de verano</option>
                        </select>
                    </div>
                    <div>
                        <label class="block text-sm font-medium mb-1">Año</label>
                        <input type="number" name="anio" min="2020" max="2100" value="<?= date('Y') ?>" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                    </div>
                </div>
                <div>
                    <label class="block text-sm font-medium mb-1">Profesor</label>
                    <input type="text" name="profesor" required class="w-full rounded-2xl border border-slate-300 px-4 py-3 outline-none focus:ring-2 focus:ring-sky-500">
                </div>
                <button type="submit" class="px-5 py-3 rounded-2xl bg-slate-900 text-white font-semibold hover:bg-slate-800 transition">Guardar materia</button>
            </form>
        </div>
    </div>

    <script src="assets/js/app.js"></script>
</body>
</html>
