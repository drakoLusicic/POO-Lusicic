#include "modobloqueado.h"
#include "login.h"
#include <QKeyEvent>
#include <QCloseEvent>
#include <QApplication>

ModoBloqueado::ModoBloqueado(QWidget *parent) : Pantalla(parent), tiempoRestante(30), settings("config.ini", QSettings::IniFormat) {
    tiempoRestante = settings.value("tiempo_bloqueo", 30000).toInt() / 1000;
    inicializarUI();
    conectarEventos();
    cargarDatos();
    timer->start(1000);
}

void ModoBloqueado::inicializarUI() {
    setWindowTitle("Bloqueado");
    setFixedSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);

    mensajeLabel = new QLabel("Sistema bloqueado por intentos fallidos.");
    mensajeLabel->setAlignment(Qt::AlignCenter);
    mensajeLabel->setStyleSheet("font-size: 18px; color: #FFB6C1; background: rgba(255, 182, 193, 0.1); border-radius: 15px; padding: 10px;");

    tiempoLabel = new QLabel("Tiempo restante: " + QString::number(tiempoRestante) + " segundos");
    tiempoLabel->setAlignment(Qt::AlignCenter);
    tiempoLabel->setStyleSheet("color: #E0E0E0; font-size: 14px;");

    layout->addWidget(mensajeLabel);
    layout->addWidget(tiempoLabel);

    setLayout(layout);
}

void ModoBloqueado::conectarEventos() {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ModoBloqueado::actualizarTiempo);
}

void ModoBloqueado::cargarDatos() {
    // No aplica
}

void ModoBloqueado::validarEstado() {
    // No aplica
}

void ModoBloqueado::registrarEvento(const QString &evento) {
    registrarEventoBase("Bloqueado: " + evento);
}

void ModoBloqueado::actualizarTiempo() {
    tiempoRestante--;
    tiempoLabel->setText("Tiempo restante: " + QString::number(tiempoRestante) + " segundos");
    if (tiempoRestante <= 0) {
        timer->stop();
        Login *login = new Login();
        login->show();
        this->close();
    }
}

void ModoBloqueado::keyPressEvent(QKeyEvent *event) {
    // Bloquear entrada
    event->ignore();
}

void ModoBloqueado::closeEvent(QCloseEvent *event) {
    event->ignore(); // No permitir cerrar
}