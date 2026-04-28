#ifndef SERVERMONITOR_H
#define SERVERMONITOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QString>
#include <QDateTime>

// Estructura que agrupa todas las métricas recibidas del servidor
struct ServerMetrics {
    QString   status;        // "ok", "warning", "down"
    QString   uptime;        // Texto descriptivo del uptime
    double    latencyMs;     // Latencia del endpoint en milisegundos
    double    cpuLoad;       // Porcentaje de carga de CPU (0–100)
    qint64    memoryUsed;    // MB en uso
    qint64    memoryTotal;   // MB totales
    double    diskUsed;      // GB usados
    double    diskTotal;     // GB totales
    QDateTime lastCheck;     // Momento del último chequeo
    bool      reachable;     // Si el servidor respondió la petición

    ServerMetrics()
        : latencyMs(0.0), cpuLoad(0.0), memoryUsed(0), memoryTotal(0),
          diskUsed(0.0), diskTotal(0.0), reachable(false)
    {}
};

/**
 * ServerMonitor — Clase de lógica de monitoreo.
 *
 * Gestiona el temporizador periódico, las peticiones HTTP y el
 * análisis de la respuesta JSON. Emite señales para desacoplar la
 * lógica del servidor de la interfaz gráfica (MainWindow).
 *
 * Endpoints esperados:
 *   GET <url>  →  200 OK  →  JSON:
 *   {
 *     "status":       "ok" | "warning" | "down",
 *     "uptime":       "15d 3h 42m",
 *     "cpu_load":     23.5,
 *     "memory_used":  2048,
 *     "memory_total": 4096,
 *     "disk_used":    45.2,
 *     "disk_total":   100.0
 *   }
 */
class ServerMonitor : public QObject
{
    Q_OBJECT

public:
    explicit ServerMonitor(QObject *parent = nullptr);
    ~ServerMonitor() override = default;

    // Configuración
    void setEndpointUrl(const QString &url);
    void setInterval(int seconds);
    void setAlertThreshold(double cpuPct, int memPct);

    // Control del ciclo de monitoreo
    void startMonitoring();
    void stopMonitoring();

public slots:
    // Consulta inmediata (para el botón de refresco manual)
    void fetchNow();

signals:
    void metricsUpdated(const ServerMetrics &metrics);
    void errorOccurred(const QString &errorMsg);
    void eventLogged(const QString &entry);   // entradas del historial

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    ServerMetrics parseJson(const QByteArray &data);
    void          checkAlerts(const ServerMetrics &m);

    QNetworkAccessManager *m_manager;
    QTimer                *m_timer;
    QString                m_url;
    double                 m_cpuThreshold;  // % de CPU para alerta
    int                    m_memThreshold;  // % de RAM para alerta
};

#endif // SERVERMONITOR_H
