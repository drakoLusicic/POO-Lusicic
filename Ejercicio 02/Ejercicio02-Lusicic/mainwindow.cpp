#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSizePolicy>
#include <QListWidgetItem>
#include <QDateTime>
#include <QFont>
#include <QColor>
#include <QStatusBar>

// ═══════════════════════════════════════════════════════════
// Constructor
// ═══════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Panel de Monitoreo VPS");
    setMinimumSize(920, 660);
    resize(1050, 720);

    m_monitor = new ServerMonitor(this);
    connect(m_monitor, &ServerMonitor::metricsUpdated, this, &MainWindow::onMetricsUpdated);
    connect(m_monitor, &ServerMonitor::errorOccurred,  this, &MainWindow::onError);
    connect(m_monitor, &ServerMonitor::eventLogged,    this, &MainWindow::onEventLogged);

    setupUi();
    applyDarkTheme();

    statusBar()->showMessage(
        "Configure el endpoint y presione \"Aplicar Configuración\" para iniciar el monitoreo.");
}

// ═══════════════════════════════════════════════════════════
// Construcción de la interfaz
// ═══════════════════════════════════════════════════════════
void MainWindow::setupUi()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setSpacing(10);
    rootLayout->setContentsMargins(14, 14, 14, 10);

    // ── 1. ENCABEZADO ─────────────────────────────────────
    QGroupBox   *headerBox    = new QGroupBox(central);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerBox);
    headerLayout->setContentsMargins(12, 10, 12, 10);

    QLabel *titleLbl = new QLabel("VPS MONITOR", headerBox);
    titleLbl->setObjectName("titleLabel");
    QFont titleFont("Consolas", 20, QFont::Bold);
    titleLbl->setFont(titleFont);

    m_statusBadge = new QLabel("  ●  DESCONOCIDO  ", headerBox);
    m_statusBadge->setObjectName("statusUnknown");
    m_statusBadge->setAlignment(Qt::AlignCenter);
    QFont badgeFont("Consolas", 13, QFont::Bold);
    m_statusBadge->setFont(badgeFont);

    m_lastCheckLabel = new QLabel("Último chequeo: —", headerBox);
    m_lastCheckLabel->setObjectName("lastCheckLabel");
    m_lastCheckLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    headerLayout->addWidget(titleLbl);
    headerLayout->addStretch();
    headerLayout->addWidget(m_statusBadge);
    headerLayout->addStretch();
    headerLayout->addWidget(m_lastCheckLabel);

    rootLayout->addWidget(headerBox);

    // ── 2. SECCIÓN CENTRAL: Métricas + Configuración ──────
    QHBoxLayout *midLayout = new QHBoxLayout();
    midLayout->setSpacing(10);

    // ── 2a. Panel de métricas ──────────────────────────────
    QGroupBox  *metricsBox  = new QGroupBox("Métricas del Servidor", central);
    metricsBox->setObjectName("metricsBox");
    QGridLayout *grid = new QGridLayout(metricsBox);
    grid->setVerticalSpacing(16);
    grid->setHorizontalSpacing(10);
    grid->setContentsMargins(14, 18, 14, 14);

    auto makeMetricLabel = [&](const QString &text, QWidget *parent) {
        QLabel *l = new QLabel(text, parent);
        l->setObjectName("metricLabel");
        return l;
    };
    auto makeValueLabel = [&](const QString &text, QWidget *parent) {
        QLabel *l = new QLabel(text, parent);
        l->setObjectName("metricValue");
        l->setFont(QFont("Consolas", 12, QFont::Bold));
        return l;
    };
    auto makeBar = [&](const QString &name, QWidget *parent) {
        QProgressBar *b = new QProgressBar(parent);
        b->setRange(0, 100);
        b->setValue(0);
        b->setObjectName(name);
        b->setFixedHeight(22);
        b->setFormat("%p%");
        return b;
    };

    // Fila 0 — Uptime
    grid->addWidget(makeMetricLabel("Tiempo en línea:", metricsBox), 0, 0);
    m_uptimeValue = makeValueLabel("N/A", metricsBox);
    grid->addWidget(m_uptimeValue, 0, 1, 1, 2);

    // Fila 1 — Latencia
    grid->addWidget(makeMetricLabel("Latencia:", metricsBox), 1, 0);
    m_latencyValue = makeValueLabel("N/A", metricsBox);
    grid->addWidget(m_latencyValue, 1, 1, 1, 2);

    // Fila 2 — CPU
    grid->addWidget(makeMetricLabel("Carga CPU:", metricsBox), 2, 0);
    m_cpuBar = makeBar("cpuBar", metricsBox);
    m_cpuValue = makeValueLabel("0.0 %", metricsBox);
    m_cpuValue->setMinimumWidth(80);
    m_cpuValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(m_cpuBar,   2, 1);
    grid->addWidget(m_cpuValue, 2, 2);

    // Fila 3 — Memoria RAM
    grid->addWidget(makeMetricLabel("Memoria RAM:", metricsBox), 3, 0);
    m_memBar = makeBar("memBar", metricsBox);
    m_memValue = makeValueLabel("0 / 0 MB", metricsBox);
    m_memValue->setMinimumWidth(80);
    m_memValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(m_memBar,   3, 1);
    grid->addWidget(m_memValue, 3, 2);

    // Fila 4 — Disco
    grid->addWidget(makeMetricLabel("Disco:", metricsBox), 4, 0);
    m_diskBar = makeBar("diskBar", metricsBox);
    m_diskValue = makeValueLabel("0.0 / 0.0 GB", metricsBox);
    m_diskValue->setMinimumWidth(80);
    m_diskValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    grid->addWidget(m_diskBar,   4, 1);
    grid->addWidget(m_diskValue, 4, 2);

    grid->setColumnStretch(1, 1);
    metricsBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // ── 2b. Panel de configuración ─────────────────────────
    QGroupBox   *ctrlBox    = new QGroupBox("Configuración", central);
    ctrlBox->setObjectName("ctrlBox");
    ctrlBox->setFixedWidth(280);
    QVBoxLayout *ctrlLayout = new QVBoxLayout(ctrlBox);
    ctrlLayout->setSpacing(6);
    ctrlLayout->setContentsMargins(12, 18, 12, 12);

    auto makeCtrlLabel = [&](const QString &text, QWidget *parent) {
        QLabel *l = new QLabel(text, parent);
        l->setObjectName("ctrlLabel");
        return l;
    };

    // Endpoint URL — QLineEdit justificado: texto libre, el usuario
    // escribe la dirección del servidor que desea monitorear.
    ctrlLayout->addWidget(makeCtrlLabel("Endpoint del servidor:", ctrlBox));
    m_urlInput = new QLineEdit(ctrlBox);
    m_urlInput->setObjectName("urlInput");
    m_urlInput->setPlaceholderText("http://mi-vps.com/api/health");
    m_urlInput->setToolTip(
        "URL del endpoint JSON que devuelve las métricas del VPS.\n"
        "Ejemplo: http://192.168.1.10:8080/health");
    ctrlLayout->addWidget(m_urlInput);

    ctrlLayout->addSpacing(4);

    // Intervalo — QSpinBox justificado: valor entero en segundos,
    // define la frecuencia del chequeo automático (rango 5–300 s).
    ctrlLayout->addWidget(makeCtrlLabel("Intervalo de chequeo:", ctrlBox));
    m_intervalSpin = new QSpinBox(ctrlBox);
    m_intervalSpin->setRange(5, 300);
    m_intervalSpin->setValue(30);
    m_intervalSpin->setSuffix("  s");
    m_intervalSpin->setObjectName("intervalSpin");
    m_intervalSpin->setToolTip(
        "Cada cuántos segundos se consulta el servidor automáticamente.\n"
        "Mínimo: 5 s   Máximo: 300 s");
    ctrlLayout->addWidget(m_intervalSpin);

    ctrlLayout->addSpacing(4);

    // Umbral CPU — QDoubleSpinBox justificado: porcentaje con un
    // decimal, configurar a qué nivel de CPU se dispara la alerta.
    ctrlLayout->addWidget(makeCtrlLabel("Umbral de alerta CPU:", ctrlBox));
    m_cpuThreshSpin = new QDoubleSpinBox(ctrlBox);
    m_cpuThreshSpin->setRange(10.0, 100.0);
    m_cpuThreshSpin->setValue(80.0);
    m_cpuThreshSpin->setDecimals(1);
    m_cpuThreshSpin->setSuffix("  %");
    m_cpuThreshSpin->setObjectName("cpuThreshSpin");
    m_cpuThreshSpin->setToolTip(
        "Si la carga de CPU supera este valor se genera una alerta\n"
        "y la barra se colorea en rojo.");
    ctrlLayout->addWidget(m_cpuThreshSpin);

    ctrlLayout->addSpacing(4);

    // Umbral memoria — QSpinBox justificado: porcentaje entero para
    // definir el umbral de uso de RAM que activa la alerta.
    ctrlLayout->addWidget(makeCtrlLabel("Umbral de alerta Memoria:", ctrlBox));
    m_memThreshSpin = new QSpinBox(ctrlBox);
    m_memThreshSpin->setRange(10, 100);
    m_memThreshSpin->setValue(85);
    m_memThreshSpin->setSuffix("  %");
    m_memThreshSpin->setObjectName("memThreshSpin");
    m_memThreshSpin->setToolTip(
        "Si el uso de RAM supera este porcentaje se genera una alerta\n"
        "y la barra se colorea en rojo.");
    ctrlLayout->addWidget(m_memThreshSpin);

    ctrlLayout->addSpacing(8);

    // Botón "Aplicar Configuración" — QPushButton: confirma y activa
    // la URL, el intervalo y los umbrales ingresados arriba.
    m_applyBtn = new QPushButton("Aplicar Configuración", ctrlBox);
    m_applyBtn->setObjectName("applyBtn");
    m_applyBtn->setToolTip("Guarda los parámetros y (re)inicia el monitoreo automático.");
    connect(m_applyBtn, &QPushButton::clicked, this, &MainWindow::onApplySettingsClicked);
    ctrlLayout->addWidget(m_applyBtn);

    // Botón "Actualizar Ahora" — QPushButton: refresco manual
    // inmediato sin esperar el intervalo periódico.
    m_refreshBtn = new QPushButton("  Actualizar Ahora", ctrlBox);
    m_refreshBtn->setObjectName("refreshBtn");
    m_refreshBtn->setToolTip("Dispara una consulta manual al servidor de forma inmediata.");
    connect(m_refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshClicked);
    ctrlLayout->addWidget(m_refreshBtn);

    ctrlLayout->addSpacing(4);

    // Botón "Limpiar Historial" — QPushButton: vacía la lista de
    // eventos para mantener el panel limpio cuando se desee.
    m_clearBtn = new QPushButton("Limpiar Historial", ctrlBox);
    m_clearBtn->setObjectName("clearBtn");
    m_clearBtn->setToolTip("Elimina todas las entradas del historial de eventos.");
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::onClearHistoryClicked);
    ctrlLayout->addWidget(m_clearBtn);

    ctrlLayout->addStretch();

    midLayout->addWidget(metricsBox);
    midLayout->addWidget(ctrlBox);
    rootLayout->addLayout(midLayout);

    // ── 3. HISTORIAL DE EVENTOS ───────────────────────────
    QGroupBox   *histBox    = new QGroupBox("Historial de Eventos", central);
    histBox->setObjectName("histBox");
    QVBoxLayout *histLayout = new QVBoxLayout(histBox);
    histLayout->setContentsMargins(10, 16, 10, 10);

    m_historyList = new QListWidget(histBox);
    m_historyList->setObjectName("historyList");
    m_historyList->setMaximumHeight(160);
    m_historyList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    histLayout->addWidget(m_historyList);

    rootLayout->addWidget(histBox);
}

