#ifndef PANTALLA_H
#define PANTALLA_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Pantalla : public QWidget {
    Q_OBJECT
public:
    explicit Pantalla(QWidget *parent = nullptr);
    virtual ~Pantalla() = default;

    virtual void inicializarUI() = 0;
    virtual void conectarEventos() = 0;
    virtual void cargarDatos() = 0;
    virtual void validarEstado() = 0;
    virtual void registrarEvento(const QString &evento) = 0;

protected:
    void registrarEventoBase(const QString &evento);
};

#endif // PANTALLA_H