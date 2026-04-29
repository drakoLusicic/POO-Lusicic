#include <QApplication>
#include <QDir>
#include <QSettings>
#include "login.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Configuración global
    QSettings settings("config.ini", QSettings::IniFormat);
    if (!settings.contains("usuario")) {
        settings.setValue("usuario", "admin");
        settings.setValue("password", "1234");
        settings.setValue("tiempo_bloqueo", 15000); // 15 segundos
        settings.setValue("lenguaje_defecto", "C++");
        settings.setValue("ruta_exportacion", QDir::homePath() + "/codigo.jpg");
    }

    // Estilo dark dashboard suave
    app.setStyleSheet(R"(
    QWidget { background: #121721; color: #D9E1FF; font-family: 'Segoe UI', sans-serif; }
    QLineEdit { background: #181F2C; border: 1px solid #2D3648; border-radius: 18px; color: #D9E1FF; padding: 12px; font-size: 14px; }
    QLineEdit:focus { border: 1px solid #8B97FF; }
    QPushButton { background: #232B3B; color: #F2F6FF; border: 1px solid #3A4461; border-radius: 20px; padding: 12px 22px; font-weight: 600; font-size: 14px; }
    QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #7C8FFF, stop:1 #A0B3FF); border-color: #8B97FF; }
    QComboBox { background: #181F2C; border: 1px solid #2D3648; border-radius: 18px; color: #D9E1FF; padding: 10px; font-size: 14px; }
    QComboBox::drop-down { border: none; }
    QComboBox QAbstractItemView { background: #181F2C; border: 1px solid #2D3648; color: #D9E1FF; selection-background-color: #7C8FFF; selection-color: #FFFFFF; }
    QTextEdit, QPlainTextEdit { background: #161D2C; border: 1px solid #2D3648; border-radius: 18px; color: #D9E1FF; padding: 14px; font-size: 14px; }
    QLabel { color: #D9E1FF; }
    QLabel#sectionTitle { font-size: 20px; font-weight: 700; }
    QLabel#subTitle { color: #A9B9EB; font-size: 13px; }
    QSplitter::handle { background: transparent; }
    QSplitter::handle:horizontal { width: 14px; }
    )") ;

    Login login;
    login.show();

    return app.exec();
}