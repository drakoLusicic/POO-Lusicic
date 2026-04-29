#include "login.h"
#include "ui_login.h"
#include "ventana.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrl>
#include <QUrlQuery>

static const QString kDefaultBackgroundUrl = QStringLiteral("https://images.unsplash.com/photo-1518770660439-4636190af475?auto=format&fit=crop&w=1200&q=80");

Login::Login(QWidget *parent)
    : Pantalla(parent),
      ui(new Ui::Login),
      clima(new Clima(this)),
      networkManager(new QNetworkAccessManager(this)),
      backgroundReply(nullptr),
      failedAttempts(0),
      bloqueado(false),
      pendingLogin(false),
      imageReady(false)
{
    ui->setupUi(this);
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    ui->statusLabel->setText("Bienvenido a Dark Tech");

    connect(ui->loginButton, &QPushButton::clicked, this, &Login::on_loginButton_clicked);
    connect(&lockTimer, &QTimer::timeout, this, &Login::onLoginLockTimeout);
    connect(clima, &Clima::climaListo, this, &Login::onWeatherReady);
    connect(clima, &Clima::errorClima, this, &Login::onWeatherError);
}

Login::~Login()
{
    delete ui;
}

void Login::inicializar()
{
    aplicarEstiloDark();
    cargarConfiguracion();
    aplicarProxy();
    ui->usernameEdit->setText("admin");
    ui->passwordEdit->clear();
    ui->timeLabel->setText(formatoHoraLocal());
    ui->weatherLabel->setText("Cargando clima...");
    ui->statusLabel->setText("Conexión segura Dark Tech");

    if (puedeConectar()) {
        clima->consultar(city, apiKey, proxy, true);
    } else {
        clima->consultar(city, apiKey, proxy, false);
        mostrarEstado("Modo offline activado. Se muestran datos simulados.", false);
    }

    cargarFondoCache();
    if (!imageReady) {
        iniciarDescargaFondo();
    }
}

void Login::cargarConfiguracion()
{
    const QString configPath = qApp->applicationDirPath() + "/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    apiKey = settings.value("api/key").toString().trimmed();
    city = settings.value("api/city", "Córdoba").toString().trimmed();
    backgroundUrl = settings.value("ui/background_url", kDefaultBackgroundUrl).toString().trimmed();
    profilePhotoPath = settings.value("ui/profile_photo_path").toString().trimmed();
    if (!profilePhotoPath.isEmpty() && !QDir(profilePhotoPath).isAbsolute()) {
        profilePhotoPath = qApp->applicationDirPath() + QDir::separator() + profilePhotoPath;
    }
    const bool proxyEnabled = settings.value("proxy/enabled", false).toBool();
    if (proxyEnabled) {
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(settings.value("Proxy/host", "127.0.0.1").toString());
        proxy.setPort(settings.value("Proxy/port", 8080).toInt());
        proxy.setUser(settings.value("Proxy/username").toString());
        proxy.setPassword(settings.value("Proxy/password").toString());
    } else {
        proxy = QNetworkProxy::NoProxy;
    }
}

void Login::aplicarProxy()
{
    networkManager->setProxy(proxy);
}

bool Login::puedeConectar() const
{
    // Qt6 puede tener distintos métodos de disponibilidad, así que dejamos
    // el proceso de fallback al código de red en caso de error.
    return true;
}

QString Login::cacheFondoRuta() const
{
    const QString cacheDir = appDataPath();
    QDir().mkpath(cacheDir);
    return cacheDir + QDir::separator() + "darktech_fondo.jpg";
}

