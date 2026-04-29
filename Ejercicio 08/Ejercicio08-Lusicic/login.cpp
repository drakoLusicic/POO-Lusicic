#include "login.h"
#include "editorprincipal.h"
#include "modobloqueado.h"
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMessageBox>

Login::Login(QWidget *parent) : Pantalla(parent), intentosFallidos(0), cerrarPorLogin(false), settings("config.ini", QSettings::IniFormat) {
    inicializarUI();
    conectarEventos();
    cargarDatos();
}

void Login::inicializarUI() {
    setWindowTitle("Login - Editor Multilenguaje");
    setFixedSize(420, 340);
    setStyleSheet("background: #12151B; border: 1px solid #1F232D; border-radius: 28px;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(18);
    layout->setContentsMargins(26, 26, 26, 26);

    QLabel *titulo = new QLabel("Iniciar Sesión");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 24px; color: #E6E8F0; font-weight: 700;");

    usuarioEdit = new QLineEdit();
    usuarioEdit->setPlaceholderText("Usuario");

    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Contraseña");

    loginButton = new QPushButton("Login");
    loginButton->setStyleSheet("background: #1F2430; border: 1px solid #3E3F4A; border-radius: 20px; color: #E6E8F0; padding: 12px; font-size: 14px; font-weight: 700;");

    mensajeLabel = new QLabel("");
    mensajeLabel->setAlignment(Qt::AlignCenter);
    mensajeLabel->setStyleSheet("color: #F6C8FF; font-weight: 600; background: rgba(126, 94, 255, 0.14); border-radius: 14px; padding: 10px;");

    layout->addWidget(titulo);
    layout->addWidget(usuarioEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(mensajeLabel);

    setLayout(layout);
}

void Login::conectarEventos() {
    connect(loginButton, &QPushButton::clicked, this, &Login::intentarLogin);
}

void Login::cargarDatos() {
    // Datos ya en settings
}

void Login::validarEstado() {
    // No aplica
}

void Login::registrarEvento(const QString &evento) {
    registrarEventoBase("Login: " + evento);
}

void Login::intentarLogin() {
    QString usuario = usuarioEdit->text();
    QString password = passwordEdit->text();

    if (usuario == settings.value("usuario").toString() && password == settings.value("password").toString()) {
        cerrarPorLogin = true;
        registrarEvento("Login exitoso");
        EditorPrincipal *editor = new EditorPrincipal();
        editor->showFullScreen();
        this->close();
    } else {
        intentosFallidos++;
        mensajeLabel->setText("Credenciales incorrectas. Intentos: " + QString::number(intentosFallidos));
        registrarEvento("Intento fallido: " + usuario);
        if (intentosFallidos >= 3) {
            cerrarPorLogin = true;
            ModoBloqueado *bloqueado = new ModoBloqueado();
            bloqueado->show();
            this->close();
        }
    }
}

void Login::desbloquear() {
    intentosFallidos = 0;
    mensajeLabel->setText("");
}

void Login::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        intentarLogin();
    }
    QWidget::keyPressEvent(event);
}

void Login::closeEvent(QCloseEvent *event) {
    if (cerrarPorLogin) {
        event->accept();
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Salir", "¿Seguro que quieres salir?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}