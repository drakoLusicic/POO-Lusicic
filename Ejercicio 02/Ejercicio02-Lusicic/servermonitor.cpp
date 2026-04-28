#include "servermonitor.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRegularExpression>
#include <QUrl>

// ─────────────────────────────────────────────────────────
// Constructor / destructor
// ─────────────────────────────────────────────────────────
ServerMonitor::ServerMonitor(QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
    , m_timer(new QTimer(this))
    , m_cpuThreshold(80.0)
    , m_memThreshold(85)
{
    // Intervalo predeterminado: 30 segundos
    m_timer->setInterval(30 * 1000);

    connect(m_manager, &QNetworkAccessManager::finished,
            this,      &ServerMonitor::onReplyFinished);

    connect(m_timer, &QTimer::timeout,
            this,    &ServerMonitor::fetchNow);
}

// ─────────────────────────────────────────────────────────
// Configuración pública
// ─────────────────────────────────────────────────────────
void ServerMonitor::setEndpointUrl(const QString &url)
{
    QString normalized = url.trimmed();
    if (normalized.isEmpty()) {
        m_url.clear();
        return;
    }

    // Si el usuario pega solo IP/host, asumir http:// para facilitar uso.
    if (!normalized.startsWith("http://", Qt::CaseInsensitive) &&
        !normalized.startsWith("https://", Qt::CaseInsensitive)) {
        normalized.prepend("http://");
    }

    QUrl parsed = QUrl::fromUserInput(normalized);
    if (parsed.path().isEmpty() || parsed.path() == "/") {
        parsed.setPath("/health");
    }

    m_url = parsed.toString(QUrl::FullyEncoded);
}

void ServerMonitor::setInterval(int seconds)
{
    m_timer->setInterval(qMax(1, seconds) * 1000);
    if (m_timer->isActive())
        m_timer->start(); // reinicia con el nuevo intervalo
}

void ServerMonitor::setAlertThreshold(double cpuPct, int memPct)
{
    m_cpuThreshold = cpuPct;
    m_memThreshold = memPct;
}

// ─────────────────────────────────────────────────────────
// Control del ciclo de monitoreo
// ─────────────────────────────────────────────────────────
void ServerMonitor::startMonitoring()
{
    if (!m_timer->isActive())
        m_timer->start();
    fetchNow(); // consulta inmediata al iniciar
}

void ServerMonitor::stopMonitoring()
{
    m_timer->stop();
}

// ─────────────────────────────────────────────────────────
// Slot: consulta al endpoint
// ─────────────────────────────────────────────────────────
void ServerMonitor::fetchNow()
{
    if (m_url.isEmpty()) {
        emit errorOccurred("URL del endpoint no configurada.");
        return;
    }

    QUrl url = QUrl::fromUserInput(m_url);
    // Validación básica de URL antes de enviar la petición
    if (!url.isValid() || url.scheme().isEmpty() || url.host().isEmpty()) {
        emit errorOccurred("URL inválida: " + m_url);
        return;
    }

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "VPSMonitor/1.0");
    request.setRawHeader("Accept", "application/json");
    // Tiempo de espera de la respuesta: 10 s
    request.setTransferTimeout(10000);

    m_manager->get(request);
}

// ─────────────────────────────────────────────────────────
// Slot: procesamiento de la respuesta HTTP
// ─────────────────────────────────────────────────────────
void ServerMonitor::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        ServerMetrics down;
        down.status    = "down";
        down.reachable = false;
        down.lastCheck = QDateTime::currentDateTime();

        emit metricsUpdated(down);
        emit errorOccurred(reply->errorString());
        emit eventLogged("[" + down.lastCheck.toString("hh:mm:ss") +
                         "] CRÍTICO: " + reply->errorString());
        return;
    }

    QByteArray body = reply->readAll();
    ServerMetrics metrics = parseJson(body);
    metrics.lastCheck = QDateTime::currentDateTime();

    checkAlerts(metrics);
    emit metricsUpdated(metrics);
}

