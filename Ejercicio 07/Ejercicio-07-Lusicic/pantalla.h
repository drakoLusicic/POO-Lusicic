#ifndef PANTALLA_H
#define PANTALLA_H

#include <QWidget>

class Pantalla : public QWidget
{
    Q_OBJECT
public:
    explicit Pantalla(QWidget *parent = nullptr);
    virtual void inicializar() = 0;
    void aplicarEstiloDark();
    void registrarEvento(const QString &texto) const;
    QString formatoHoraLocal() const;
    QString appDataPath() const;
};

#endif // PANTALLA_H
