# 🖥️ Ejercicio 02 - Panel de Monitoreo VPS

## 🎯 Descripción del Proyecto

Aplicación desktop profesional desarrollada en **Qt 6** que funciona como panel de monitoreo de un servidor VPS en la nube. El panel consume datos de un endpoint HTTP que devuelve métricas de salud del servidor en formato JSON y presenta los datos en una interfaz moderna e interactiva, inspirada en herramientas profesionales como **Grafana**, **Netdata** y **Zabbix**.

### ✨ Características Principales

- 📊 **Monitoreo en tiempo real** - Refresco automático configurable
- 🟢 **Indicador de estado** - OK (verde), Alerta (amarillo), Caído (rojo)
- 📈 **Métricas visuales** - CPU, RAM, disco con barras de progreso
- 🔔 **Sistema de alertas** - Umbrales configurables por métrica
- 📜 **Historial de eventos** - Log con colores por severidad
- ⚙️ **Panel de configuración** - URL, intervalo y umbrales personalizables
- 🎨 **Tema oscuro profesional** - Inspirado en dashboards modernos
- 🔄 **Refresco manual** - Botón para consultar métricas al instante

---

## 🏗️ Arquitectura y Organización del Código

### Estructura de Carpetas

```
Ejercicio 2/
├── README.md
└── Ejercicio02-Lusicic/
    ├── Ejercicio02-Lusicic.pro      # Archivo de configuración Qt/qmake
    ├── main.cpp                     # Punto de entrada
    │
    ├── mainwindow.h/cpp             # Interfaz gráfica (panel de monitoreo)
    ├── servermonitor.h/cpp          # Lógica de monitoreo y peticiones HTTP
    │
    └── build/                       # Carpeta de compilación
```

---

## 📚 Componentes Principales

### 1️⃣ **ServerMonitor** - Clase de Lógica de Monitoreo
**Archivos:** `servermonitor.h` / `servermonitor.cpp`

**Responsabilidades:**
- Gestionar peticiones HTTP periódicas al endpoint
- Parsear respuestas JSON
- Emitir señales con métricas actualizadas
- Detectar condiciones de alerta (umbrales)
- Mantener historial de eventos

**Estructura de datos (ServerMetrics):**
```cpp
struct ServerMetrics {
    QString   status;        // "ok", "warning", "down"
    QString   uptime;        // Ej: "15d 3h 42m"
    double    latencyMs;     // Latencia en milisegundos
    double    cpuLoad;       // % de CPU (0-100)
    qint64    memoryUsed;    // MB en uso
    qint64    memoryTotal;   // MB disponibles
    double    diskUsed;      // GB usados
    double    diskTotal;     // GB totales
    QDateTime lastCheck;     // Timestamp del chequeo
    bool      reachable;     // Si el servidor respondió
};
```

**Flujo de funcionamiento:**
```
┌─────────────────────────┐
│   QTimer Activo         │  (intervalo configurable)
└────────────┬────────────┘
             │
             ▼
┌─────────────────────────┐
│   fetchNow()            │  (consulta manual o periódica)
└────────────┬────────────┘
             │
             ▼
┌─────────────────────────┐
│  GET <url>  (HTTP)      │  (petición al endpoint)
└────────────┬────────────┘
             │
             ▼
┌─────────────────────────┐
│  parseJson()            │  (analiza JSON)
└────────────┬────────────┘
             │
             ▼
┌─────────────────────────┐
│  checkAlerts()          │  (verifica umbrales)
└────────────┬────────────┘
             │
             ▼
┌─────────────────────────────────────────┐
│  Emite: metricsUpdated(ServerMetrics)   │
│         eventLogged(QString)             │
│         errorOccurred(QString)           │
└─────────────────────────────────────────┘
```

