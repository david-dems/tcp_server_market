#include "registrationdialog.h"
#include "ui_registrationdialog.h"
#include <QDebug>

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &RegistrationDialog::onSignIn);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &RegistrationDialog::onOk);
}

RegistrationDialog::~RegistrationDialog()
{
    delete ui;
}

void RegistrationDialog::onSignIn(){
    emit QDialog::reject();
}

void RegistrationDialog::onOk(){
    emit done(QDialog::Accepted);
}

std::string RegistrationDialog::getUserId(){
    return usrId;
}

std::tuple<
        std::string,
        std::string,
        std::string,
        std::string> RegistrationDialog::getFLLP(){
        return {
            ui->firstName->text().toStdString(),
            ui->lastName->text().toStdString(),
            ui->login->text().toStdString(),
            ui->password->text().toStdString(),
        };
    }