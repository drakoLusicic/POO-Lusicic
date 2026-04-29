/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QHBoxLayout *mainContentLayout;
    QGroupBox *loginGroup;
    QVBoxLayout *loginFormLayout;
    QLabel *userLabel;
    QLineEdit *usernameEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QLabel *statusLabel;
    QGroupBox *infoGroup;
    QVBoxLayout *infoLayout;
    QLabel *imagePreviewLabel;
    QLabel *weatherLabel;
    QLabel *timeLabel;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(980, 640);
        verticalLayout = new QVBoxLayout(Login);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(Login);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        mainContentLayout = new QHBoxLayout();
        mainContentLayout->setObjectName("mainContentLayout");
        loginGroup = new QGroupBox(Login);
        loginGroup->setObjectName("loginGroup");
        loginFormLayout = new QVBoxLayout(loginGroup);
        loginFormLayout->setObjectName("loginFormLayout");
        userLabel = new QLabel(loginGroup);
        userLabel->setObjectName("userLabel");

        loginFormLayout->addWidget(userLabel);

        usernameEdit = new QLineEdit(loginGroup);
        usernameEdit->setObjectName("usernameEdit");

        loginFormLayout->addWidget(usernameEdit);

        passwordLabel = new QLabel(loginGroup);
        passwordLabel->setObjectName("passwordLabel");

        loginFormLayout->addWidget(passwordLabel);

        passwordEdit = new QLineEdit(loginGroup);
        passwordEdit->setObjectName("passwordEdit");

        loginFormLayout->addWidget(passwordEdit);

        loginButton = new QPushButton(loginGroup);
        loginButton->setObjectName("loginButton");

        loginFormLayout->addWidget(loginButton);

        statusLabel = new QLabel(loginGroup);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setAlignment(Qt::AlignCenter);

        loginFormLayout->addWidget(statusLabel);


        mainContentLayout->addWidget(loginGroup);

        infoGroup = new QGroupBox(Login);
        infoGroup->setObjectName("infoGroup");
        infoLayout = new QVBoxLayout(infoGroup);
        infoLayout->setObjectName("infoLayout");
        imagePreviewLabel = new QLabel(infoGroup);
        imagePreviewLabel->setObjectName("imagePreviewLabel");
        imagePreviewLabel->setMinimumSize(QSize(320, 260));
        imagePreviewLabel->setAlignment(Qt::AlignCenter);

        infoLayout->addWidget(imagePreviewLabel);

        weatherLabel = new QLabel(infoGroup);
        weatherLabel->setObjectName("weatherLabel");

        infoLayout->addWidget(weatherLabel);

        timeLabel = new QLabel(infoGroup);
        timeLabel->setObjectName("timeLabel");

        infoLayout->addWidget(timeLabel);


        mainContentLayout->addWidget(infoGroup);


        verticalLayout->addLayout(mainContentLayout);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Login Dark Tech", nullptr));
        titleLabel->setText(QCoreApplication::translate("Login", "Dark Tech Access", nullptr));
        loginGroup->setTitle(QCoreApplication::translate("Login", "Credenciales", nullptr));
        userLabel->setText(QCoreApplication::translate("Login", "Usuario", nullptr));
        passwordLabel->setText(QCoreApplication::translate("Login", "Contrase\303\261a", nullptr));
        loginButton->setText(QCoreApplication::translate("Login", "Ingresar", nullptr));
        statusLabel->setText(QCoreApplication::translate("Login", "Listo para acceso.", nullptr));
        infoGroup->setTitle(QCoreApplication::translate("Login", "Estado de sesi\303\263n", nullptr));
        weatherLabel->setText(QCoreApplication::translate("Login", "Clima cargando...", nullptr));
        timeLabel->setText(QCoreApplication::translate("Login", "Hora local", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
