# 🛠️ Setup de Forward Proxy en VPS (Contabo) con Tinyproxy

## 📅 Fecha

Abril 2026

## 🎯 Objetivo

Implementar un **Forward Proxy seguro y funcional** en un VPS (Contabo) para enrutar tráfico de una aplicación cliente (Qt), con control de acceso por IP, autenticación y firewall restrictivo.

---

# 🧱 0. Supuestos iniciales

* VPS con **Ubuntu/Debian**
* Acceso por **SSH con privilegios sudo**
* IP pública del VPS: `194.163.180.102`
* IPs clientes permitidas:

  * `186.13.47.15`
  * `186.158.234.134`
* Puerto del proxy: `8888`
* Credenciales:

  * Usuario: `chulengo`
  * Contraseña: `chunchu123`

---

# 📁 1. Preparación de estructura

Se creó una estructura organizada para mantener configuraciones, backups y documentación:

```bash
sudo mkdir -p /opt/proxy/docs
sudo mkdir -p /opt/proxy/backups
sudo mkdir -p /opt/proxy/scripts
sudo chown -R root:root /opt/proxy
sudo chmod -R 755 /opt/proxy
```

---

# 📦 2. Instalación de dependencias

```bash
sudo apt update
sudo apt install -y tinyproxy ufw curl
```

---

# 💾 3. Backup de configuración original

```bash
sudo cp /etc/tinyproxy/tinyproxy.conf /opt/proxy/backups/tinyproxy.conf.original.$(date +%F-%H%M%S)
```

---

# ⚙️ 4. Configuración de Tinyproxy

Se configuró el archivo principal:

```bash
sudo nano /etc/tinyproxy/tinyproxy.conf
```

Contenido aplicado:

```conf
User tinyproxy
Group tinyproxy
Port 8888
Timeout 600

LogFile "/var/log/tinyproxy/tinyproxy.log"
LogLevel Info
PidFile "/run/tinyproxy/tinyproxy.pid"

Allow 127.0.0.1
Allow ::1
Allow 186.13.47.15/32
Allow 186.158.234.134/32

BasicAuth chulengo chunchu123

DisableViaHeader Yes

ConnectPort 80
ConnectPort 443
```

📌 Características:

* Acceso restringido por IP
* Autenticación básica
* Soporte HTTP/HTTPS
* Logs habilitados

Backup activo:

```bash
sudo cp /etc/tinyproxy/tinyproxy.conf /opt/proxy/docs/tinyproxy.conf.activo
```

---

# 🚀 5. Levantar servicio

```bash
sudo systemctl daemon-reload
sudo systemctl enable tinyproxy
sudo systemctl restart tinyproxy
sudo systemctl status tinyproxy --no-pager
```

Verificación de puerto:

```bash
sudo ss -ltnp | grep 8888
```

---

# 🛡️ 6. Configuración de Firewall (UFW)

```bash
sudo ufw --force reset

sudo ufw default deny incoming
sudo ufw default allow outgoing

sudo ufw allow OpenSSH

sudo ufw allow from 186.13.47.15 to any port 8888 proto tcp
sudo ufw allow from 186.158.234.134 to any port 8888 proto tcp

sudo ufw --force enable
sudo ufw status verbose
```

---

# 🌐 6.1 Firewall en Contabo (Cloud Firewall)

Se configuraron reglas adicionales:

### Permitir:

* Puerto 22 (SSH)
* Puerto 8888 solo para:

  * `186.13.47.15/32`
  * `186.158.234.134/32`

### Denegar:

* Todo el resto del tráfico

---

# 🧪 7. Pruebas funcionales

## Test local (VPS)

```bash
curl -x http://chulengo:chunchu123@127.0.0.1:8888 https://api.ipify.org
```

## Test remoto (cliente)

```bash
curl -x http://chulengo:chunchu123@194.163.180.102:8888 https://api.ipify.org
```

✔ Resultado esperado:

```
194.163.180.102
```

---

# 📊 8. Verificación de logs

```bash
sudo tail -f /var/log/tinyproxy/tinyproxy.log
```

Permite monitorear tráfico en tiempo real.

---

# 🖥️ 9. Integración con aplicación Qt

Archivo `config.ini`:

```ini
[proxy]
enabled=true
host=194.163.180.102
port=8888
user=chulengo
password=chunchu123
```

---

# 🔄 10. Mantenimiento

Backup antes de cambios:

```bash
sudo cp /etc/tinyproxy/tinyproxy.conf /opt/proxy/backups/tinyproxy.conf.$(date +%F-%H%M%S)
```

Reinicio:

```bash
sudo systemctl restart tinyproxy
```

---

# 🔒 11. Endurecimiento (opcional)

* Uso de contraseñas más robustas
* Restricción a puerto 443 únicamente
* Implementación de VPN (WireGuard)
* Rotación periódica de credenciales

---

# ♻️ 12. Rollback

Restaurar configuración:

```bash
sudo cp /opt/proxy/backups/tinyproxy.conf.original.FECHA /etc/tinyproxy/tinyproxy.conf
sudo systemctl restart tinyproxy
```

Detener servicio:

```bash
sudo systemctl stop tinyproxy
```

---

# 🎯 Resultado final

Se implementó un:

* ✔ Forward Proxy funcional
* ✔ Acceso restringido por IP
* ✔ Autenticación activa
* ✔ Firewall en doble capa (UFW + Contabo)
* ✔ Integración con aplicación cliente

---

# 🧠 Concepto clave

El proxy actúa como intermediario:

```
Cliente (Qt) → VPS (Tinyproxy) → Internet
```

Ocultando la IP real del cliente y centralizando el tráfico.

---

# 📌 Estado del sistema

✅ Operativo
🔒 Seguro (nivel académico/producción básica)
🚀 Listo para uso e integración

---

**FIN**