**Métodos principales:**
```cpp
// Configuración
void setEndpointUrl(const QString &url);           // URL del endpoint
void setInterval(int seconds);                     // Intervalo de chequeo
void setAlertThreshold(double cpuPct, int memPct); // Umbrales de alerta

// Control
void startMonitoring();                            // Iniciar monitoreo
void stopMonitoring();                             // Detener monitoreo
void fetchNow();                                   // Consulta inmediata

// Señales emitidas
void metricsUpdated(const ServerMetrics &metrics); // Nuevas métricas
void errorOccurred(const QString &errorMsg);       // Error en consulta
void eventLogged(const QString &entry);            // Evento en historial
```

**Endpoint esperado:**
```
GET /api/health
→ 200 OK
→ JSON:
{
  "status": "ok",           // o "warning", "down"
  "uptime": "15d 3h 42m",
  "cpu_load": 23.5,
  "memory_used": 2048,
  "memory_total": 4096,
  "disk_used": 45.2,
  "disk_total": 100.0
}
```

---

### 2️⃣ **MainWindow** - Interfaz Gráfica del Panel
**Archivos:** `mainwindow.h` / `mainwindow.cpp`

**Responsabilidades:**
- Presentar interfaz del panel de monitoreo
- Mostrar métricas en tiempo real
- Permitir configuración de parámetros
- Mantener historial visual
- Captar eventos del usuario

**Justificación de Widgets Utilizados:**

| Widget | Razón | Ejemplo |
|--------|-------|---------|
| **QLabel** | Solo lectura, mostrar valores numéricos y estado | Uptime, latencia, estado general |
| **QProgressBar** | Representación visual de porcentajes | CPU %, RAM %, Disco % |
| **QLineEdit** | Ingreso de texto modificable por el usuario | URL del endpoint |
| **QSpinBox** | Valores enteros (intervalo en seg, umbrales) | 30 seg, 512 MB |
| **QDoubleSpinBox** | Valores decimales de precisión | 75.5% CPU umbral |
| **QPushButton** | Acciones puntuales del usuario | Actualizar, Aplicar, Limpiar |
| **QListWidget** | Historial con scroll y colores | Log de eventos |
| **QGroupBox** | Organización visual de secciones | Panel de Config, Métricas |

**Disposición de la Interfaz:**

```
┌─────────────────────────────────────────────────────────────┐
│ 🖥️ Panel de Monitoreo VPS                                   │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ Estado: ● OK (Verde)    │ Último chequeo: 10:30 AM │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                              │
│  ┌──────────────┬──────────────┬──────────────────────┐    │
│  │ Uptime       │ Latencia     │ CPU Load             │    │
│  │ 15d 3h 42m   │ 28.5 ms      │ ▓░░░░ 23.5%          │    │
│  └──────────────┴──────────────┴──────────────────────┘    │
│                                                              │
│  ┌──────────────┬──────────────┬──────────────────────┐    │
│  │ Memoria      │ Disco        │                      │    │
│  │ ▓▓░░░ 50.2%  │ ▓▓▓░░ 45.2GB │                      │    │
│  │ 2048 / 4096  │ de 100 GB    │                      │    │
│  └──────────────┴──────────────┴──────────────────────┘    │
│                                                              │
│  ┌─ Configuración ───────────────────────────────────────┐ │
│  │ URL: [http://example.com/api/health______]            │ │
│  │ Intervalo: [30] segundos                              │ │
│  │ Umbral CPU: [75.5] %      Umbral RAM: [80] %         │ │
│  │ [Aplicar Configuración] [Actualizar Ahora]           │ │
│  └───────────────────────────────────────────────────────┘ │
│                                                              │
│  ┌─ Historial de Eventos (últimas 50 acciones) ─────────┐ │
│  │ [OK] 10:30:15 - Métricas actualizadas correctamente  │ │
│  │ [⚠ ALERTA] 10:29:50 - CPU excedió 75%                │ │
│  │ [OK] 10:29:30 - Conexión establecida                 │ │
│  │ [❌ ERROR] 10:29:00 - Timeout en petición HTTP       │ │
│  │ [Limpiar Historial]                                   │ │
│  └───────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

**Paleta de colores (Tema Oscuro):**
- **Fondo:** #1E1E2E (gris oscuro)
- **Primario:** #00D9FF (cian brillante)
- **Verde (OK):** #00FF41 (verde lima)
- **Amarillo (Alerta):** #FFD60A (amarillo)
- **Rojo (Error/Caído):** #FF0000 (rojo)
- **Texto:** #E0E0E0 (gris claro)
- **Bordes:** #404040 (gris medio)

---

### 3️⃣ **main.cpp** - Punto de Entrada
**Archivo:** `main.cpp`

```cpp
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
```

**Responsabilidades:**
- Crear instancia de QApplication
- Configurar metadata de la aplicación
- Mostrar ventana principal
- Ejecutar event loop

---

## 🚀 Compilación y Ejecución

### Requisitos
- **Qt 6** (Widgets + Network)
- **Compilador:** MinGW, MSVC o GCC
- **qmake**

### Compilación

Desde la carpeta `Ejercicio 2/Ejercicio02-Lusicic/`:

```bash
# Generar Makefile
qmake Ejercicio02-Lusicic.pro

