#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <iostream>
#include <QBuffer>
#include <QDataStream>
#include <QString>
#include <QFont>
#include <QGraphicsTextItem>
#include <cstdlib>
#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    connect(ui->delete_2, &QPushButton::clicked, this, &MainWindow::onDelete);
    connect(ui->publish,  &QPushButton::clicked, this, &MainWindow::onPublish);

    order_scene = new QGraphicsScene;
    deals_scene = new QGraphicsScene;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete order_scene;
    delete deals_scene;
}

//when user push delete order
void MainWindow::onDelete(){
    delDialog = new DeleteDialog;
    switch (delDialog->exec()){
        case QDialog::Accepted:{
            int appid = delDialog->getAppId();
            emit deleteSignal(appid);
            break;
        }
        default:{
            break;
        }
    }
    delete delDialog;
}

//when requester emits update signal
void MainWindow::onUpdate(int val){
    if (val != Flags::Success)
        return;

    QMutexLocker locker(mutex);

    //update user balance
    ui->balance->setText(QString::fromStdString((*upds)[1]));

    QFont font;
    font.setPixelSize(20);
    font.setBold(false);
    font.setFamily("Times");

    //update user active orders and completed deals
    order_scene->clear();
    deals_scene->clear();
    order_scene->addText(QString::fromStdString((*upds)[2]), font)->setDefaultTextColor(Qt::white);
    deals_scene->addText(QString::fromStdString((*upds)[4]), font)->setDefaultTextColor(Qt::white);
    ui->orderList->setScene(order_scene);
    ui->dealsList->setScene(deals_scene);
    ui->orderList->show();
    ui->dealsList->show();

    //update quotations
    auto q = std::stod((*upds)[5]);
    q = round(q *= 100) / 100;
    ui->quotation->setText(QString::number(q));
}


void MainWindow::setMutex(QMutex* mutex_){
    mutex = mutex_;
}


//when user push publish order
void MainWindow::onPublish(){
    emit publishSignal(ui->usd->text().toDouble(), ui->rub->text().toDouble(), ui->dir->currentIndex());
}