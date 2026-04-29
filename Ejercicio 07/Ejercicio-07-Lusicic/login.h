#ifndef LOGIN_H
#define LOGIN_H

#include "pantalla.h"
#include "clima.h"

#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QTimer>

namespace Ui { class Login; }

class Ventana;

class Login : public Pantalla
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login() override;
    void inicializar() override;

private slots:
    void on_loginButton_clicked();
    void onBackgroundDownloadFinished();
    void onLoginLockTimeout();
    void onWeatherReady(double temperatura, const QString &descripcion, const QString &horaLocal);
    void onWeatherError(const QString &mensaje);

private:
    void cargarConfiguracion();
    void aplicarProxy();
    void iniciarDescargaFondo();
    void mostrarEstado(const QString &mensaje, bool error = false);
    void validarCredenciales(const QString &usuario, const QString &clave);
    void abrirVentanaPrincipal();
    void cargarFondoCache();
    QString cacheFondoRuta() const;
    bool puedeConectar() const;

    Ui::Login *ui;
    Clima *clima;
    QNetworkAccessManager *networkManager;
    QNetworkReply *backgroundReply;
    QTimer lockTimer;
    int failedAttempts;
    bool bloqueado;
    bool pendingLogin;
    QString apiKey;
    QString city;
    QString backgroundUrl;
    QString profilePhotoPath;
    QNetworkProxy proxy;
    QPixmap backgroundPixmap;
    bool imageReady;
};

#endif // LOGIN_H
