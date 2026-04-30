# 🧠 Editor Multilenguaje Qt — Ejercicio 08

Un editor de código en Qt con login seguro, validación básica de sintaxis, modo bloqueado tras 3 intentos fallidos y exportación de código como imagen JPG.

---

## 🚀 Descripción general

Esta aplicación es una interfaz de edición de código pensada para practicar:

- Login con usuario y contraseña guardados en `config.ini`
- Editor con selección de lenguaje: `C++`, `Python` o `Java`
- Validación simple de la línea actual según el lenguaje elegido
- Exportación de la vista del editor a un archivo `JPG`
- Modo bloqueado tras 3 intentos fallidos de login
- Registro de eventos en `log.txt`

---

## 🧩 Estructura del proyecto

- `main.cpp` — inicializa la aplicación, carga settings y abre la ventana de login
- `login.cpp / login.h` — pantalla de autenticación y control de intentos fallidos
- `editorprincipal.cpp / editorprincipal.h` — ventana principal del editor con UI, validación y exportación
- `modobloqueado.cpp / modobloqueado.h` — pantalla de espera cuando el login falla 3 veces
- `pantalla.cpp / pantalla.h` — clase base para registrar eventos en `log.txt`
- `validadorcpp.cpp / validadorcpp.h` — validación básica para C++
- `validadorpython.cpp / validadorpython.h` — validador simple para Python
- `validadorjava.cpp / validadorjava.h` — validación básica para Java
- `syntaxhighlighter.cpp / syntaxhighlighter.h` — resaltado de sintaxis para el editor
- `resources.qrc` — recursos Qt (como imágenes usadas en la interfaz)

---

## 🔐 Flujo de uso

1. Al iniciar la aplicación se muestra la ventana de `Login`
2. El usuario debe ingresar credenciales
   - Usuario por defecto: `admin`
   - Contraseña por defecto: `1234`
3. Si el login es exitoso se abre el editor en pantalla completa
4. Si el login falla 3 veces, se activa el `Modo Bloqueado`
   - Bloqueo temporal configurado en `config.ini` (`tiempo_bloqueo`)
   - Tras el tiempo se vuelve a mostrar el login

---

## 🧠 Funciones del editor

- `selectorLenguaje`: elige entre `C++`, `Python` o `Java`
- `editorTexto`: area de texto para escribir código
- `errorLabel`: muestra mensajes de error básico en tiempo real
- `Exportar a JPG`: guarda la vista del editor como imagen en la ruta configurada

### Validación de línea actual

- `C++` — acepta líneas que terminen en `;`, `{`, `}`, directivas `#`, o comentarios
- `Java` — acepta líneas que terminen en `;` o que contengan `{`
- `Python` — ejecuta validación muy simple (actualmente siempre permite la línea)

> Nota: la validación está pensada como un chequeo rápido de sintaxis, no como un compilador completo.

---

## ⚙️ Configuración de `config.ini`

Al iniciar por primera vez, `main.cpp` crea `config.ini` con estos valores por defecto:

- `usuario = admin`
- `password = 1234`
- `tiempo_bloqueo = 15000` (milisegundos)
- `lenguaje_defecto = C++`
- `ruta_exportacion = <home>/codigo.jpg`

Puedes modificar `config.ini` manualmente para cambiar:

- credenciales
- tiempo de bloqueo
- lenguaje inicial
- ruta de exportación JPG

---

## 📦 Archivos importantes

- `Ejercicio08-Lusicic.pro` — archivo del proyecto Qt
- `resources.qrc` — recursos usados en la UI
- `log.txt` — archivo generado que registra acciones del usuario
- `config.ini` — configuración de usuario y opciones de la aplicación

---

## 🛠️ Compilación y ejecución

1. Abrir `Ejercicio08-Lusicic.pro` en Qt Creator
2. Seleccionar el kit adecuado (por ejemplo Qt 6.10.2 MinGW 64-bit)
3. Compilar y ejecutar

> Si usas la carpeta `build/`, revisa que el `.pro` esté correctamente enlazado y que los recursos estén incluidos.

---

## 💡 Extensiones posibles

- Agregar guardado/auto-guardado de archivos de código
- Mejorar validación de Python y Java
- Añadir resaltado de sintaxis completo en los tres lenguajes
- Permitir exportar a PNG o PDF
- Incluir un menú de configuración dentro de la app

---

## 👍 Resumen

Este proyecto es un editor Qt con:

- login seguro
- interfaz moderna
- selección de lenguaje
- validación en tiempo real
- exportación a imagen
- modo bloqueo por seguridad

¡Ideal para practicar POO, Qt y GUI con C++!
