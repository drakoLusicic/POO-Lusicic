#include "validadorcpp.h"

bool ValidadorCpp::validar(const QString &linea) {
    QString trimmed = linea.trimmed();
    if (trimmed.isEmpty()) return true;
    if (trimmed.startsWith("#") || trimmed.endsWith(";") || trimmed.endsWith("{") || trimmed.endsWith("}")) return true;
    // Para líneas que no terminan con ;, {, }, o directivas, asumir error si no es comentario o similar
    if (trimmed.contains("//") || trimmed.startsWith("/*")) return true;
    return false;
}

QString ValidadorCpp::mensajeError() const {
    return "Línea inválida: debe terminar con ';' o ser una directiva.";
}