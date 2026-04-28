#include "login.h"

Login::Login() { // Constructor de la clase Login, se encarga de inicializar los controles y configurar la interfaz de usuario
    lUser = new QLabel("Username"); // Crea una etiqueta para el nombre de usuario
    lPass = new QLabel("Password");  // Crea una etiqueta para la contraseña
    leUser = new QLineEdit; // Crea un campo de entrada de texto para el nombre de usuario
    lePass = new QLineEdit; // Crea un campo de entrada de texto para la contraseña
    lePass->setEchoMode(QLineEdit::Password); // oculta los caracteres ingresados en la contrasena
    bLogin = new QPushButton("Login"); // Crea un botón de inicio de sesión
    layout.addWidget(lUser, 1,1,1,1);  // Agrega la etiqueta de nombre de usuario al diseño en la posición (1,1) y ocupa 1 fila y 1 columna
    layout.addWidget(leUser, 1,2,1,2); // Agrega el campo de entrada de nombre de usuario al diseño en la posición (1,2) y ocupa 1 fila y 2 columnas
    layout.addWidget(lPass, 2,1,1,1); // Agrega la etiqueta de contraseña al diseño en la posición (2,1) y ocupa 1 fila y 1 columna
    layout.addWidget(lePass, 2,2,1,2); // Agrega el campo de entrada de contraseña al diseño en la posición (2,2) y ocupa 1 fila y 2 columnas
    layout.addWidget(bLogin, 3,2,1,1); // Agrega el botón de inicio de sesión al diseño en la posición (3,2) y ocupa 1 fila y 1 columna
    this->setLayout(&layout); // Establece el diseño de la ventana para organizar los controles
    connect(bLogin, SIGNAL(pressed()), this, SLOT(loginClicked())); //cuando se presiona se llama a loginclicked, que cierra la ventana de login
}
