#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class RegistrationDialog;
}

class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);
    ~RegistrationDialog();

    std::string getUserId();

private:
    Ui::RegistrationDialog *ui;
    std::string usrId;
public slots:
    void onSignIn();
    void onOk();
};

#endif // REGISTRATIONDIALOG_H