# Compilar
mingw32-make    # Windows con MinGW
make            # Linux/Mac
```

### Ejecución

```bash
# Directamente
./Ejercicio02-Lusicic.exe      # Windows
./Ejercicio02-Lusicic          # Linux/Mac

# O desde la carpeta build
./build/Ejercicio02-Lusicic.exe
./build/Ejercicio02-Lusicic
```

---

## 🔌 Configuración del Endpoint

### URL del Servidor
Por defecto, la aplicación intenta conectarse a:
```
http://localhost:8080/api/health
```

Puedes cambiar esta URL en el panel de configuración:
1. Ingresa una nueva URL en el campo `QLineEdit`
2. Haz clic en "Aplicar Configuración"

### Intervalo de Monitoreo
- **Valor por defecto:** 10 segundos
- **Rango sugerido:** 5-60 segundos
- **Cambiar:** Usa el `QSpinBox` e "Aplicar Configuración"

### Umbrales de Alerta
- **CPU:** Default 75% (configurable con `QDoubleSpinBox`)
- **Memoria:** Default 80% (configurable con `QSpinBox`)
- Cuando se exceden, aparece alerta en historial

---

## 📊 Flujo de Uso

### 1. Iniciar Aplicación
```
main() 
  → MainWindow creado
  → Tema oscuro aplicado
  → ServerMonitor iniciado con config por defecto
```

### 2. Configurar Endpoint
```
Usuario ingresa URL → [Aplicar] 
  → ServerMonitor.setEndpointUrl(url)
  → fetchNow() automático
```

### 3. Monitoreo Automático
```
QTimer (cada 10s)
  → ServerMonitor.fetchNow()
  → GET request → parseJson()
  → Signal metricsUpdated()
  → MainWindow.onMetricsUpdated()
  → Labels y ProgressBars actualizados
```

### 4. Cambiar Umbrales
```
Usuario modifica spinboxes → [Aplicar]
  → ServerMonitor.setAlertThreshold(cpu, mem)
  → Próxima consulta valida contra nuevos umbrales
```

### 5. Refresco Manual
```
Usuario → [Actualizar Ahora]
  → ServerMonitor.fetchNow()
  → Consulta inmediata (no espera timer)
  → Actualiza interfaz
