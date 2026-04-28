#ifndef LOGIN_H
#define LOGIN_H
#include <QWidget> // ventana de bases y controles de qt
#include <QLabel> // Etiquetas de texto para mostrar informacion
#include <QLineEdit> // Controles de entrada de texto para que el usuario ingrese datos
#include <QPushButton> // Botones para que el usuario interactue con la interfaz
#include <QGridLayout> // Diseño de cuadrícula para organizar los controles en la ventana
class Login : public QWidget{ //clase base para ventanas y controles de qt
    Q_OBJECT // Permite el uso de señales y ranuras en la clase, lo que es fundamental para la comunicación entre objetos en Qt
private:
    QLabel *lUser, *lPass; // Muestra 'username' y 'password'
    QLineEdit *leUser, *lePass; // Campo de entrada de username y password
    QPushButton *bLogin; // boton de login
    QGridLayout layout; // cuadricula para centrar los objetos
public:
    Login(); //constructor
private slots:
    void loginClicked(){
        this->close(); // Cierra la ventana de login al hacer clic en el botón de login
    }
};


#endif // LOGIN_H
