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
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QGroupBox *groupBoxCredentials;
    QFormLayout *formLayout;
    QLabel *labelUsername;
    QLineEdit *lineUsername;
    QLabel *labelPassword;
    QLineEdit *linePassword;
    QLabel *labelMessage;
    QPushButton *btnLogin;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(420, 260);
        verticalLayout = new QVBoxLayout(LoginDialog);
        verticalLayout->setObjectName("verticalLayout");
        labelTitle = new QLabel(LoginDialog);
        labelTitle->setObjectName("labelTitle");
        labelTitle->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        labelTitle->setFont(font);

        verticalLayout->addWidget(labelTitle);

        groupBoxCredentials = new QGroupBox(LoginDialog);
        groupBoxCredentials->setObjectName("groupBoxCredentials");
        formLayout = new QFormLayout(groupBoxCredentials);
        formLayout->setObjectName("formLayout");
        labelUsername = new QLabel(groupBoxCredentials);
        labelUsername->setObjectName("labelUsername");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelUsername);

        lineUsername = new QLineEdit(groupBoxCredentials);
        lineUsername->setObjectName("lineUsername");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lineUsername);

        labelPassword = new QLabel(groupBoxCredentials);
        labelPassword->setObjectName("labelPassword");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, labelPassword);

        linePassword = new QLineEdit(groupBoxCredentials);
        linePassword->setObjectName("linePassword");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, linePassword);


        verticalLayout->addWidget(groupBoxCredentials);

        labelMessage = new QLabel(LoginDialog);
        labelMessage->setObjectName("labelMessage");
        labelMessage->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelMessage);

        btnLogin = new QPushButton(LoginDialog);
        btnLogin->setObjectName("btnLogin");

        verticalLayout->addWidget(btnLogin);


        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Login", nullptr));
        labelTitle->setText(QCoreApplication::translate("LoginDialog", "Acceso al sistema", nullptr));
        groupBoxCredentials->setTitle(QCoreApplication::translate("LoginDialog", "Usuario", nullptr));
        labelUsername->setText(QCoreApplication::translate("LoginDialog", "Usuario:", nullptr));
        labelPassword->setText(QCoreApplication::translate("LoginDialog", "Contrase\303\261a:", nullptr));
        labelMessage->setText(QCoreApplication::translate("LoginDialog", "Ingrese sus credenciales para continuar.", nullptr));
        btnLogin->setText(QCoreApplication::translate("LoginDialog", "Iniciar sesi\303\263n", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
