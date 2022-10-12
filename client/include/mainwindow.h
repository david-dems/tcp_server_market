#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registrationdialog.h"
#include "deletedialog.h"
#include <string>
#include <QSharedMemory>
#include <QGraphicsScene>
#include <QMutexLocker>
#include <QMutex> 

#include "commonflags.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::vector<std::string> *upds;    
    void setMutex(QMutex* mutex_);

private:
    Ui::MainWindow *ui;

    DeleteDialog *delDialog;

    QGraphicsScene *order_scene, *deals_scene;

    QMutex *mutex;


public slots:
    void onDelete();
    void onUpdate(int val);
    void onPublish();
signals:
    void deleteSignal(int);
    void publishSignal(int, int, int);

};
#endif // MAINWINDOW_H