// ─────────────────────────────────────────────────────────
// Parseo de la respuesta JSON
// ─────────────────────────────────────────────────────────
ServerMetrics ServerMonitor::parseJson(const QByteArray &data)
{
    ServerMetrics m;
    m.reachable = true;

    if (data.trimmed().isEmpty()) {
        m.status = "warning";
        emit errorOccurred("El endpoint respondió vacío.");
        emit eventLogged("[" + QDateTime::currentDateTime().toString("hh:mm:ss") +
                         "] ERROR: respuesta vacía del endpoint");
        return m;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);

    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        m.status = "warning";
        emit errorOccurred("JSON inválido en endpoint: " + err.errorString());
        emit eventLogged("[" + QDateTime::currentDateTime().toString("hh:mm:ss") +
                         "] ERROR PARSE: " + err.errorString());
        return m;
    }

    QJsonObject obj = doc.object();
    // Soporta respuestas tipo { "data": { ... } } además de objeto plano.
    if (obj.value("data").isObject()) {
        obj = obj.value("data").toObject();
    }

    auto valueFor = [&](std::initializer_list<const char *> keys) -> QJsonValue {
        for (const char *k : keys) {
            if (obj.contains(QLatin1String(k))) {
                return obj.value(QLatin1String(k));
            }
        }
        return QJsonValue();
    };

    auto toNumber = [](const QJsonValue &v, double defVal) -> double {
        if (v.isDouble()) {
            return v.toDouble(defVal);
        }
        if (v.isString()) {
            // Extrae números de strings como "73%" o "12.5 GB".
            const QString s = v.toString().trimmed();
            const QRegularExpression re("[-+]?\\d*\\.?\\d+");
            const QRegularExpressionMatch mt = re.match(s);
            if (mt.hasMatch()) {
                bool ok = false;
                const double n = mt.captured(0).toDouble(&ok);
                if (ok) {
                    return n;
                }
            }
        }
        return defVal;
    };

    m.status = valueFor({"status", "state", "overall_status", "estado"})
                   .toString("unknown")
                   .trimmed()
                   .toLower();

    const QJsonValue uptimeValue = valueFor(
        {"uptime", "up_time", "uptime_text", "tiempo_activo", "uptime_seconds", "up_seconds"});
    m.uptime = uptimeValue.toString("N/A");

    const QJsonObject cpuObj = obj.value("cpu").toObject();
    const QJsonObject memObj = obj.value("memory").isObject()
                                   ? obj.value("memory").toObject()
                                   : obj.value("ram").toObject();
    const QJsonObject diskObj = obj.value("disk").isObject()
                                    ? obj.value("disk").toObject()
                                    : obj.value("storage").toObject();

    m.latencyMs = toNumber(valueFor({"response_time", "latency", "latency_ms", "ping", "responseTime"}), 0.0);

    m.cpuLoad = toNumber(valueFor({"cpu_load", "cpu", "cpu_usage", "cpu_percent", "carga_cpu"}), 0.0);
    if (m.cpuLoad <= 0.0 && !cpuObj.isEmpty()) {
        m.cpuLoad = toNumber(cpuObj.value("load"),
                    toNumber(cpuObj.value("usage"),
                    toNumber(cpuObj.value("percent"), 0.0)));
    }

    m.memoryUsed = static_cast<qint64>(toNumber(
        valueFor({"memory_used", "mem_used", "ram_used", "memoryUsed"}), 0.0));
    m.memoryTotal = static_cast<qint64>(toNumber(
        valueFor({"memory_total", "mem_total", "ram_total", "memoryTotal"}), 0.0));
    if ((m.memoryUsed <= 0 || m.memoryTotal <= 0) && !memObj.isEmpty()) {
        m.memoryUsed = static_cast<qint64>(toNumber(memObj.value("used"),
                        toNumber(memObj.value("usage"),
                        toNumber(memObj.value("used_mb"), static_cast<double>(m.memoryUsed)))));
        m.memoryTotal = static_cast<qint64>(toNumber(memObj.value("total"),
                         toNumber(memObj.value("total_mb"), static_cast<double>(m.memoryTotal))));
    }

    m.diskUsed = toNumber(valueFor({"disk_used", "storage_used", "diskUsed"}), 0.0);
    m.diskTotal = toNumber(valueFor({"disk_total", "storage_total", "diskTotal"}), 0.0);
    if ((m.diskUsed <= 0.0 || m.diskTotal <= 0.0) && !diskObj.isEmpty()) {
        m.diskUsed = toNumber(diskObj.value("used"),
                    toNumber(diskObj.value("used_gb"), m.diskUsed));
        m.diskTotal = toNumber(diskObj.value("total"),
                     toNumber(diskObj.value("total_gb"), m.diskTotal));
    }

    if (m.uptime == "N/A") {
        const double upSeconds = toNumber(uptimeValue, -1.0);
        if (upSeconds >= 0.0) {
            const int secs = static_cast<int>(upSeconds);
            const int days = secs / 86400;
            const int hours = (secs % 86400) / 3600;
            const int mins = (secs % 3600) / 60;
            m.uptime = QString("%1d %2h %3m").arg(days).arg(hours).arg(mins);
        }
    }

    // Fallback si el API entrega porcentajes en lugar de absolutos.
    const double memPct = toNumber(valueFor({"memory_percent", "mem_percent", "ram_percent", "memory_usage"}),
                                   toNumber(memObj.value("percent"), -1.0));
    if (m.memoryTotal <= 0 && memPct >= 0.0) {
        m.memoryTotal = 100;
        m.memoryUsed = static_cast<qint64>(memPct);
    }

    const double diskPct = toNumber(valueFor({"disk_percent", "storage_percent", "disk_usage"}),
                                    toNumber(diskObj.value("percent"), -1.0));
    if (m.diskTotal <= 0.0 && diskPct >= 0.0) {
        m.diskTotal = 100.0;
        m.diskUsed = diskPct;
    }

    if (m.status == "unknown") {
        // Derivar estado básico si el endpoint no lo informa explícitamente.
        if (m.cpuLoad >= 90.0) {
            m.status = "down";
        } else if (m.cpuLoad >= 75.0) {
            m.status = "warning";
        } else {
            m.status = "ok";
        }
    }

    return m;
}

