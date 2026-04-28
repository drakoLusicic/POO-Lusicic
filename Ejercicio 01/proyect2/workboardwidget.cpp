#include "workboardwidget.h"
#include "workdialog.h"
#include "noteeditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>
#include <QLayoutItem>

WorkBoardWidget::WorkBoardWidget(const QString& username, QWidget *parent)
    : QWidget(parent), currentUsername(username) {

    workManager = new WorkManager();
    historyManager = new HistoryManager();

    setupUI();
    refreshBoard();
    refreshHistoryView();

    setWindowTitle("Planificador de Trabajos Practicos - " + username);
    resize(1400, 820);
    setStyleSheet(
        "QWidget { background-color: #FFFFFF; font-family: 'Segoe UI', Arial; color: #1E293B; }"
        "QScrollBar:vertical { background: #F1F5F9; width: 8px; border-radius: 4px; }"
        "QScrollBar::handle:vertical { background: #CBD5E1; border-radius: 4px; }"
        "QScrollBar::handle:vertical:hover { background: #94A3B8; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    );
}

void WorkBoardWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ── Topbar ──────────────────────────────────────────────────
    QWidget *topbar = new QWidget();
    topbar->setFixedHeight(64);
    topbar->setStyleSheet(
        "background-color: #6366F1;"
    );
    QHBoxLayout *toolbarLayout = new QHBoxLayout(topbar);
    toolbarLayout->setContentsMargins(20, 0, 20, 0);
    toolbarLayout->setSpacing(12);

    // Logo/título
    QLabel *appLabel = new QLabel("TP Planner");
    appLabel->setStyleSheet(
        "color: white; font-size: 18px; font-weight: bold; background: transparent;"
    );
    toolbarLayout->addWidget(appLabel);

    // Separador visual
    QFrame *sep1 = new QFrame();
    sep1->setFrameShape(QFrame::VLine);
    sep1->setStyleSheet("color: rgba(255,255,255,0.3);");
    toolbarLayout->addWidget(sep1);

    userLabel = new QLabel(QString::fromUtf8("\xF0\x9F\x91\xA4") + " " + currentUsername);
    userLabel->setStyleSheet(
        "color: #E0E7FF; font-size: 13px; font-weight: 600; background: transparent;"
    );
    toolbarLayout->addWidget(userLabel);

    toolbarLayout->addStretch();

    // Botón agregar
    addButton = new QPushButton("+ Nuevo Trabajo");
    addButton->setMinimumHeight(36);
    addButton->setMinimumWidth(140);
    addButton->setCursor(Qt::PointingHandCursor);
    addButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #10B981;"
        "  color: white; border: none;"
        "  border-radius: 8px;"
        "  font-size: 13px; font-weight: bold;"
        "  padding: 0 16px;"
        "}"
        "QPushButton:hover { background-color: #059669; }"
        "QPushButton:pressed { background-color: #047857; }"
    );
    toolbarLayout->addWidget(addButton);

    // Botón logout
    logoutButton = new QPushButton("Salir");
    logoutButton->setMinimumHeight(36);
    logoutButton->setMinimumWidth(70);
    logoutButton->setCursor(Qt::PointingHandCursor);
    logoutButton->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(255,255,255,0.15);"
        "  color: white; border: 1px solid rgba(255,255,255,0.4);"
        "  border-radius: 8px;"
        "  font-size: 13px; font-weight: bold;"
        "  padding: 0 14px;"
        "}"
        "QPushButton:hover { background-color: #EF4444; border-color: #EF4444; }"
    );
    toolbarLayout->addWidget(logoutButton);

    mainLayout->addWidget(topbar);

    // ── Barra de filtros ────────────────────────────────────────
    QWidget *filterBar = new QWidget();
    filterBar->setFixedHeight(56);
    filterBar->setStyleSheet(
        "background-color: #F8FAFC;"
        "border-bottom: 1px solid #E2E8F0;"
    );
    QHBoxLayout *filterLayout = new QHBoxLayout(filterBar);
    filterLayout->setContentsMargins(20, 0, 20, 0);
    filterLayout->setSpacing(10);

    QString filterLabelStyle =
        "color: #64748B; font-size: 12px; font-weight: 600;"
        "background: transparent;";
    QString comboStyle =
        "QComboBox {"
        "  border: 1.5px solid #D1D5DB;"
        "  border-radius: 8px;"
        "  padding: 4px 10px;"
        "  font-size: 13px;"
        "  color: #374151;"
        "  background-color: #FFFFFF;"
        "  min-width: 130px;"
        "}"
        "QComboBox:focus { border-color: #6366F1; }"
        "QComboBox::drop-down { border: none; }"
        "QComboBox QAbstractItemView {"
        "  border: 1px solid #E2E8F0;"
        "  border-radius: 6px;"
        "  selection-background-color: #EEF2FF;"
        "  selection-color: #4F46E5;"
        "}";

    QLabel *statusLabel = new QLabel("Estado:");
    statusLabel->setStyleSheet(filterLabelStyle);
    statusFilter = new QComboBox();
    statusFilter->addItem("Todos");
    statusFilter->addItem("Pendiente");
    statusFilter->addItem("En Progreso");
    statusFilter->addItem("Completado");
    statusFilter->setStyleSheet(comboStyle);
    filterLayout->addWidget(statusLabel);
    filterLayout->addWidget(statusFilter);

    filterLayout->addSpacing(16);

    QLabel *priorityLabel = new QLabel("Prioridad:");
    priorityLabel->setStyleSheet(filterLabelStyle);
    priorityFilter = new QComboBox();
    priorityFilter->addItem("Todas");
    priorityFilter->addItem("Baja");
    priorityFilter->addItem("Normal");
    priorityFilter->addItem("Alta");
    priorityFilter->setStyleSheet(comboStyle);
    filterLayout->addWidget(priorityLabel);
    filterLayout->addWidget(priorityFilter);

    filterLayout->addStretch();
    mainLayout->addWidget(filterBar);

    // ── Área central (tabla + historial) ────────────────────────
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    // Tabla con scroll
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: #FFFFFF; }");

    gridWidget = new QWidget();
    gridWidget->setStyleSheet("background-color: #FFFFFF;");
    gridLayout = new QGridLayout(gridWidget);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(16, 16, 16, 16);
    rebuildGridHeaders();

    scrollArea->setWidget(gridWidget);
    contentLayout->addWidget(scrollArea, 3);

    // Panel lateral historial
    QWidget *historyPanel = new QWidget();
    historyPanel->setFixedWidth(300);
    historyPanel->setStyleSheet(
        "background-color: #F8FAFC;"
        "border-left: 1px solid #E2E8F0;"
    );
    QVBoxLayout *historyPanelLayout = new QVBoxLayout(historyPanel);
    historyPanelLayout->setContentsMargins(0, 0, 0, 0);
    historyPanelLayout->setSpacing(0);

    QWidget *historyHeader = new QWidget();
    historyHeader->setFixedHeight(44);
    historyHeader->setStyleSheet(
        "background-color: #EEF2FF;"
        "border-bottom: 1px solid #C7D2FE;"
    );
    QHBoxLayout *historyHeaderLayout = new QHBoxLayout(historyHeader);
    historyHeaderLayout->setContentsMargins(16, 0, 16, 0);
    QLabel *historyTitle = new QLabel("Historial de Acciones");
    historyTitle->setStyleSheet(
        "color: #4F46E5; font-weight: bold; font-size: 13px;"
        "background: transparent;"
    );
    historyHeaderLayout->addWidget(historyTitle);
    historyPanelLayout->addWidget(historyHeader);

    historyView = new QTextEdit();
    historyView->setReadOnly(true);
    historyView->setStyleSheet(
        "QTextEdit {"
        "  background-color: #F8FAFC;"
        "  border: none;"
        "  color: #475569;"
        "  font-size: 12px;"
        "  padding: 10px;"
        "}"
    );
    historyPanelLayout->addWidget(historyView);
    contentLayout->addWidget(historyPanel, 0);

    QWidget *contentWidget = new QWidget();
    contentWidget->setLayout(contentLayout);
    mainLayout->addWidget(contentWidget);

    // Conexiones
    connect(statusFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WorkBoardWidget::onFilterChanged);
    connect(priorityFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WorkBoardWidget::onFilterChanged);
    connect(addButton, &QPushButton::clicked, this, &WorkBoardWidget::onAddWorkClicked);
    connect(logoutButton, &QPushButton::clicked, this, &WorkBoardWidget::onLogoutClicked);

    setLayout(mainLayout);
}