```

---

## 📈 Inspiración en Sistemas Profesionales

La interfaz se inspiró en herramientas líderes de monitoreo:

| Aspecto | Inspiración | Implementación |
|--------|-------------|-----------------|
| **Dashboard visual** | Grafana | ProgressBars para métricas, QLabels para valores |
| **Tema oscuro** | Netdata/Datadog | #1E1E2E fondo, colores cian/verde |
| **Alertas por umbral** | Zabbix | QSpinBox para umbrales, historial coloreado |
| **Historial de eventos** | Prometheus | QListWidget con timestamps y severidad |
| **Configuración interactiva** | Todos | Panel configurable sin reiniciar |
| **Indicador de estado** | Todos | Badge con color según salud |

---

## 🎨 Elementos Interactivos

### Botones y Controles

| Control | Tipo | Acción |
|---------|------|--------|
| **URL Input** | QLineEdit | Ingresa URL del endpoint |
| **Intervalo (seg)** | QSpinBox | Configura periodicidad (5-300 seg) |
| **Umbral CPU (%)** | QDoubleSpinBox | Define límite de alerta (0.0-100.0%) |
| **Umbral RAM (%)** | QSpinBox | Define límite de alerta (0-100%) |
| **Aplicar Config** | QPushButton | Guarda configuración y aplica |
| **Actualizar Ahora** | QPushButton | Refresco manual inmediato |
| **Limpiar Historial** | QPushButton | Vacía la lista de eventos |

### Indicadores Visuales

- **ProgressBar CPU:** Verde (0-60%), Amarillo (60-80%), Rojo (80-100%)
- **ProgressBar RAM:** Verde (0-50%), Amarillo (50-80%), Rojo (80-100%)
- **ProgressBar Disco:** Verde (0-70%), Amarillo (70-90%), Rojo (90-100%)
- **Estado General:** 🟢 OK, 🟡 ALERTA, 🔴 CAÍDO

---

## 📜 Historial de Eventos

El historial registra:
- Actualizaciones exitosas: `[OK]`
- Alertas activadas: `[⚠️ ALERTA]`
- Errores de conexión: `[❌ ERROR]`
- Cambios de estado: `[ℹ️ INFO]`

Ejemplo de historial:
```
[OK] 10:35:42 - Métricas actualizadas correctamente
[⚠️ ALERTA] 10:34:15 - CPU excedió umbral: 82.3%
[OK] 10:33:50 - Conexión exitosa, latencia: 28ms
[❌ ERROR] 10:33:20 - Timeout conectando a servidor
[ℹ️ INFO] 10:33:10 - Configuración aplicada: Intervalo=15s
```

---

## ✅ Checklist de Cumplimiento

- ✔️ **Empty qmake Project** - Proyecto desde cero con .pro
- ✔️ **Endpoint HTTP** - Consume datos de servidor remoto
- ✔️ **Datos de salud** - Uptime, CPU, RAM, Disco, latencia
- ✔️ **Panel legible** - Interfaz clara y profesional
- ✔️ **Estado general** - Badge OK/ALERTA/CAÍDO
- ✔️ **Métricas principales** - Mostradas con valores y barras
- ✔️ **Último chequeo** - Timestamp visible
- ✔️ **Historial de eventos** - QListWidget con colores
- ✔️ **Widgets interactivos** - QSpinBox, QPushButton, QLineEdit, QProgressBar, QDoubleSpinBox, QLabel
- ✔️ **Justificación de widgets** - Documentada en mainwindow.h
- ✔️ **Inspiración profesional** - Estilo Grafana/Netdata/Zabbix
- ✔️ **Refresco manual** - Botón "Actualizar Ahora"
- ✔️ **Configuración** - URL, intervalo, umbrales editables
- ✔️ **Clase ServerMonitor** - Lógica desacoplada de UI

---

## 📸 Capturas Requeridas para la Entrega

### 1. **Panel de Monitoreo Activo**
- Mostrar todas las métricas (CPU, RAM, Disco)
- ProgressBars con valores porcentuales
- Indicador de estado verde (OK)

### 2. **Panel de Configuración**
- URL configurada
- Intervalos y umbrales visibles
- Botones de acción funcionales

### 3. **Historial de Eventos**
- Múltiples eventos listados
- Colores por severidad visibles
- Timestamps exactos

### 4. **Alerta Activada**
- Estado en rojo/amarillo
- Evento en historial con warning
- Badge actualizado

---

## 🔧 Configuración de Desarrollo

### En Qt Creator
1. Abrir `Ejercicio02-Lusicic.pro`
2. Configurar kit: Qt 6.x
3. Build → Compilar (Ctrl+B)
4. Run → Ejecutar (F5)

### Variables de Entorno (opcional)
```bash
# Para especificar URL de servidor de prueba
export VPS_MONITOR_URL=http://tu.servidor.com/api/health
```

---

## 🐛 Troubleshooting

| Problema | Causa | Solución |
|----------|-------|----------|
| "Error de conexión" | URL incorrecta o servidor offline | Verifica URL en panel, reinicia servidor |
| "Timeout en petición" | Servidor lento | Aumenta intervalo en configuración |
| "JSON inválido" | Endpoint retorna formato inesperado | Verifica formato con curl/Postman |
| "Compilación falla" | Qt Network no incluido | Añade `network` a `QT +=` en .pro |
| "Interfaz congelada" | Petición síncrona | Las peticiones son asíncronas, verifica logs |

---

## 📊 Estructura del Proyecto en Detalle

### Desacoplamiento Arquitectónico

```
┌──────────────────────────┐
│       main.cpp           │  Entrada
└────────┬─────────────────┘
         │
         ▼
