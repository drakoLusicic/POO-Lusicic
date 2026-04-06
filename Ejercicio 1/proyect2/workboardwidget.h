#ifndef WORKBOARDWIDGET_H
#define WORKBOARDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include "workmanager.h"
#include "historymanager.h"

class WorkBoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit WorkBoardWidget(const QString& username, QWidget *parent = nullptr);

signals:
    void logoutRequested();

private slots:
    void refreshBoard();
    void onFilterChanged();
    void onAddWorkClicked();
    void onEditWorkClicked();
    void onDeleteWorkClicked();
    void onNotesClicked();
    void onStatusClicked();
    void onLogoutClicked();

private:
    QString currentUsername;

    WorkManager *workManager;
    HistoryManager *historyManager;

    QGridLayout *gridLayout;
    QComboBox *statusFilter;
    QComboBox *priorityFilter;
    QPushButton *addButton;
    QPushButton *logoutButton;
    QLabel *userLabel;
    QWidget *gridWidget;
    QTextEdit *historyView;

    QList<Work> displayedWorks;

    void setupUI();
    void rebuildGridHeaders();
    void clearGridRows();
    void addWorkRow(const Work& work, int row);
    QString getWorkIdFromSender();
    void refreshHistoryView();
};

#endif // WORKBOARDWIDGET_H