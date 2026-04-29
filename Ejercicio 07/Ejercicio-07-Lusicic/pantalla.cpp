#include "pantalla.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

Pantalla::Pantalla(QWidget *parent)
    : QWidget(parent)
{
    aplicarEstiloDark();
}

void Pantalla::aplicarEstiloDark()
{
    const QString estilo = R"(
        QWidget {
            background-color: #0f1116;
            color: #c7d1ff;
            font-family: "Segoe UI", "Roboto", "Arial", sans-serif;
            font-size: 10pt;
        }
        QLabel#titleLabel {
            color: #87c6ff;
            font-size: 20pt;
            font-weight: bold;
        }
        QLabel#subtitleLabel, QLabel#weatherLabel, QLabel#timeLabel {
            color: #c7d1ff;
        }
        QPushButton {
            border: 2px solid #3a4a6b;
            border-radius: 10px;
            background-color: #141a26;
            padding: 8px 14px;
            color: #e5ecff;
        }
        QPushButton:hover {
            background-color: #1c2550;
            border-color: #58a6ff;
        }
        QPushButton:pressed {
            background-color: #0b1123;
        }
        QLineEdit, QTextEdit {
            background-color: #161b24;
            border: 1px solid #394662;
            border-radius: 8px;
            padding: 6px;
            color: #eef2ff;
        }
        QGroupBox {
            border: 1px solid #2a3b5a;
            border-radius: 12px;
            margin-top: 12px;
        }
        QGroupBox:title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 8px;
            color: #7a9cff;
        }
    )";
    setStyleSheet(estilo);
}

void Pantalla::registrarEvento(const QString &texto) const
{
    const QString logDirectory = appDataPath();
    QDir().mkpath(logDirectory);
    const QString nombreArchivo = logDirectory + QDir::separator() + "darktech_log.txt";
    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::Append | QIODevice::Text)) {
        return;
    }
    QTextStream stream(&archivo);
    stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
           << " - " << texto << "\n";
    archivo.close();
}

QString Pantalla::formatoHoraLocal() const
{
    return QDateTime::currentDateTime().toString("hh:mm:ss dd/MM/yyyy");
}

QString Pantalla::appDataPath() const
{
    const QString local = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (local.isEmpty()) {
        return QDir::homePath() + QDir::separator() + ".darktech";
    }
    return local;
}
