#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("../resources/Combinear.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();
    return a.exec();
}
