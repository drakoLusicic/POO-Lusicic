#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Ejercicio09-Lusicic");
    QApplication::setOrganizationName("Lusicic");

    MainWindow mainWindow;
    if (!mainWindow.initializeDatabase()) {
        return 1;
    }

    LoginDialog login(&mainWindow);
    if (login.exec() != QDialog::Accepted) {
        return 0;
    }

    mainWindow.show();
    return app.exec();
}
