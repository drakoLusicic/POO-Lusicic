#include "logindialog.h"
#include "ui_login.h"
#include "mainwindow.h"

LoginDialog::LoginDialog(MainWindow *mainWindow, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::LoginDialog),
      m_mainWindow(mainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Iniciar sesión"));
    ui->linePassword->setEchoMode(QLineEdit::Password);
    ui->labelMessage->clear();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnLogin_clicked()
{
    const QString username = ui->lineUsername->text().trimmed();
    const QString password = ui->linePassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->labelMessage->setText(tr("Complete usuario y contraseña."));
        return;
    }

    const bool success = m_mainWindow->validateUser(username, password);
    m_mainWindow->logAccess(username, success);
    if (success) {
        accept();
    } else {
        ui->labelMessage->setText(tr("Usuario o contraseña incorrectos."));
    }
}
