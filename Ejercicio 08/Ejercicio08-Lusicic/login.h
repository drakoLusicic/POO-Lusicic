#ifndef LOGIN_H
#define LOGIN_H

#include "pantalla.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QSettings>

class Login : public Pantalla {
    Q_OBJECT
public:
    explicit Login(QWidget *parent = nullptr);

    void inicializarUI() override;
    void conectarEventos() override;
    void cargarDatos() override;
    void validarEstado() override;
    void registrarEvento(const QString &evento) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void intentarLogin();
    void desbloquear();

private:
    QLineEdit *usuarioEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QLabel *mensajeLabel;
    QTimer *timerBloqueo;
    int intentosFallidos;
    bool cerrarPorLogin;
    QSettings settings;
};

#endif // LOGIN_H