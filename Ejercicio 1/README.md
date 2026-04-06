# 📋 Ejercicio 01 - Planificador de Trabajos Prácticos

## 🎯 Descripción del Proyecto

Aplicación desktop desarrollada en **Qt 6 (Widgets)** que permite planificar, gestionar y hacer seguimiento de trabajos prácticos. Incluye autenticación de usuarios, persistencia local, filtrado avanzado y un historial completo de acciones.

### ✨ Características Principales

- 🔐 **Login con validación** - Autenticación de usuarios contra base de datos local (JSON)
- 💾 **Persistencia de sesión** - Recordar sesión por 5 minutos (sin pedir login nuevamente)
- 📊 **Tablero dinámico** - Vista en grilla (QGridLayout) con filtros por estado y prioridad
- ➕ **CRUD completo** - Alta, edición y eliminación de trabajos prácticos
- 📝 **Editor de notas** - Notas asociadas por trabajo con guardado manual
- 📜 **Historial visible** - Registro de todas las acciones guardado en archivo JSON
- 🎨 **Interfaz moderna** - Diseño con colores, bordes redondeados y feedback visual

---

## 🏗️ Arquitectura y Organización del Código

### Estructura de Carpetas

```
Ejercicio 1/
├── README.md
└── proyect2/
    ├── proyect2.pro              # Archivo de configuración de Qt
    ├── main.cpp                  # Punto de entrada de la aplicación
    ├── data/                     # Carpeta de datos persistentes (se crea automáticamente)
    │   ├── users.json            # Base de datos de usuarios
    │   ├── session.json          # Sesión actual del usuario
    │   ├── works.json            # Trabajos prácticos almacenados
    │   └── history.json          # Historial de acciones
    │
    ├── mainwidget.h/cpp          # Contenedor principal (maneja login/tablero)
    ├── loginwidget.h/cpp         # Pantalla de login con validación
    ├── workboardwidget.h/cpp     # Tablero principal con filtros e historial
    ├── workdialog.h/cpp          # Diálogo para crear/editar trabajos
    ├── noteeditor.h/cpp          # Editor de notas con guardado
    │
    ├── authmanager.h/cpp         # Gestor de autenticación y sesiones
    ├── workmanager.h/cpp         # Gestor de trabajos (CRUD + persistencia)
    ├── historymanager.h/cpp      # Gestor de historial de acciones
    │
    └── widget.h/cpp/ui           # Widget generado por Qt Designer
```

---

## 📚 Componentes Principales

### 1️⃣ **AuthManager** - Gestión de Autenticación
**Archivos:** `authmanager.h` / `authmanager.cpp`

**Responsabilidades:**
- Cargar y validar usuarios contra `data/users.json`
- Crear nuevos usuarios
- Gestionar sesiones con expiración de 5 minutos
- Crear usuarios por defecto si no existen

**Usuarios de prueba:**
```
Usuario: admin       | Contraseña: admin
Usuario: estudiante1 | Contraseña: password123
```

**Métodos principales:**
```cpp
bool authenticate(const QString& username, const QString& password);  // Valida credenciales
void saveSession(const QString& username);                             // Crea sesión
QString getActiveSession();                                            // Obtiene usuario en sesión activa
void logout();                                                         // Cierra sesión
```

---

### 2️⃣ **WorkManager** - Gestión de Trabajos
**Archivos:** `workmanager.h` / `workmanager.cpp`

**Responsabilidades:**
- CRUD completo de trabajos prácticos
- Persistencia en `data/works.json`
- Manejo de notas por trabajo
- Generación automática de IDs únicos (UUID)

**Estructura de Trabajo (Work):**
```cpp
struct Work {
    QString id;                  // ID único (UUID)
    QString title;               // Título del trabajo
    QString description;         // Descripción detallada
    QString status;              // Estado: "Pendiente", "En Progreso", "Completado", "Entregado"
    QString priority;            // Prioridad: "Baja", "Normal", "Alta"
    QDateTime dueDate;           // Fecha de entrega
    QDateTime createdDate;       // Fecha de creación
    QString createdBy;           // Usuario que lo creó
    QString notes;               // Notas asociadas
};
```

**Métodos principales:**
```cpp
QString addWork(...);                           // Crea nuevo trabajo y retorna su ID
bool updateWork(const QString& id, const Work& updatedWork);  // Actualiza trabajo
bool deleteWork(const QString& id);             // Elimina trabajo
bool updateWorkNotes(const QString& id, const QString& notes); // Actualiza solo notas
QList<Work> getAllWorks() const;                // Obtiene todos los trabajos
```

---

