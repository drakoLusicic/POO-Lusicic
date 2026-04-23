# 🚀 Ejercicio 04 – Deploy en VPS con Docker

Este proyecto documenta el proceso completo de despliegue de una aplicación en un servidor VPS utilizando **SSH, SCP y Docker Compose**, junto con la verificación de su correcto funcionamiento.

---

## 🧩 Requisitos previos

Antes de comenzar, asegurate de contar con:

* Acceso a un **VPS**
* Cliente **SSH**
* Archivos del proyecto en tu máquina local
* **Docker** y **Docker Compose** instalados en el servidor

---

## 🔐 1. Conexión al servidor (SSH)

Primero se establece conexión con el servidor desde la terminal local:

```bash
ssh usuario@ip_del_servidor
```

---

## 📁 2. Creación del directorio en el VPS

Se crea el directorio donde se alojará el proyecto:

```bash
mkdir Ejercicio-04
```

---

## 📤 3. Subida de archivos al servidor (SCP)

Se copia el proyecto desde la máquina local al VPS:

```bash
scp -r Ejercicio-04 usuario@ip_del_servidor:/ruta/destino
```

---

## 🔍 4. Verificación de archivos

Una vez dentro del servidor:

```bash
ls
cd Ejercicio-04
ls -la
```

Esto permite confirmar que los archivos se copiaron correctamente.

---

## 🐳 5. Levantar los contenedores con Docker

Para iniciar los servicios:

```bash
docker compose up -d
```

⏱️ Esperar aproximadamente 30 segundos.

Luego verificar el estado:

```bash
docker compose ps
```

✔️ Ambos contenedores deben aparecer en estado **UP**

---

## 🔄 6. Reinicio de contenedores (si falla)

Si algún contenedor no está funcionando correctamente:

```bash
docker compose down
docker compose up -d
```

---

## 🌐 7. Verificación de la API (curl)

Se prueba la conexión y autenticación desde la terminal:

```bash
curl -i -u admin:1234 http://ip_host:port/health
```

✔️ Si responde correctamente, la API está funcionando.

---

## 🖥️ 8. Prueba desde la aplicación

Ejecutar el cliente desde **Qt Creator** para validar la integración completa.

---

## 🔑 9. Credenciales de acceso

* 🌍 URL: http://192.163.180.102:8000
* 👤 Usuario: `admin`
* 🔒 Contraseña: `1234`

---

## ✅ Conclusión

El despliegue se considera exitoso cuando:

* Los contenedores están en estado **UP**
* La API responde correctamente vía `curl`
* La aplicación cliente se conecta sin errores

---

## 🧠 Notas finales

* Si algo falla, revisar logs con:

  ```bash
  docker compose logs
  ```
* Verificar puertos abiertos en el VPS
* Confirmar credenciales y rutas

---

💡 *Este flujo es reutilizable para futuros despliegues en entornos similares.*