void WorkBoardWidget::rebuildGridHeaders() {
    QStringList headers = {"Título", "Descripción", "Estado", "Prioridad", "Vencimiento", "Acciones"};
    for (int i = 0; i < headers.size(); ++i) {
        QLabel *headerLabel = new QLabel(headers[i]);
        headerLabel->setStyleSheet(
            "font-weight: bold;"
            "font-size: 11px;"
            "color: #6366F1;"
            "background-color: #EEF2FF;"
            "padding: 10px 12px;"
            "border-bottom: 2px solid #C7D2FE;"
            "letter-spacing: 0.5px;"
        );
        gridLayout->addWidget(headerLabel, 0, i);
    }
}

void WorkBoardWidget::clearGridRows() {
    for (int index = gridLayout->count() - 1; index >= 0; --index) {
        int row = 0;
        int column = 0;
        int rowSpan = 0;
        int columnSpan = 0;
        gridLayout->getItemPosition(index, &row, &column, &rowSpan, &columnSpan);

        if (row == 0) {
            continue;
        }

        QLayoutItem *item = gridLayout->takeAt(index);
        if (!item) {
            continue;
        }

        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void WorkBoardWidget::refreshBoard() {
    clearGridRows();

    // Obtener trabajos
    QString statusFilterText = statusFilter->currentText();
    QString priorityFilterText = priorityFilter->currentText();

    QList<Work> allWorks = workManager->getAllWorks();
    displayedWorks.clear();

    for (const auto& work : allWorks) {
        bool statusMatch = (statusFilterText == "Todos" || work.status == statusFilterText);
        bool priorityMatch = (priorityFilterText == "Todas" || work.priority == priorityFilterText);

        if (statusMatch && priorityMatch) {
            displayedWorks.append(work);
        }
    }

    // Agregar filas
    for (int i = 0; i < displayedWorks.size(); ++i) {
        addWorkRow(displayedWorks[i], i + 1);
    }
}

void WorkBoardWidget::onFilterChanged() {
    refreshBoard();
}

void WorkBoardWidget::addWorkRow(const Work& work, int row) {
    // Color de fila alternada
    QString rowBg = (row % 2 == 0) ? "#F8FAFC" : "#FFFFFF";
    QString cellBase = QString("padding: 10px 12px; background-color: %1; "
                               "border-bottom: 1px solid #F1F5F9; font-size: 13px;").arg(rowBg);

    // Título
    QLabel *titleLabel = new QLabel(work.title);
    titleLabel->setStyleSheet(cellBase + " color: #1E293B; font-weight: 600;");
    gridLayout->addWidget(titleLabel, row, 0);

    // Descripción (truncada)
    QString desc = work.description;
    if (desc.length() > 50) desc = desc.left(47) + "...";
    QLabel *descLabel = new QLabel(desc);
    descLabel->setStyleSheet(cellBase + " color: #64748B;");
    gridLayout->addWidget(descLabel, row, 1);

    // Badge de Estado
    QString statusBg, statusFg, statusHover;
    if (work.status == "Completado") {
        statusBg = "#D1FAE5"; statusFg = "#065F46"; statusHover = "#A7F3D0";
    } else if (work.status == "En Progreso") {
        statusBg = "#DBEAFE"; statusFg = "#1E40AF"; statusHover = "#BFDBFE";
    } else {
        statusBg = "#FEF3C7"; statusFg = "#92400E"; statusHover = "#FDE68A";
    }
    QPushButton *statusBtn = new QPushButton(work.status);
    statusBtn->setProperty("workId", work.id);
    statusBtn->setMinimumHeight(30);
    statusBtn->setCursor(Qt::PointingHandCursor);
    statusBtn->setStyleSheet(
        QString("QPushButton {"
                "  background-color: %1; color: %2;"
                "  border: none; border-radius: 12px;"
                "  font-size: 11px; font-weight: bold; padding: 4px 10px;"
                "}"
                "QPushButton:hover { background-color: %3; }").arg(statusBg, statusFg, statusHover)
    );
    connect(statusBtn, &QPushButton::clicked, this, &WorkBoardWidget::onStatusClicked);
    QWidget *statusCell = new QWidget();
    statusCell->setStyleSheet(QString("background-color: %1; border-bottom: 1px solid #F1F5F9;").arg(rowBg));
    QHBoxLayout *statusCellLayout = new QHBoxLayout(statusCell);
    statusCellLayout->setContentsMargins(8, 4, 8, 4);
    statusCellLayout->addWidget(statusBtn);
    statusCellLayout->addStretch();
    gridLayout->addWidget(statusCell, row, 2);

    // Badge Prioridad
    QString prioBg, prioFg;
    if (work.priority == "Alta")       { prioBg = "#FEE2E2"; prioFg = "#991B1B"; }
    else if (work.priority == "Normal") { prioBg = "#FEF3C7"; prioFg = "#92400E"; }
    else                                { prioBg = "#D1FAE5"; prioFg = "#065F46"; }
    QLabel *priorityLabel = new QLabel(work.priority);
    priorityLabel->setAlignment(Qt::AlignCenter);
    priorityLabel->setStyleSheet(
        QString("background-color: %1; color: %2;"
                "border-radius: 12px; font-size: 11px; font-weight: bold;"
                "padding: 4px 10px;").arg(prioBg, prioFg)
    );
    QWidget *prioCell = new QWidget();
    prioCell->setStyleSheet(QString("background-color: %1; border-bottom: 1px solid #F1F5F9;").arg(rowBg));
    QHBoxLayout *prioCellLayout = new QHBoxLayout(prioCell);
    prioCellLayout->setContentsMargins(8, 4, 8, 4);
    prioCellLayout->addWidget(priorityLabel);
    prioCellLayout->addStretch();
    gridLayout->addWidget(prioCell, row, 3);

    // Vencimiento
    QLabel *dateLabel = new QLabel(work.dueDate.toString("dd/MM/yyyy"));
    dateLabel->setStyleSheet(cellBase + " color: #64748B;");
    gridLayout->addWidget(dateLabel, row, 4);

    // Botones de acciones
    QWidget *actionWidget = new QWidget();
    actionWidget->setStyleSheet(QString("background-color: %1; border-bottom: 1px solid #F1F5F9;").arg(rowBg));
    QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(6, 4, 6, 4);
    actionLayout->setSpacing(6);

    QPushButton *editBtn = new QPushButton("Editar");
    editBtn->setProperty("workId", work.id);
    editBtn->setMinimumHeight(30);
    editBtn->setCursor(Qt::PointingHandCursor);
    editBtn->setStyleSheet(
        "QPushButton { background-color: #EEF2FF; color: #4F46E5; border: none;"
        "  border-radius: 6px; font-size: 12px; font-weight: bold; padding: 4px 10px; }"
        "QPushButton:hover { background-color: #6366F1; color: white; }"
    );
    connect(editBtn, &QPushButton::clicked, this, &WorkBoardWidget::onEditWorkClicked);
    actionLayout->addWidget(editBtn);

    QPushButton *notesBtn = new QPushButton("Notas");
    notesBtn->setProperty("workId", work.id);
    notesBtn->setMinimumHeight(30);
    notesBtn->setCursor(Qt::PointingHandCursor);
    notesBtn->setStyleSheet(
        "QPushButton { background-color: #F0F9FF; color: #0369A1; border: none;"
        "  border-radius: 6px; font-size: 12px; font-weight: bold; padding: 4px 10px; }"
        "QPushButton:hover { background-color: #0EA5E9; color: white; }"
    );
    connect(notesBtn, &QPushButton::clicked, this, &WorkBoardWidget::onNotesClicked);
    actionLayout->addWidget(notesBtn);

    QPushButton *deleteBtn = new QPushButton("Eliminar");
    deleteBtn->setProperty("workId", work.id);
    deleteBtn->setMinimumHeight(30);
    deleteBtn->setCursor(Qt::PointingHandCursor);
    deleteBtn->setStyleSheet(
        "QPushButton { background-color: #FEF2F2; color: #B91C1C; border: none;"
        "  border-radius: 6px; font-size: 12px; font-weight: bold; padding: 4px 10px; }"
        "QPushButton:hover { background-color: #EF4444; color: white; }"
    );
    connect(deleteBtn, &QPushButton::clicked, this, &WorkBoardWidget::onDeleteWorkClicked);
    actionLayout->addWidget(deleteBtn);

    actionLayout->addStretch();
    gridLayout->addWidget(actionWidget, row, 5);
}

QString WorkBoardWidget::getWorkIdFromSender() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        return btn->property("workId").toString();
    }
    return QString();
}

