#ifndef MODOBLOQUEADO_H
#define MODOBLOQUEADO_H

#include "pantalla.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QSettings>

class ModoBloqueado : public Pantalla {
    Q_OBJECT
public:
    explicit ModoBloqueado(QWidget *parent = nullptr);

    void inicializarUI() override;
    void conectarEventos() override;
    void cargarDatos() override;
    void validarEstado() override;
    void registrarEvento(const QString &evento) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void actualizarTiempo();

private:
    QLabel *mensajeLabel;
    QLabel *tiempoLabel;
    QTimer *timer;
    int tiempoRestante;
    QSettings settings;
};

#endif // MODOBLOQUEADO_H