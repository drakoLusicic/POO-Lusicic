#include "pantalla.h"

Pantalla::Pantalla(QWidget *parent) : QWidget(parent) {}

void Pantalla::registrarEventoBase(const QString &evento) {
    QFile file("log.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " - " << evento << "\n";
        file.close();
    }
}