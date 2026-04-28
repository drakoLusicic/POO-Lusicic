#include "mainwidget.h"
#include "loginwidget.h"
#include "workboardwidget.h"
#include <QVBoxLayout>
#include <QStackedWidget>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    authManager = new AuthManager();
    loginWidget = new LoginWidget(this);
    workBoardWidget = nullptr;
    stackedWidget = new QStackedWidget(this);

    connect(loginWidget, &LoginWidget::loginSuccess,
            this, &MainWidget::onLoginSuccess);

    stackedWidget->addWidget(loginWidget);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    setWindowTitle("Planificador de Trabajos Prácticos");
    resize(1100, 700);

    checkActiveSession();
}

void MainWidget::checkActiveSession() {
    QString username = authManager->getActiveSession();

    if (!username.isEmpty()) {
        onLoginSuccess(username);
    }
}

void MainWidget::onLoginSuccess(const QString& username) {
    if (workBoardWidget != nullptr) {
        stackedWidget->removeWidget(workBoardWidget);
        delete workBoardWidget;
        workBoardWidget = nullptr;
    }

    workBoardWidget = new WorkBoardWidget(username, this);
    stackedWidget->addWidget(workBoardWidget);
        connect(workBoardWidget, &WorkBoardWidget::logoutRequested,
                this, &MainWidget::onLogout);

    stackedWidget->setCurrentWidget(workBoardWidget);
}

void MainWidget::onLogout() {
    authManager->logout();
    stackedWidget->setCurrentWidget(loginWidget);
}