#include <QApplication>
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("Fusion");

    Login login;
    login.inicializar();
    login.show();

    return app.exec();
}
