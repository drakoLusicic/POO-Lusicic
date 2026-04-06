#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "authmanager.h"

class LoginWidget;
class WorkBoardWidget;
class QStackedWidget;

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);

private slots:
    void onLoginSuccess(const QString& username);
    void onLogout();

private:
    AuthManager *authManager;
    LoginWidget *loginWidget;
    WorkBoardWidget *workBoardWidget;
    QStackedWidget *stackedWidget;

    void checkActiveSession();
};

#endif // MAINWIDGET_H