### 3️⃣ **HistoryManager** - Gestión de Historial
**Archivos:** `historymanager.h` / `historymanager.cpp`

**Responsabilidades:**
- Registrar todas las acciones del usuario
- Persistencia en `data/history.json`
- Filtrado por usuario, tipo de acción o trabajo

**Estructura de Entrada (HistoryEntry):**
```cpp
struct HistoryEntry {
    QDateTime timestamp;        // Momento de la acción
    QString username;           // Usuario que realizó la acción
    QString actionType;         // Tipo: "Crear", "Editar", "Eliminar", "Notas", etc.
    QString description;        // Descripción de la acción
    QString workId;             // ID del trabajo afectado (opcional)
};
```

**Métodos principales:**
```cpp
void addEntry(const QString& username, const QString& actionType,
              const QString& description, const QString& workId);  // Registra acción
QList<HistoryEntry> getHistory() const;        // Obtiene todo el historial
QList<HistoryEntry> getHistoryByUser(const QString& username) const;
QList<HistoryEntry> getHistoryByActionType(const QString& actionType) const;
```

---

### 4️⃣ **LoginWidget** - Pantalla de Login
**Archivos:** `loginwidget.h` / `loginwidget.cpp`

**Responsabilidades:**
- Interfaz de autenticación moderna
- Validación de campos vacíos
- Captura de presionar Enter en contraseña
- Gestión de mensajes de error

**Flujo:**
1. Usuario ingresa credenciales
2. Se valida contra `AuthManager`
3. Si es correcto, se guarda sesión y se emite signal `loginSuccess`
4. Si falla, se muestra mensaje de error

