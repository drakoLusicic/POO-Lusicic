#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include "workmanager.h"
#include "historymanager.h"

class NoteEditor : public QDialog {
    Q_OBJECT

public:
    explicit NoteEditor(const Work& work, const QString& username, QWidget *parent = nullptr);

private slots:
    void onSaveClicked();

private:
    QTextEdit *notesEdit;
    QPushButton *saveButton;
    QPushButton *closeButton;

    WorkManager *workManager;
    HistoryManager *historyManager;

    Work currentWork;
    QString currentUsername;

    void setupUI();
};

#endif // NOTEEDITOR_H