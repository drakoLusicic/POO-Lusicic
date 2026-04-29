#include "validadorjava.h"

bool ValidadorJava::validar(const QString &linea) {
    QString trimmed = linea.trimmed();
    if (trimmed.isEmpty()) return true;
    if (trimmed.endsWith(";") || trimmed.contains("{")) return true;
    return false;
}

QString ValidadorJava::mensajeError() const {
    return "Línea inválida: debe terminar con ';'.";
}