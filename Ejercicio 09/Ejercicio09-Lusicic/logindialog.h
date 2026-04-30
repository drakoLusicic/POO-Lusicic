#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class MainWindow;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_btnLogin_clicked();

private:
    Ui::LoginDialog *ui;
    MainWindow *m_mainWindow;
};

#endif // LOGINDIALOG_H
