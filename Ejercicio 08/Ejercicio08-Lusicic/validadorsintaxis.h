#ifndef VALIDADORSINTAXIS_H
#define VALIDADORSINTAXIS_H

#include <QString>

class ValidadorSintaxis {
public:
    virtual ~ValidadorSintaxis() = default;
    virtual bool validar(const QString &linea) = 0;
    virtual QString mensajeError() const = 0;
};

#endif // VALIDADORSINTAXIS_H