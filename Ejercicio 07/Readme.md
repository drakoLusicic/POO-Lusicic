# 🌦️ Ejercicio 07 — Login con Clima, Fondo Dinámico y Proxy (Qt / C++)

## 🎯 Objetivo del ejercicio

Implementar una aplicación completa en Qt que integre:

* Autenticación de usuario (login)
* Consumo de API externa (clima)
* Descarga dinámica de recursos (imagen de fondo)
* Manejo de red (incluyendo proxy)
* Arquitectura orientada a objetos
* Manejo de errores y modo offline
* Logging y configuración externa

👉 No es un ejercicio aislado: es un **mini-sistema real**.

---

# 🧠 ARQUITECTURA GENERAL

El sistema está compuesto por tres capas bien diferenciadas:

```text
[ UI ]
  ↓
[ Lógica / Control ]
  ↓
[ Red / Datos ]
```

---

# 🧱 1. CLASE BASE: `Pantalla` (ABSTRACTA)

## 🔑 Rol

Define una **interfaz común** para todas las pantallas.

```cpp
class Pantalla : public QWidget {
    Q_OBJECT
public:
    virtual void inicializar() = 0;
};
```

👉 No se instancia directamente.

## 🧠 Qué lográs con esto

* Forzar consistencia entre pantallas
* Permitir polimorfismo
* Escalar el sistema fácilmente

---

# 🖥️ 2. LOGIN — LÓGICA REAL

## 🔐 Autenticación

Credenciales:

```text
usuario: admin
password: 1234
```

---

## 🚫 Control de intentos

```cpp
int intentos = 0;

if(user == "admin" && pass == "1234") {
    // acceso permitido
} else {
    intentos++;
}
```

### 🔥 Regla clave

```cpp
if(intentos >= 3){
    bloquearLogin();
}
```

👉 Bloqueo temporal (ej: 10–30 segundos)

---

## 🌡️ Datos mostrados

En el login se muestran:

* Temperatura actual
* Hora local

👉 Esto implica que el login **no es pasivo**, depende de red.

---

# 🌐 3. CLASE `Clima` (DATOS + RED)

## 🔑 Responsabilidad

Encapsula todo lo relacionado a:

* API externa
* parsing de datos
* manejo de errores

---

## 🔗 Request HTTP (SIN SDK)

Uso obligatorio:

```cpp
QNetworkAccessManager *manager;
QNetworkRequest request;
```

Ejemplo conceptual:

```cpp
request.setUrl(QUrl("https://api.openweathermap.org/..."));
manager->get(request);
```

---

## 🔐 Autenticación API

Puede ir:

* en query:

```text
?appid=API_KEY
```

* o en header:

```cpp
request.setRawHeader("Authorization", "API_KEY");
```

---

## 🔄 SIGNAL / SLOT

```cpp
connect(manager, &QNetworkAccessManager::finished,
        this, &Clima::onReply);
```

👉 Flujo:

```text
Request → async → respuesta → slot → procesamiento
```

---

## 📦 Procesamiento de datos

* Parseo JSON
* Extracción de:

  * temperatura
  * hora

👉 Validación obligatoria:

```cpp
if(!json.contains("main")) → error
```

---

# 🖼️ 4. DESCARGA DE IMAGEN DE FONDO

## 🔗 Desde URL pública

```cpp
manager->get(QNetworkRequest(QUrl("https://...imagen.jpg")));
```

---

## 💾 Cache local

* Se guarda en disco
* Si ya existe → no se descarga de nuevo

```text
cache/fondo.jpg
```

---

## 🔑 Condición crítica

👉 La ventana principal **NO abre hasta que la imagen esté lista**

```cpp
if(imagenDescargada){
    abrirVentana();
}
```

---

# 🖥️ 5. VENTANA PRINCIPAL

## 📺 Características

* Fullscreen
* Fondo adaptado

---

## 🖼️ Escalado correcto

```cpp
pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
```

👉 Evita:

* deformación
* pérdida de calidad

---

## 📄 Contenido

Currículum estilo LinkedIn:

* Foto
* Nombre
* Descripción
* Skills

👉 Esto evita una UI vacía y suma puntos de presentación.

---

# 🌐 6. SOPORTE PARA PROXY

## 🔧 Configuración

```cpp
QNetworkProxy proxy;
proxy.setHostName("IP");
proxy.setPort(8888);
proxy.setUser("user");
proxy.setPassword("pass");

QNetworkProxy::setApplicationProxy(proxy);
```

👉 Todo el tráfico HTTP pasa por ahí.

---

# 📁 7. CONFIGURACIÓN EXTERNA

Archivo:

```ini
config.ini
```

Ejemplo:

```ini
[api]
key=TU_API_KEY
ciudad=Cordoba

[proxy]
enabled=true
host=IP
port=8888
user=xxx
password=xxx
```

👉 Ventaja:

* no hardcodear datos
* fácil mantenimiento

---

# 🧾 8. LOGGING

Archivo:

```text
logs/eventos.log
```

Ejemplo:

```text
[2026-04-20 18:32] Login fallido
[2026-04-20 18:33] Login exitoso
[2026-04-20 18:33] Clima obtenido correctamente
```

👉 Se registra:

* acciones de usuario
* errores
* eventos de red

---

# 🌐 9. MODO OFFLINE

## 🔍 Detección

Si falla:

```cpp
QNetworkReply::NetworkError
```

---

## 🔁 Comportamiento

* no se rompe la app
* usa datos simulados

```cpp
temperatura = 20;
estado = "Offline";
```

👉 UX consistente incluso sin internet

---

# ⚠️ 10. MANEJO DE ERRORES

## Casos cubiertos:

* API caída
* JSON inválido
* sin conexión
* credenciales incorrectas

👉 UI muestra mensajes claros:

```text
"No se pudo obtener el clima"
"Verifique su conexión"
```

---

# 🔄 FLUJO COMPLETO DEL SISTEMA

```text
App inicia
   ↓
Login se muestra
   ↓
Se solicita clima (async)
   ↓
Se descarga fondo (async)
   ↓
Usuario intenta login
   ↓
✔ válido → espera imagen → abre ventana
❌ inválido → contador intentos
   ↓
Si falla red → modo offline
```

---

# 🎯 DECISIONES IMPORTANTES (ANÁLISIS REAL)

## ✔ Bien resuelto

* Uso de `QNetworkAccessManager` correctamente async
* Separación de responsabilidades (Login / Clima / Ventana)
* Manejo de errores (no crashea)
* Proxy integrado (nivel avanzado)
* Cache de imagen (optimización real)

---

## ⚠️ Mejorable

* Auth hardcodeada → debería ir a backend
* Falta cifrado de datos sensibles
* Logging básico (podría tener niveles: INFO, ERROR)
* No hay retry automático en red

---

# 📊 ESTADO FINAL

✔ Login funcional
✔ API integrada
✔ Imagen dinámica
✔ Proxy operativo
✔ Offline soportado
✔ UI completa


**FIN**

