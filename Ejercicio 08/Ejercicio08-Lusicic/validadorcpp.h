#ifndef VALIDADORCPP_H
#define VALIDADORCPP_H

#include "validadorsintaxis.h"

class ValidadorCpp : public ValidadorSintaxis {
public:
    bool validar(const QString &linea) override;
    QString mensajeError() const override;
};

#endif // VALIDADORCPP_H