// ═══════════════════════════════════════════════════════════
// Hoja de estilos — tema oscuro inspirado en Grafana/Netdata
// ═══════════════════════════════════════════════════════════
void MainWindow::applyDarkTheme()
{
    setStyleSheet(R"(
        /* ── Base ── */
        QMainWindow, QWidget {
            background-color: #111827;
            color: #d1d5db;
            font-family: "Segoe UI", Arial, sans-serif;
        }

        /* ── GroupBox ── */
        QGroupBox {
            border: 1px solid #1e3a5f;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 6px;
            font-weight: bold;
            font-size: 11px;
            color: #60a5fa;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 6px;
            color: #60a5fa;
        }

        /* ── Encabezado ── */
        QLabel#titleLabel {
            color: #93c5fd;
            font-size: 20px;
            letter-spacing: 2px;
        }
        QLabel#lastCheckLabel {
            color: #6b7280;
            font-size: 11px;
        }

        /* ── Badge de estado — coloreado dinámicamente en C++ ── */

        /* ── Etiquetas ── */
        QLabel#metricLabel {
            color: #9ca3af;
            font-size: 12px;
            min-width: 110px;
        }
        QLabel#metricValue {
            color: #e5e7eb;
            font-size: 12px;
        }
        QLabel#ctrlLabel {
            color: #9ca3af;
            font-size: 11px;
        }

        /* ── Barras de progreso ── */
        QProgressBar {
            border: 1px solid #1e3a5f;
            border-radius: 4px;
            background-color: #1f2937;
            text-align: center;
            color: #e5e7eb;
            font-size: 11px;
            font-family: "Consolas", monospace;
        }
        QProgressBar#cpuBar::chunk {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #2563eb, stop:1 #38bdf8);
            border-radius: 4px;
        }
        QProgressBar#memBar::chunk {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #7c3aed, stop:1 #a78bfa);
            border-radius: 4px;
        }
        QProgressBar#diskBar::chunk {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #059669, stop:1 #34d399);
            border-radius: 4px;
        }

        /* ── Inputs ── */
        QLineEdit {
            background-color: #1f2937;
            border: 1px solid #374151;
            border-radius: 4px;
            padding: 5px 8px;
            color: #e5e7eb;
            font-size: 12px;
        }
        QLineEdit:focus { border: 1px solid #3b82f6; }

        QSpinBox, QDoubleSpinBox {
            background-color: #1f2937;
            border: 1px solid #374151;
            border-radius: 4px;
            padding: 4px 8px;
            color: #e5e7eb;
            font-size: 12px;
        }
        QSpinBox:focus, QDoubleSpinBox:focus { border: 1px solid #3b82f6; }

        /* ── Botones ── */
        QPushButton {
            background-color: #1f2937;
            border: 1px solid #374151;
            border-radius: 4px;
            padding: 7px 12px;
            color: #d1d5db;
            font-size: 12px;
        }
        QPushButton:hover  { background-color: #374151; border-color: #60a5fa; }
        QPushButton:pressed { background-color: #111827; }

        QPushButton#refreshBtn {
            background-color: #1e40af;
            border-color: #3b82f6;
            color: #ffffff;
            font-weight: bold;
        }
        QPushButton#refreshBtn:hover  { background-color: #2563eb; }
        QPushButton#refreshBtn:pressed { background-color: #1e3a8a; }

        QPushButton#applyBtn {
            background-color: #065f46;
            border-color: #10b981;
            color: #d1fae5;
        }
        QPushButton#applyBtn:hover  { background-color: #059669; }

        QPushButton#clearBtn {
            background-color: #3b1010;
            border-color: #7f1d1d;
            color: #fca5a5;
        }
        QPushButton#clearBtn:hover  { background-color: #7f1d1d; }

        /* ── Lista de historial ── */
        QListWidget {
            background-color: #0d1117;
            border: 1px solid #1e3a5f;
            border-radius: 4px;
            color: #9ca3af;
            font-family: "Consolas", "Courier New", monospace;
            font-size: 11px;
        }
        QListWidget::item {
            padding: 3px 8px;
            border-bottom: 1px solid #1f2937;
        }
        QListWidget::item:selected { background-color: #1e3a5f; }

        /* ── Barra de estado ── */
        QStatusBar {
            background-color: #0d1117;
            color: #6b7280;
            border-top: 1px solid #1e3a5f;
            font-size: 11px;
        }

        /* ── Scrollbar ── */
        QScrollBar:vertical {
            background: #1f2937;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #374151;
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover { background: #60a5fa; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
    )");
}

// ═══════════════════════════════════════════════════════════
// Slots — actualización de la UI con las métricas recibidas
// ═══════════════════════════════════════════════════════════
void MainWindow::onMetricsUpdated(const ServerMetrics &metrics)
{
    // ── Badge de estado ───────────────────────────────────
    if (!metrics.reachable) {
        m_statusBadge->setText("  ●  CAÍDO  ");
        m_statusBadge->setStyleSheet(
            "color:#ef4444; background:#2d1212; padding:6px 16px;"
            "border-radius:5px; border:1px solid #ef4444;");
    } else if (metrics.status == "ok") {
        m_statusBadge->setText("  ●  OK  ");
        m_statusBadge->setStyleSheet(
            "color:#22c55e; background:#12201a; padding:6px 16px;"
            "border-radius:5px; border:1px solid #22c55e;");
    } else if (metrics.status == "warning") {
        m_statusBadge->setText("  ●  ALERTA  ");
        m_statusBadge->setStyleSheet(
            "color:#f59e0b; background:#201a0a; padding:6px 16px;"
            "border-radius:5px; border:1px solid #f59e0b;");
    } else {
        m_statusBadge->setText("  ●  " + metrics.status.toUpper() + "  ");
        m_statusBadge->setStyleSheet(
            "color:#9ca3af; background:#1f2937; padding:6px 16px;"
            "border-radius:5px; border:1px solid #374151;");
    }

    // ── Último chequeo ────────────────────────────────────
    m_lastCheckLabel->setText(
        "Último chequeo:  " + metrics.lastCheck.toString("dd/MM/yyyy  hh:mm:ss"));

    // ── Uptime ────────────────────────────────────────────
    m_uptimeValue->setText(metrics.reachable ? metrics.uptime : "—");
    m_latencyValue->setText(
        (metrics.reachable && metrics.latencyMs > 0.0)
            ? QString::number(metrics.latencyMs, 'f', 0) + " ms"
            : "N/A");

    if (!metrics.reachable) {
        // Sin datos numéricos si el servidor no responde
        statusBar()->showMessage(
            "Sin conexión — " + metrics.lastCheck.toString("hh:mm:ss"));
        return;
    }

    // ── CPU ───────────────────────────────────────────────
    {
        int pct = qBound(0, static_cast<int>(metrics.cpuLoad), 100);
        m_cpuBar->setValue(pct);
        m_cpuValue->setText(QString("%1 %").arg(metrics.cpuLoad, 0, 'f', 1));

        if (metrics.cpuLoad >= m_cpuThreshSpin->value()) {
            m_cpuBar->setStyleSheet(
                "QProgressBar::chunk{background:#ef4444;border-radius:4px;}");
        } else {
            m_cpuBar->setStyleSheet(""); // restaura el estilo del tema
        }
    }

    // ── Memoria RAM ───────────────────────────────────────
    if (metrics.memoryTotal > 0) {
        int pct = static_cast<int>(
            static_cast<double>(metrics.memoryUsed) / metrics.memoryTotal * 100.0);
        pct = qBound(0, pct, 100);
        m_memBar->setValue(pct);

        if (metrics.memoryTotal == 100 && metrics.memoryUsed <= 100) {
            m_memValue->setText(QString::number(pct) + " %");
        } else if (metrics.memoryTotal >= 1024) {
            m_memValue->setText(
                QString("%1 / %2 GB")
                .arg(metrics.memoryUsed  / 1024.0, 0, 'f', 1)
                .arg(metrics.memoryTotal / 1024.0, 0, 'f', 1));
        } else {
            m_memValue->setText(
                QString("%1 / %2 MB")
                .arg(metrics.memoryUsed).arg(metrics.memoryTotal));
        }

        if (pct >= m_memThreshSpin->value()) {
            m_memBar->setStyleSheet(
                "QProgressBar::chunk{background:#ef4444;border-radius:4px;}");
        } else {
            m_memBar->setStyleSheet("");
        }
    }

    // ── Disco ─────────────────────────────────────────────
    if (metrics.diskTotal > 0.0) {
        int pct = static_cast<int>(metrics.diskUsed / metrics.diskTotal * 100.0);
        pct = qBound(0, pct, 100);
        m_diskBar->setValue(pct);
        if (metrics.diskTotal == 100.0 && metrics.diskUsed <= 100.0) {
            m_diskValue->setText(QString::number(metrics.diskUsed, 'f', 1) + " %");
        } else {
            m_diskValue->setText(
                QString("%1 / %2 GB")
                .arg(metrics.diskUsed,  0, 'f', 1)
                .arg(metrics.diskTotal, 0, 'f', 1));
        }
    }

    // ── Status bar ────────────────────────────────────────
    statusBar()->showMessage(
        "Conectado  —  Datos actualizados a las "
        + metrics.lastCheck.toString("hh:mm:ss")
        + ((metrics.latencyMs > 0.0)
               ? "  —  Latencia: " + QString::number(metrics.latencyMs, 'f', 0) + " ms"
               : ""));
}

void MainWindow::onError(const QString &errorMsg)
{
    statusBar()->showMessage("Error: " + errorMsg);
}

void MainWindow::onEventLogged(const QString &entry)
{
    appendHistory(entry);
}

void MainWindow::onRefreshClicked()
{
    statusBar()->showMessage("Consultando servidor...");
    m_monitor->fetchNow();
}

void MainWindow::onApplySettingsClicked()
{
    const QString url      = m_urlInput->text().trimmed();
    const int     interval = m_intervalSpin->value();
    const double  cpuThr   = m_cpuThreshSpin->value();
    const int     memThr   = m_memThreshSpin->value();

    m_monitor->stopMonitoring();
    m_monitor->setEndpointUrl(url);
    m_monitor->setInterval(interval);
    m_monitor->setAlertThreshold(cpuThr, memThr);
    m_monitor->startMonitoring();

    appendHistory(
        "[" + QDateTime::currentDateTime().toString("hh:mm:ss") + "] "
        "CONFIG aplicada — Intervalo: " + QString::number(interval) + "s | "
        "Umbral CPU: "  + QString::number(cpuThr, 'f', 0) + "% | "
        "Umbral MEM: "  + QString::number(memThr) + "%");

    statusBar()->showMessage(
        "Configuración aplicada. Monitoreando cada "
        + QString::number(interval) + " s — " + url);
}

void MainWindow::onClearHistoryClicked()
{
    m_historyList->clear();
}

// ═══════════════════════════════════════════════════════════
// Helpers
// ═══════════════════════════════════════════════════════════
void MainWindow::appendHistory(const QString &entry)
{
    QListWidgetItem *item = new QListWidgetItem(entry);

    // Color por severidad (igual que Grafana/Netdata usan semáforos)
    if (entry.contains("CRÍT") || entry.contains("ERROR") || entry.contains("CAÍDO")) {
        item->setForeground(QColor("#f87171"));  // rojo
    } else if (entry.contains("ALERTA") || entry.contains("AVISO")) {
        item->setForeground(QColor("#fbbf24"));  // amarillo
    } else if (entry.contains("OK") || entry.contains("INFO")) {
        item->setForeground(QColor("#4ade80"));  // verde
    } else if (entry.contains("CONFIG")) {
        item->setForeground(QColor("#60a5fa"));  // azul
    } else {
        item->setForeground(QColor("#9ca3af"));  // gris
    }

    // Inserta al principio → evento más reciente arriba (como Datadog)
    m_historyList->insertItem(0, item);

    // Limitar a 150 entradas para no saturar memoria
    while (m_historyList->count() > 150)
        delete m_historyList->takeItem(m_historyList->count() - 1);
}
