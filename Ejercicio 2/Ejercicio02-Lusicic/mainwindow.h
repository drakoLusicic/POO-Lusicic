#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QListWidget>
#include <QProgressBar>
#include <QGroupBox>

#include "servermonitor.h"

/**
 * MainWindow — Panel de monitoreo VPS estilo Grafana/Netdata.
 *
 * Justificación de widgets:
 *  · QLabel          → muestra métricas, estado y último chequeo (información de solo lectura)
 *  · QProgressBar    → representación visual porcentual de CPU, RAM y disco
 *  · QLineEdit       → ingreso de la URL del endpoint (configurable por el usuario)
 *  · QSpinBox        → intervalo de chequeo en segundos y umbral de memoria (valores enteros)
 *  · QDoubleSpinBox  → umbral de CPU en porcentaje con decimal (valor numérico con decimales)
 *  · QPushButton (×3)→ "Actualizar Ahora" (refresco manual), "Aplicar Configuración" y
 *                      "Limpiar Historial" (acciones puntuales del usuario)
 *  · QListWidget     → historial de eventos con colores por severidad (log tipo consola)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onMetricsUpdated(const ServerMetrics &metrics);
    void onError(const QString &errorMsg);
    void onEventLogged(const QString &entry);
    void onRefreshClicked();
    void onApplySettingsClicked();
    void onClearHistoryClicked();

private:
    void setupUi();
    void applyDarkTheme();
    void appendHistory(const QString &entry);

    // ── Lógica de monitoreo ──────────────────────────────
    ServerMonitor  *m_monitor;

    // ── Encabezado ───────────────────────────────────────
    QLabel         *m_statusBadge;      // Estado general: OK / ALERTA / CAÍDO
    QLabel         *m_lastCheckLabel;   // Timestamp del último chequeo exitoso

    // ── Tarjetas de métricas ─────────────────────────────
    QLabel         *m_uptimeValue;
    QLabel         *m_latencyValue;
    QProgressBar   *m_cpuBar;
    QLabel         *m_cpuValue;
    QProgressBar   *m_memBar;
    QLabel         *m_memValue;
    QProgressBar   *m_diskBar;
    QLabel         *m_diskValue;

    // ── Panel de configuración ───────────────────────────
    QLineEdit      *m_urlInput;         // URL del endpoint
    QSpinBox       *m_intervalSpin;     // Intervalo de chequeo (s)
    QDoubleSpinBox *m_cpuThreshSpin;    // Umbral de alerta CPU (%)
    QSpinBox       *m_memThreshSpin;    // Umbral de alerta memoria (%)
    QPushButton    *m_applyBtn;         // Aplicar configuración
    QPushButton    *m_refreshBtn;       // Refresco manual
    QPushButton    *m_clearBtn;         // Limpiar historial

    // ── Historial de eventos ─────────────────────────────
    QListWidget    *m_historyList;
};

#endif // MAINWINDOW_H
