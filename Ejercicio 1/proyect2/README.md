# Ejercicio 01 - Planificador de trabajos practicos

Aplicacion Qt Widgets (sin QML) para planificar trabajos practicos con:

- Login con usuarios locales en JSON.
- Persistencia de sesion local por 5 minutos.
- Tablero en grilla con filtros por estado y prioridad.
- Alta, edicion y eliminacion de trabajos practicos.
- Editor de notas por trabajo con guardado manual.
- Historial visible en la UI y persistido en archivo JSON.

## Requisitos

- Qt 6 (Widgets)
- Compilador MinGW (o equivalente)
- qmake

## Estructura relevante

- `main.cpp`: inicio de la app.
- `mainwidget.*`: contenedor principal (login/tablero).
- `loginwidget.*`: pantalla de login.
- `workboardwidget.*`: tablero, filtros, acciones e historial visible.
- `workdialog.*`: alta/edicion de trabajos.
- `noteeditor.*`: editor de notas.
- `workmanager.*`: modelo y persistencia de trabajos (`data/works.json`).
- `authmanager.*`: autenticacion y sesion (`data/users.json`, `data/session.json`).
- `historymanager.*`: historial (`data/history.json`).

## Compilacion (qmake)

Desde la carpeta del proyecto:

```bash
qmake proyect2.pro
mingw32-make
```

## Ejecucion

```bash
./proyect2.exe
```

Si compilas en carpeta `build`, ejecuta el binario generado desde esa salida de compilacion.

## Datos locales

Los archivos JSON se guardan en la carpeta `data/` junto al ejecutable:

- `data/users.json`
- `data/session.json`
- `data/works.json`
- `data/history.json`

Usuarios de prueba por defecto:

- `admin / admin`
- `estudiante1 / password123`

## Para la entrega

Agregar capturas de:

- Login
- Tablero con filtros
- Editor de notas con estado guardado
- Historial visible
