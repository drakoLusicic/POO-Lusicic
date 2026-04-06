#include "workdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QMessageBox>

WorkDialog::WorkDialog(QWidget *parent) : QDialog(parent) {
    setupUI();
    setWindowTitle("Nuevo Trabajo Práctico");
}

WorkDialog::WorkDialog(const Work& work, QWidget *parent) : QDialog(parent) {
    setupUI();
    loadWorkData(work);
    setWindowTitle("Editar Trabajo Práctico");
}

void WorkDialog::setupUI() {
    setStyleSheet(
        "QDialog { background-color: #FFFFFF; font-family: 'Segoe UI', Arial; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Header
    QWidget *header = new QWidget();
    header->setFixedHeight(56);
    header->setStyleSheet("background-color: #6366F1;");
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(20, 0, 20, 0);
    QLabel *headerTitle = new QLabel("Trabajo Práctico");
    headerTitle->setStyleSheet("color: white; font-size: 16px; font-weight: bold; background: transparent;");
    headerLayout->addWidget(headerTitle);
    mainLayout->addWidget(header);

    // Formulario
    QWidget *formWidget = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);
    formLayout->setSpacing(12);
    formLayout->setContentsMargins(24, 20, 24, 20);

    QString labelStyle = "color: #374151; font-size: 12px; font-weight: 600; background: transparent;";
    QString inputStyle =
        "QLineEdit, QTextEdit, QDateTimeEdit {"
        "  border: 1.5px solid #D1D5DB;"
        "  border-radius: 8px;"
        "  padding: 6px 10px;"
        "  font-size: 13px; color: #1E293B;"
        "  background-color: #F9FAFB;"
        "}"
        "QLineEdit:focus, QTextEdit:focus, QDateTimeEdit:focus {"
        "  border-color: #6366F1; background-color: #FFFFFF;"
        "}";
    QString comboStyle =
        "QComboBox {"
        "  border: 1.5px solid #D1D5DB; border-radius: 8px;"
        "  padding: 6px 10px; font-size: 13px;"
        "  color: #1E293B; background-color: #F9FAFB; min-width: 160px;"
        "}"
        "QComboBox:focus { border-color: #6366F1; }"
        "QComboBox::drop-down { border: none; }"
        "QComboBox QAbstractItemView {"
        "  border: 1px solid #E2E8F0;"
        "  selection-background-color: #EEF2FF; selection-color: #4F46E5;"
        "}";

    // Título
    QLabel *titleLabel = new QLabel("Título");
    titleLabel->setStyleSheet(labelStyle);
    titleEdit = new QLineEdit();
    titleEdit->setPlaceholderText("Ingrese título del trabajo");
    titleEdit->setMinimumHeight(38);
    titleEdit->setStyleSheet(inputStyle);
    formLayout->addWidget(titleLabel);
    formLayout->addWidget(titleEdit);

    // Descripción
    QLabel *descLabel = new QLabel("Descripción");
    descLabel->setStyleSheet(labelStyle);
    descriptionEdit = new QTextEdit();
    descriptionEdit->setPlaceholderText("Ingrese descripción del trabajo");
    descriptionEdit->setMinimumHeight(100);
    descriptionEdit->setStyleSheet(inputStyle);
    formLayout->addWidget(descLabel);
    formLayout->addWidget(descriptionEdit);

    // Estado + Prioridad en la misma fila
    QHBoxLayout *rowLayout = new QHBoxLayout();
    rowLayout->setSpacing(16);

    QVBoxLayout *statusCol = new QVBoxLayout();
    QLabel *statusLabel = new QLabel("Estado");
    statusLabel->setStyleSheet(labelStyle);
    statusCombo = new QComboBox();
    statusCombo->addItem("Pendiente");
    statusCombo->addItem("En Progreso");
    statusCombo->addItem("Completado");
    statusCombo->setStyleSheet(comboStyle);
    statusCol->addWidget(statusLabel);
    statusCol->addWidget(statusCombo);
    rowLayout->addLayout(statusCol);

    QVBoxLayout *prioCol = new QVBoxLayout();
    QLabel *priorityLabel = new QLabel("Prioridad");
    priorityLabel->setStyleSheet(labelStyle);
    priorityCombo = new QComboBox();
    priorityCombo->addItem("Baja");
    priorityCombo->addItem("Normal");
    priorityCombo->addItem("Alta");
    priorityCombo->setStyleSheet(comboStyle);
    prioCol->addWidget(priorityLabel);
    prioCol->addWidget(priorityCombo);
    rowLayout->addLayout(prioCol);
    rowLayout->addStretch();
    formLayout->addLayout(rowLayout);

    // Fecha
    QLabel *dateLabel = new QLabel("Fecha de Vencimiento");
    dateLabel->setStyleSheet(labelStyle);
    dueDateEdit = new QDateTimeEdit();
    dueDateEdit->setDateTime(QDateTime::currentDateTime().addDays(7));
    dueDateEdit->setDisplayFormat("yyyy-MM-dd hh:mm");
    dueDateEdit->setMinimumHeight(38);
    dueDateEdit->setStyleSheet(inputStyle);
    dueDateEdit->setCalendarPopup(true);
    formLayout->addWidget(dateLabel);
    formLayout->addWidget(dueDateEdit);

    mainLayout->addWidget(formWidget);

    // Footer con botones
    QWidget *footer = new QWidget();
    footer->setFixedHeight(64);
    footer->setStyleSheet("background-color: #F8FAFC; border-top: 1px solid #E2E8F0;");
    QHBoxLayout *buttonLayout = new QHBoxLayout(footer);
    buttonLayout->setContentsMargins(24, 0, 24, 0);
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancelar");
    cancelButton->setMinimumHeight(38);
    cancelButton->setMinimumWidth(100);
    cancelButton->setCursor(Qt::PointingHandCursor);
    cancelButton->setStyleSheet(
        "QPushButton { background-color: #FFFFFF; color: #64748B;"
        "  border: 1.5px solid #D1D5DB; border-radius: 8px;"
        "  font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { border-color: #6366F1; color: #6366F1; }"
    );
    buttonLayout->addWidget(cancelButton);

    saveButton = new QPushButton("Guardar");
    saveButton->setMinimumHeight(38);
    saveButton->setMinimumWidth(100);
    saveButton->setCursor(Qt::PointingHandCursor);
    saveButton->setStyleSheet(
        "QPushButton { background-color: #6366F1; color: white;"
        "  border: none; border-radius: 8px;"
        "  font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #4F46E5; }"
    );
    buttonLayout->addWidget(saveButton);
    mainLayout->addWidget(footer);

    connect(saveButton, &QPushButton::clicked, this, &WorkDialog::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(mainLayout);
    resize(560, 520);
}

void WorkDialog::onSaveClicked() {
    if (titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validacion", "El titulo no puede estar vacio.");
        return;
    }

    if (descriptionEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validacion", "La descripcion no puede estar vacia.");
        return;
    }

    accept();
}

void WorkDialog::loadWorkData(const Work& work) {
    titleEdit->setText(work.title);
    descriptionEdit->setText(work.description);
    statusCombo->setCurrentText(work.status);
    priorityCombo->setCurrentText(work.priority);
    dueDateEdit->setDateTime(work.dueDate);
}

Work WorkDialog::getWork() const {
    Work work;
    work.title = titleEdit->text();
    work.description = descriptionEdit->toPlainText();
    work.status = statusCombo->currentText();
    work.priority = priorityCombo->currentText();
    work.dueDate = dueDateEdit->dateTime();
    return work;
}