void WorkBoardWidget::onAddWorkClicked() {
    WorkDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Work newWork = dialog.getWork();
        newWork.createdBy = currentUsername;
        QString workId = workManager->addWork(newWork.title, newWork.description,
                                              newWork.priority, newWork.dueDate,
                                              currentUsername);
        historyManager->addEntry(currentUsername, "Crear",
                                 "Trabajo creado: " + newWork.title, workId);
        refreshBoard();
        refreshHistoryView();
    }
}

void WorkBoardWidget::onEditWorkClicked() {
    QString workId = getWorkIdFromSender();
    Work work = workManager->getWorkById(workId);

    if (work.id.isEmpty()) {
        QMessageBox::warning(this, "Error", "Trabajo no encontrado");
        return;
    }

    WorkDialog dialog(work, this);
    if (dialog.exec() == QDialog::Accepted) {
        Work updatedWork = dialog.getWork();
        updatedWork.id = work.id;
        updatedWork.createdDate = work.createdDate;
        updatedWork.createdBy = work.createdBy;
        updatedWork.notes = work.notes;

        if (!workManager->updateWork(workId, updatedWork)) {
            QMessageBox::warning(this, "Error", "No se pudo actualizar el trabajo.");
            return;
        }

        historyManager->addEntry(currentUsername, "Editar",
                                 "Trabajo editado: " + updatedWork.title, workId);
        refreshBoard();
        refreshHistoryView();
    }
}