// ─────────────────────────────────────────────────────────
// Evaluación de umbrales y generación de alertas
// ─────────────────────────────────────────────────────────
void ServerMonitor::checkAlerts(const ServerMetrics &m)
{
    const QString ts = "[" + QDateTime::currentDateTime().toString("hh:mm:ss") + "] ";

    if (!m.reachable) {
        emit eventLogged(ts + "CRÍTICO: Servidor inaccesible.");
        return;
    }

    // Alerta de CPU
    if (m.cpuLoad >= m_cpuThreshold) {
        emit eventLogged(ts + QString("ALERTA CPU: %1% ≥ umbral %2%")
                         .arg(m.cpuLoad, 0, 'f', 1)
                         .arg(m_cpuThreshold, 0, 'f', 0));
    }

    // Alerta de memoria
    if (m.memoryTotal > 0) {
        int memPct = static_cast<int>(
            static_cast<double>(m.memoryUsed) / m.memoryTotal * 100.0);
        if (memPct >= m_memThreshold) {
            emit eventLogged(ts + QString("ALERTA MEM: %1% ≥ umbral %2%")
                             .arg(memPct).arg(m_memThreshold));
        }
    }

    // Registro de estado general
    if (m.status == "ok")
        emit eventLogged(ts + "INFO: Servidor OK.");
    else if (m.status == "warning")
        emit eventLogged(ts + "AVISO: Estado degradado reportado por el servidor.");
    else if (m.status == "down")
        emit eventLogged(ts + "CRÍTICO: El servidor reporta estado caído.");
}
