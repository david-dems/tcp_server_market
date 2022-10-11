#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>

LogInDialog::LogInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInDialog)
{
    ui->setupUi(this);
    connect(this->ui->buttonBox, &QDialogButtonBox::rejected, this, &LogInDialog::onCancel);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &LogInDialog::onOk);
    connect(this->ui->createAccaunt, &QPushButton::clicked, this, &LogInDialog::startRegistration);
}

LogInDialog::~LogInDialog()
{
    delete ui;
}

void LogInDialog::onCancel(){
    emit QDialog::reject();
}

void LogInDialog::onOk(){
    qDebug() << ui->login->text();
    qDebug() << ui->password->text();
    emit accept();
}

void LogInDialog::startRegistration(){
    emit done(20);
}
