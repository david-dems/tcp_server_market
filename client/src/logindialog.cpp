#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>

#include "commonflags.h"

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
    emit accept();
}

void LogInDialog::startRegistration(){
    emit done(Flags::Registration);
}

std::tuple<std::string, std::string> LogInDialog::getLP(){
    return {ui->login->text().toStdString(), ui->password->text().toStdString()};
}