<?php
function h($value) {
    return htmlspecialchars((string)$value, ENT_QUOTES, 'UTF-8');
}

function calcularPromedio(array $notas): float {
    if (count($notas) === 0) return 0;
    $suma = 0;
    foreach ($notas as $nota) {
        $suma += (float)$nota['nota'];
    }
    return round($suma / count($notas), 2);
}

function obtenerEstado(float $promedio): string {
    if ($promedio >= 8) return 'Promocionado';
    if ($promedio >= 4) return 'Regular';
    if ($promedio >= 1) return 'Desaprobado';
    return 'Sin notas';
}

function obtenerClaseEstado(float $promedio): array {
    if ($promedio >= 8) {
        return [
            'barra' => 'bg-green-500',
            'badge' => 'bg-green-100 text-green-700 border-green-200',
            'porcentaje' => min(100, ($promedio / 10) * 100),
        ];
    }
    if ($promedio >= 4) {
        return [
            'barra' => 'bg-yellow-400',
            'badge' => 'bg-yellow-100 text-yellow-700 border-yellow-200',
            'porcentaje' => min(100, ($promedio / 10) * 100),
        ];
    }
    if ($promedio >= 1) {
        return [
            'barra' => 'bg-red-500',
            'badge' => 'bg-red-100 text-red-700 border-red-200',
            'porcentaje' => min(100, ($promedio / 10) * 100),
        ];
    }
    return [
        'barra' => 'bg-slate-300',
        'badge' => 'bg-slate-100 text-slate-600 border-slate-200',
        'porcentaje' => 0,
    ];
}
