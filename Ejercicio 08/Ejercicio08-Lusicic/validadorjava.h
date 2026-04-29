#ifndef VALIDADORJAVA_H
#define VALIDADORJAVA_H

#include "validadorsintaxis.h"

class ValidadorJava : public ValidadorSintaxis {
public:
    bool validar(const QString &linea) override;
    QString mensajeError() const override;
};

#endif // VALIDADORJAVA_H