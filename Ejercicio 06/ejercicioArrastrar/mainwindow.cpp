#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->useredit->setFocus();
    this->setWindowTitle("Login");
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->passwordedit, SIGNAL(returnedPressed), this, SLOT(on_pushButton_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void::MainWindow::on_pushButton_clicked(){
    if (ui->useredit->text() == "admin" && ui->passwordedit->text() == "1234"){
        this->close();
    }
    else {
        ui->passwordedit->clear();
    }
}
