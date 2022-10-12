#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <tuple>

namespace Ui {
class LogInDialog;
}

class LogInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogInDialog(QWidget *parent = nullptr);
    ~LogInDialog();
    std::tuple<std::string, std::string> getLP();


private:
    Ui::LogInDialog *ui;
public slots:
    void onCancel();
    void onOk();
    void startRegistration();

signals:
    void registration();
};

#endif // LOGINDIALOG_H
