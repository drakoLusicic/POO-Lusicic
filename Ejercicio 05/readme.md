# 🔐 Ejercicio 05 — Login en Qt (C++)

## 📅 Fecha

Abril 2026

## 🎯 Objetivo

Desarrollar una **interfaz de login básica en Qt** utilizando C++, aplicando:

* Creación de ventanas (`QWidget`)
* Uso de controles gráficos (labels, inputs, botones)
* Layouts (organización visual)
* Sistema de **señales y slots**
* Manejo de eventos de usuario

---

# 🧱 1. Estructura del proyecto

El proyecto está compuesto por los siguientes archivos:

* `main.cpp` → punto de entrada de la aplicación
* `login.h` → definición de la clase Login
* `login.cpp` → implementación de la interfaz
* `clase1-4-26.pro` → archivo de configuración del proyecto Qt

---

# ⚙️ 2. Configuración del proyecto (.pro)

El archivo `.pro` define los módulos y archivos utilizados:

```pro
QT += core gui widgets

HEADERS += \
    login.h

SOURCES += \
    login.cpp \
    main.cpp
```

📌 Se incluye el módulo `widgets`, necesario para interfaces gráficas.

---

# 🚀 3. Punto de entrada (main.cpp)

```cpp
#include <QApplication>
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();
    return a.exec();
}
```

### 🧠 Qué hace:

* Inicializa la aplicación Qt
* Instancia la ventana `Login`
* La muestra en pantalla
* Ejecuta el loop de eventos

---

# 🧩 4. Definición de la clase (login.h)

```cpp
class Login : public QWidget {
    Q_OBJECT

private:
    QLabel *lUser, *lPass;
    QLineEdit *leUser, *lePass;
    QPushButton *bLogin;
    QGridLayout layout;

public:
    Login();

private slots:
    void loginClicked();
};
```

### 📌 Componentes utilizados:

* `QLabel` → etiquetas de texto
* `QLineEdit` → inputs de usuario
* `QPushButton` → botón de acción
* `QGridLayout` → organización en grilla

### 🔑 `Q_OBJECT`

Permite usar el sistema de **señales y slots** de Qt.

---

# 🎨 5. Implementación de la interfaz (login.cpp)

## 🏗️ Constructor

```cpp
Login::Login() {
    lUser = new QLabel("Username");
    lPass = new QLabel("Password");

    leUser = new QLineEdit;
    lePass = new QLineEdit;
    lePass->setEchoMode(QLineEdit::Password);

    bLogin = new QPushButton("Login");
```

### 🧠 Detalles clave:

* Se crean los controles dinámicamente
* `setEchoMode(QLineEdit::Password)` oculta la contraseña

---

## 🧱 Layout (organización visual)

```cpp
layout.addWidget(lUser, 1,1,1,1);
layout.addWidget(leUser, 1,2,1,2);
layout.addWidget(lPass, 2,1,1,1);
layout.addWidget(lePass, 2,2,1,2);
layout.addWidget(bLogin, 3,2,1,1);

this->setLayout(&layout);
```

📌 Se utiliza un **GridLayout** para ordenar:

```
Username  [ input usuario ]
Password  [ input password ]
           [   botón      ]
```

---

## 🔗 Conexión de evento (señal-slot)

```cpp
connect(bLogin, SIGNAL(pressed()), this, SLOT(loginClicked()));
```

### 🧠 Qué significa:

* Cuando el botón es presionado (`pressed`)
* Se ejecuta el método `loginClicked()`

---

## ⚡ Slot: acción del login

```cpp
void loginClicked(){
    this->close();
}
```

👉 Acción actual:

* Cierra la ventana al presionar "Login"

---

# 🔄 6. Flujo de ejecución

```text
Usuario abre app
      ↓
Se muestra ventana Login
      ↓
Usuario ingresa datos
      ↓
Presiona botón Login
      ↓
Se ejecuta loginClicked()
      ↓
Se cierra la ventana
```

---

# ⚠️ Limitaciones actuales

Este login es **visual (UI)**, pero:

* ❌ No valida usuario/contraseña
* ❌ No hay backend
* ❌ No hay persistencia
* ❌ No hay manejo de errores

---

# 🚀 Posibles mejoras

Para evolucionarlo a un sistema real:

### 🔐 Validación básica

```cpp
if(leUser->text() == "admin" && lePass->text() == "1234")
```

---

### 📢 Mensajes al usuario

* `QMessageBox` para errores o éxito

---

### 🧠 Integración real

* Base de datos (SQLite / MySQL)
* API REST
* Tokens de sesión

---

### 🔒 Seguridad

* Hash de contraseñas
* No guardar texto plano

---

# 🎯 Conceptos aplicados

* Programación orientada a objetos en Qt
* Interfaces gráficas (GUI)
* Gestión de eventos
* Señales y slots
* Layouts dinámicos

---

# 📌 Resultado final

Se desarrolló una:

* ✔ Ventana de login funcional
* ✔ Interfaz clara y estructurada
* ✔ Manejo de eventos con Qt
* ✔ Base sólida para sistemas más complejos

---

# 🧠 Concepto clave

El login implementado es:

👉 **una interfaz desacoplada de la lógica de autenticación**

Esto permite escalar el sistema sin modificar la UI.

---

# 📊 Estado del ejercicio

✅ Compila
✅ Ejecuta correctamente
⚠️ Lógica de autenticación básica (expandible)

---

**FIN**