void Login::cargarFondoCache()
{
    const QString ruta = cacheFondoRuta();
    if (QFile::exists(ruta)) {
        if (backgroundPixmap.load(ruta)) {
            imageReady = true;
            ui->imagePreviewLabel->setPixmap(backgroundPixmap.scaled(ui->imagePreviewLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            registrarEvento("Fondo cargado desde caché local.");
        }
    }
}

void Login::iniciarDescargaFondo()
{
    if (backgroundUrl.isEmpty()) {
        backgroundUrl = kDefaultBackgroundUrl;
    }
    QNetworkRequest request{QUrl(backgroundUrl)};
    request.setHeader(QNetworkRequest::UserAgentHeader, "DarkTechBackground/1.0");
    if (backgroundReply) {
        backgroundReply->abort();
        backgroundReply->deleteLater();
    }
    backgroundReply = networkManager->get(request);
    connect(backgroundReply, &QNetworkReply::finished, this, &Login::onBackgroundDownloadFinished);
}

void Login::onBackgroundDownloadFinished()
{
    if (!backgroundReply) {
        mostrarEstado("No se pudo descargar la imagen principal.", true);
        return;
    }

    if (backgroundReply->error() != QNetworkReply::NoError) {
        mostrarEstado("No se puede descargar el fondo. Usando caché o modo offline.", true);
        backgroundReply->deleteLater();
        backgroundReply = nullptr;
        return;
    }

    const QByteArray data = backgroundReply->readAll();
    backgroundReply->deleteLater();
    backgroundReply = nullptr;

    backgroundPixmap.loadFromData(data);
    if (!backgroundPixmap.isNull()) {
        imageReady = true;
        ui->imagePreviewLabel->setPixmap(backgroundPixmap.scaled(ui->imagePreviewLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        QFile archivo(cacheFondoRuta());
        if (archivo.open(QIODevice::WriteOnly)) {
            archivo.write(data);
            archivo.close();
        }
        registrarEvento("Imagen principal descargada y cacheada localmente.");
        if (pendingLogin) {
            abrirVentanaPrincipal();
        }
    } else {
        mostrarEstado("La imagen descargada es inválida.", true);
    }
}

void Login::mostrarEstado(const QString &mensaje, bool error)
{
    ui->statusLabel->setText(mensaje);
    if (error) {
        ui->statusLabel->setStyleSheet("color: #ff8a8a;");
    } else {
        ui->statusLabel->setStyleSheet("color: #b9e1ff;");
    }
}

void Login::onWeatherReady(double temperatura, const QString &descripcion, const QString &horaLocal)
{
    ui->weatherLabel->setText(QStringLiteral("Córdoba: %1 °C · %2").arg(QString::number(temperatura, 'f', 1), descripcion));
    ui->timeLabel->setText(QStringLiteral("Hora local: %1").arg(horaLocal));
    registrarEvento(QStringLiteral("Clima actualizado: %1 °C - %2").arg(QString::number(temperatura, 'f', 1), descripcion));
}

void Login::onWeatherError(const QString &mensaje)
{
    mostrarEstado(mensaje, true);
    registrarEvento(QStringLiteral("Error de clima: %1").arg(mensaje));
}

void Login::on_loginButton_clicked()
{
    if (bloqueado) {
        mostrarEstado("Bloqueado. Espera que termine el período de seguridad.", true);
        return;
    }
    validarCredenciales(ui->usernameEdit->text().trimmed(), ui->passwordEdit->text());
}

void Login::validarCredenciales(const QString &usuario, const QString &clave)
{
    if (usuario == QLatin1String("admin") && clave == QLatin1String("1234")) {
        mostrarEstado("Credenciales válidas. Preparando la ventana principal...");
        registrarEvento("Ingreso exitoso del usuario admin.");
        if (imageReady) {
            abrirVentanaPrincipal();
        } else {
            pendingLogin = true;
        }
        return;
    }

    failedAttempts++;
    registrarEvento(QStringLiteral("Intento de acceso fallido %1.").arg(failedAttempts));
    if (failedAttempts >= 3) {
        bloqueado = true;
        lockTimer.start(30000);
        mostrarEstado("Demasiados intentos fallidos. Bloqueado por 30 segundos.", true);
    } else {
        mostrarEstado(QStringLiteral("Usuario o contraseña incorrectos. %1 intentos restantes.").arg(3 - failedAttempts), true);
    }
}

void Login::onLoginLockTimeout()
{
    bloqueado = false;
    failedAttempts = 0;
    lockTimer.stop();
    mostrarEstado("El bloqueo ha terminado. Intenta nuevamente.");
    registrarEvento("Fin del bloqueo por intentos fallidos.");
}

void Login::abrirVentanaPrincipal()
{
    Ventana *ventana = new Ventana(backgroundPixmap, profilePhotoPath, nullptr);
    ventana->inicializar();
    ventana->showFullScreen();
    hide();
    registrarEvento("Apertura de ventana principal.");
}
