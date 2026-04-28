#include "noteeditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QMessageBox>

NoteEditor::NoteEditor(const Work& work, const QString& username, QWidget *parent)
    : QDialog(parent), currentWork(work), currentUsername(username) {

    workManager = new WorkManager();
    historyManager = new HistoryManager();

    setupUI();
    notesEdit->setText(currentWork.notes);

    setWindowTitle("Editor de Notas - " + work.title);
    resize(680, 500);
    setStyleSheet("QDialog { background-color: #FFFFFF; font-family: 'Segoe UI', Arial; }");
}

void NoteEditor::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Header
    QWidget *header = new QWidget();
    header->setFixedHeight(56);
    header->setStyleSheet("background-color: #0EA5E9;");
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(20, 0, 20, 0);
    QLabel *headerIcon = new QLabel("Notas");
    headerIcon->setStyleSheet("color: white; font-size: 16px; font-weight: bold; background: transparent;");
    headerLayout->addWidget(headerIcon);
    QLabel *headerSub = new QLabel("  \'" + currentWork.title + "\'");
    headerSub->setStyleSheet("color: #BAE6FD; font-size: 13px; background: transparent;");
    headerLayout->addWidget(headerSub);
    headerLayout->addStretch();
    mainLayout->addWidget(header);

    // Editor
    QWidget *editorWidget = new QWidget();
    QVBoxLayout *editorLayout = new QVBoxLayout(editorWidget);
    editorLayout->setContentsMargins(20, 16, 20, 16);
    editorLayout->setSpacing(8);

    QLabel *editorLabel = new QLabel("Contenido de las notas:");
    editorLabel->setStyleSheet("color: #374151; font-size: 12px; font-weight: 600; background: transparent;");
    editorLayout->addWidget(editorLabel);

    notesEdit = new QTextEdit();
    notesEdit->setPlaceholderText("Escriba sus notas aquí...");
    notesEdit->setStyleSheet(
        "QTextEdit {"
        "  border: 1.5px solid #D1D5DB;"
        "  border-radius: 10px;"
        "  padding: 10px;"
        "  font-size: 13px;"
        "  color: #1E293B;"
        "  background-color: #F9FAFB;"
        "  line-height: 1.5;"
        "}"
        "QTextEdit:focus {"
        "  border-color: #0EA5E9;"
        "  background-color: #FFFFFF;"
        "}"
    );
    editorLayout->addWidget(notesEdit);
    mainLayout->addWidget(editorWidget);

    // Footer
    QWidget *footer = new QWidget();
    footer->setFixedHeight(64);
    footer->setStyleSheet("background-color: #F0F9FF; border-top: 1px solid #BAE6FD;");
    QHBoxLayout *buttonLayout = new QHBoxLayout(footer);
    buttonLayout->setContentsMargins(20, 0, 20, 0);
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();

    closeButton = new QPushButton("Cerrar");
    closeButton->setMinimumHeight(38);
    closeButton->setMinimumWidth(90);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(
        "QPushButton { background-color: #FFFFFF; color: #64748B;"
        "  border: 1.5px solid #D1D5DB; border-radius: 8px;"
        "  font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { border-color: #0EA5E9; color: #0EA5E9; }"
    );
    buttonLayout->addWidget(closeButton);

    saveButton = new QPushButton("Guardar Notas");
    saveButton->setMinimumHeight(38);
    saveButton->setMinimumWidth(130);
    saveButton->setCursor(Qt::PointingHandCursor);
    saveButton->setStyleSheet(
        "QPushButton { background-color: #0EA5E9; color: white;"
        "  border: none; border-radius: 8px;"
        "  font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #0284C7; }"
    );
    buttonLayout->addWidget(saveButton);
    mainLayout->addWidget(footer);

    connect(saveButton, &QPushButton::clicked, this, &NoteEditor::onSaveClicked);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(mainLayout);
}

void NoteEditor::onSaveClicked() {
    QString notes = notesEdit->toPlainText();
    if (!workManager->updateWorkNotes(currentWork.id, notes)) {
        QMessageBox::warning(this, "Error", "No se pudieron guardar las notas.");
        return;
    }

    historyManager->addEntry(currentUsername, "Notas",
                             "Notas actualizadas para: " + currentWork.title,
                             currentWork.id);
    accept();
}