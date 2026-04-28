#ifndef WORKDIALOG_H
#define WORKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include "workmanager.h"

class WorkDialog : public QDialog {
    Q_OBJECT

public:
    explicit WorkDialog(QWidget *parent = nullptr);
    explicit WorkDialog(const Work& work, QWidget *parent = nullptr);

    Work getWork() const;

private:
    QLineEdit *titleEdit;
    QTextEdit *descriptionEdit;
    QComboBox *statusCombo;
    QComboBox *priorityCombo;
    QDateTimeEdit *dueDateEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    void onSaveClicked();
    void setupUI();
    void loadWorkData(const Work& work);
};

#endif // WORKDIALOG_H