#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onDelete);

    while(authorize() == "");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeDialog(){
    qDebug() << "rejected";
}

std::string MainWindow::authorize(){
    std::string login = "";

    logDialog = new LogInDialog;
    switch(logDialog->exec()){
        case QDialog::Rejected:
            delete logDialog;
            exit(0);
            break;
        case QDialog::Accepted:
            logDialog->done(0);
            login = "qwerty";
            break;
        case 20:
            regDialog = new RegistrationDialog;
            regDialog->exec();
            delete regDialog;
            break;
        default:
            break;
    }
    delete logDialog;

    return login;
}

void MainWindow::registrate(){
    qDebug() << "registraaate!";
}

void MainWindow::onDelete(){
    delDialog = new DeleteDialog;
    delDialog->exec();
    delete delDialog;
}