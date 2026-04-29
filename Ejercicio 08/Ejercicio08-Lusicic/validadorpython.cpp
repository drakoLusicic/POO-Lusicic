#include "validadorpython.h"

bool ValidadorPython::validar(const QString &linea) {
    QString trimmed = linea.trimmed();
    if (trimmed.isEmpty()) return true;
    // Python no requiere ;, pero verificar indentación básica
    return true; // Simplificado
}

QString ValidadorPython::mensajeError() const {
    return "Línea inválida: verifica la sintaxis de Python.";
}