void WorkBoardWidget::onDeleteWorkClicked() {
    QString workId = getWorkIdFromSender();
    Work work = workManager->getWorkById(workId);

    if (work.id.isEmpty()) {
        QMessageBox::warning(this, "Error", "Trabajo no encontrado");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmar",
                                                              "Desea eliminar el trabajo '" + work.title + "'?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (!workManager->deleteWork(workId)) {
            QMessageBox::warning(this, "Error", "No se pudo eliminar el trabajo.");
            return;
        }

        historyManager->addEntry(currentUsername, "Eliminar",
                                 "Trabajo eliminado: " + work.title, workId);
        refreshBoard();
        refreshHistoryView();
    }
}

void WorkBoardWidget::onNotesClicked() {
    QString workId = getWorkIdFromSender();
    Work work = workManager->getWorkById(workId);

    if (work.id.isEmpty()) {
        QMessageBox::warning(this, "Error", "Trabajo no encontrado");
        return;
    }

    NoteEditor editor(work, currentUsername, this);
    editor.exec();
    refreshBoard();
    refreshHistoryView();
}

void WorkBoardWidget::onStatusClicked() {
    QString workId = getWorkIdFromSender();
    Work work = workManager->getWorkById(workId);

    if (work.id.isEmpty()) {
        QMessageBox::warning(this, "Error", "Trabajo no encontrado");
        return;
    }

    // Cambiar estado
    if (work.status == "Pendiente") {
        work.status = "En Progreso";
    } else if (work.status == "En Progreso") {
        work.status = "Completado";
    } else {
        work.status = "Pendiente";
    }

    if (!workManager->updateWork(workId, work)) {
        QMessageBox::warning(this, "Error", "No se pudo cambiar el estado.");
        return;
    }

    historyManager->addEntry(currentUsername, "Estado",
                             "Estado cambio a: " + work.status + " - " + work.title, workId);
    refreshBoard();
    refreshHistoryView();
}

void WorkBoardWidget::refreshHistoryView() {
    QList<HistoryEntry> allHistory = historyManager->getHistory();

    QString historyContent;
    int maxRows = 0;
    for (const auto& entry : allHistory) {
        historyContent += QString("[%1] %2 | %3 | %4\n")
        .arg(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(entry.username)
            .arg(entry.actionType)
            .arg(entry.description);

        ++maxRows;
        if (maxRows >= 50) {
            break;
        }
    }

    if (historyContent.isEmpty()) {
        historyContent = "No hay historial disponible";
    }

    historyView->setPlainText(historyContent);
}

void WorkBoardWidget::onLogoutClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmar",
                                                              "¿Desea cerrar sesión?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        emit logoutRequested();
    }
}