#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registrationdialog.h"
#include "deletedialog.h"
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    LogInDialog *logDialog;
    RegistrationDialog *regDialog;
    DeleteDialog *delDialog;

    std::string authorize();


public slots:
    void closeDialog();
    void registrate();
    void onDelete();
};
#endif // MAINWINDOW_H
