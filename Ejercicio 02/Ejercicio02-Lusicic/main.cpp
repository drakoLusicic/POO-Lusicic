#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("VPS Monitor");
    app.setApplicationDisplayName("Panel de Monitoreo VPS");
    app.setOrganizationName("POO-Informatica");

    MainWindow w;
    w.show();

    return app.exec();
}
