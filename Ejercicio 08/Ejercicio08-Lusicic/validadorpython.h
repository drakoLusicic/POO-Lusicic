#ifndef VALIDADORPYTHON_H
#define VALIDADORPYTHON_H

#include "validadorsintaxis.h"

class ValidadorPython : public ValidadorSintaxis {
public:
    bool validar(const QString &linea) override;
    QString mensajeError() const override;
};

#endif // VALIDADORPYTHON_H