**Styling:** Diseño moderno con colores Indigo (#6366F1) y Slate

---

### 5️⃣ **WorkBoardWidget** - Tablero Principal
**Archivos:** `workboardwidget.h` / `workboardwidget.cpp`

**Responsabilidades:**
- Mostrar trabajos en grilla dinámica (QGridLayout)
- Aplicar filtros por estado y prioridad
- Botones de acción: Editar, Eliminar, Ver Notas, Cambiar Estado
- Mostrar historial visible en QTextEdit
- Gestionar logout

**Filtros disponibles:**
- **Estado:** Todos, Pendiente, En Progreso, Completado, Entregado
- **Prioridad:** Todas, Baja, Normal, Alta

**Estructura de la Grilla:**
```
| Título | Descripción | Estado | Prioridad | Acciones |
| -------|-------------|--------|-----------|----------|
| Trabajo 1 | Desc... | Pendiente | Alta | [Editar] [Notas] [...] |
| Trabajo 2 | Desc... | En Progreso | Normal | [Editar] [Notas] [...] |
```

---

### 6️⃣ **NoteEditor** - Editor de Notas
**Archivos:** `noteeditor.h` / `noteeditor.cpp`

**Responsabilidades:**
- Interfaz para editar notas de un trabajo
- Guardado manual con botón "Guardar Notas"
- Registra la acción en historial
- Confirmación visual de guardado

**Características:**
- Título en header azul
- TextEdit con placeholder
- Botones Cerrar y Guardar Notas
- Validación de guardado exitoso

---

### 7️⃣ **MainWidget** - Contenedor Principal
**Archivos:** `mainwidget.h` / `mainwidget.cpp`

**Responsabilidades:**
- Contenedor que alterna entre LoginWidget y WorkBoardWidget
- Gestiona el flujo de autenticación
- Maneja logout y retorno al login

---

## 🚀 Compilación y Ejecución

### Requisitos
- **Qt 6** (Widgets)
- **Compilador:** MinGW, MSVC o GCC (según plataforma)
- **qmake** (incluido con Qt)

### Compilación con qmake

Desde la carpeta `Ejercicio 1/proyect2/`:

```bash
# Generar Makefile
qmake proyect2.pro

# Compilar
mingw32-make    # Windows con MinGW
# o
make            # Linux/Mac

# Ejecutable se crea en la misma carpeta
```

### Ejecución

```bash
# Directamente en la carpeta del proyecto
./proyect2.exe      # Windows
./proyect2          # Linux/Mac

# Si compilaste en carpeta build:
./build/proyect2.exe
./build/proyect2
```

### Compilación alternativa (Qt Creator)
1. Abrir Qt Creator
2. Archivo → Abrir archivo o proyecto
3. Seleccionar `proyect2.pro`
4. Configurar kit Qt 6
5. Build → Compilar
6. Ejecutar con F5 o Build → Run

---

## 📁 Datos Persistentes

Los archivos JSON se almacenan automáticamente en la carpeta `data/` junto al ejecutable:

```json
// data/users.json
{
  "users": [
    { "username": "admin", "password": "admin" },
    { "username": "estudiante1", "password": "password123" }
  ]
}

// data/session.json
{
  "username": "admin",
  "timestamp": "2024-01-15T10:30:00"
}

// data/works.json
[
  {
    "id": "550e8400-e29b-41d4-a716-446655440000",
    "title": "Práctica 01",
    "description": "Introducción a Qt",
    "status": "Pendiente",
    "priority": "Alta",
    "dueDate": "2024-02-01T00:00:00",
    "createdDate": "2024-01-15T10:30:00",
    "createdBy": "admin",
    "notes": "Revisar documentación..."
  }
]

// data/history.json
[
  {
    "timestamp": "2024-01-15T10:35:00",
    "username": "admin",
    "actionType": "Crear",
    "description": "Nuevo trabajo: Práctica 01",
    "workId": "550e8400-e29b-41d4-a716-446655440000"
  }
]
```

---

## 📸 Capturas Requeridas para la Entrega

### 1. **Pantalla de Login**
- Mostrar formulario con usuario/contraseña
- Validación con credenciales `admin/admin`

### 2. **Tablero con Filtros**
- Vista en grilla con trabajos
- Filtros activos (estado y prioridad)
- Botones de acción visibles

### 3. **Editor con Estado Guardado**
- Abrir editor de notas
- Escribir contenido
- Mostrar "Guardar Notas" completado
- Indicador visual de éxito

### 4. **Historial Visible**
- Mostrar panel de historial en el tablero
- Listar acciones realizadas con timestamps
- Filtrado funcional

---

## ✅ Checklist de Funcionalidades

- ✔️ **Autenticación funcional** con usuarios en JSON
- ✔️ **Sesión de 5 minutos** - Login automático si hay sesión válida
- ✔️ **Tablero en grilla** con filtros por estado y prioridad
- ✔️ **CRUD completo** - Crear, leer, actualizar y eliminar trabajos
- ✔️ **Editor de notas** con guardado manual
- ✔️ **Historial visible** registrando todas las acciones
- ✔️ **Persistencia JSON** en carpeta `data/`
- ✔️ **Código organizado** en clases (sin todo en main.cpp)
- ✔️ **Interfaz usable** con diseño moderno
- ✔️ **QWidget** en lugar de QMainWindow

---

## 🎨 Detalles de Diseño

**Paleta de colores:**
- **Primario:** Indigo (#6366F1)
- **Secundario:** Cyan (#0EA5E9)
- **Fondo:** Light Slate (#F0F4FF)
- **Texto:** Slate (#1E293B)
- **Bordes:** Gray (#D1D5DB)

**Componentes UI:**
- Botones con hover y active states
- Inputs con focus color personalizado
- Labels con typography jerárquica
- Borders redondeados (8-10px)
- Spacing consistente

---

## 📝 Notas de Desarrollo

- La carpeta `data/` se crea automáticamente en la primera ejecución
- Los archivos JSON se generan si no existen
- Las sesiones expiran a los 5 minutos de inactividad
- Los UUIDs se generan automáticamente para cada trabajo
- El historial es global (registra acciones de todos los usuarios)

---

## 🔄 Flujo de Uso Típico

1. **Iniciar app** → Se verifica sesión activa
2. **Si no hay sesión** → Se muestra LoginWidget
3. **Ingresar credenciales** → Se valida y guarda sesión
4. **Ver tablero** → Se cargan trabajos con filtros
5. **Crear trabajo** → Se abre WorkDialog
6. **Editar/eliminar** → Se actualiza grilla e historial
7. **Ver notas** → Se abre NoteEditor
8. **Guardar notas** → Se persisten y registran en historial
9. **Logout** → Se borra sesión y retorna al login

---

## 🐛 Troubleshooting

**"No se encuentra Qt"**
- Verificar instalación de Qt 6
- Agregar Qt al PATH: `C:\Qt\6.x\bin`

**"Error de compilación con qmake"**
- Ejecutar `qmake -v` para verificar versión
- Regenerar Makefile: `qmake -r proyect2.pro`

**"Los datos no persisten"**
- Verificar permisos en carpeta `data/`
- Revisar que `data/` existe junto al ejecutable

**"Archivos JSON corruptos"**
- Borrar archivos en `data/` y reiniciar app
- Se regenerarán automáticamente

---

## 📞 Autor y Créditos

Trabajo práctico de **Programación Orientada a Objetos**  
Estudiante: Drako Lusicic  
Fecha: 2024

---

**¡Listo para entregar! 🚀**
