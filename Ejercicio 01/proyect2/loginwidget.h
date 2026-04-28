#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "authmanager.h"

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString& username);

private slots:
    void onLoginClicked();
    void onPasswordEnterPressed();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QLabel *errorLabel;
    QLabel *titleLabel;

    AuthManager *authManager;

    void setupUI();
};

#endif // LOGINWIDGET_H