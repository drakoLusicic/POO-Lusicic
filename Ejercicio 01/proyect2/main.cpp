#include <QApplication>
#include "mainwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWidget mainWindow;
    mainWindow.show();

    return app.exec();
}