┌──────────────────────────────────────┐
│        MainWindow (UI)                │  Presentación
│   - Label, ProgressBar, SpinBox      │
│   - Conecta señales de ServerMonitor │
└────────────┬────────────────┬────────┘
             │                │
             │ usa            │ conecta señales
             │                │
             ▼                ▼
      ┌──────────────────────────────┐
      │   ServerMonitor (Lógica)      │  Lógica de Negocio
      │   - HTTP requests             │
      │   - JSON parsing              │
      │   - Timers                    │
      │   - Alert checking            │
      └──────────────────────────────┘
             │
             ▼
      ┌──────────────────────────────┐
      │  Servidor VPS (Endpoint)      │  Datos Remotos
      │  /api/health                  │
      └──────────────────────────────┘
```

**Ventajas:**
- UI y lógica separadas
- Fácil de testear
- Señales/slots desacoplados
- Posibilidad de reusabilidad

---

## 📝 Ejemplo de Uso Completo

### 1. Inicializar la Aplicación
```cpp
// main.cpp
QApplication app(argc, argv);
MainWindow w;           // Crea UI e inicializa ServerMonitor
w.show();
return app.exec();
```

### 2. Usuario Configura URL
```
[Panel → URL Input] "https://mi-vps.com/api/metrics"
[Aplicar Configuración]
```

### 3. Primer Chequeo
```
ServerMonitor::setEndpointUrl("https://mi-vps.com/api/metrics")
ServerMonitor::fetchNow()  // GET request
onReplyFinished()          // Respuesta recibida
parseJson()                // Parse del JSON
checkAlerts()              // Validar umbrales
emit metricsUpdated()      // Señal a UI
MainWindow::onMetricsUpdated() // Actualiza labels
```

### 4. Monitoreo Periódico
```
QTimer inicia (cada 10 segundos)
→ fetchNow()
→ Repite paso 3
```

---

## 🎓 Conceptos Clave Utilizados

- **Qt Signals/Slots:** Comunicación entre ServerMonitor y MainWindow
- **QNetworkAccessManager:** Peticiones HTTP asíncronas
- **QJsonDocument:** Parseo de JSON
- **QTimer:** Monitoreo periódico
- **QProgressBar:** Visualización de porcentajes
- **QListWidget:** Lista de eventos con estilos
- **Qt Stylesheets:** Tema personalizado

---

## 📞 Autor y Créditos

Trabajo práctico de **Programación Orientada a Objetos**  
Estudiante: Drako Lusicic  
Tema: Panel de Monitoreo VPS  
Fecha: 2024

---

**¡Listo para entregar! 🚀**
