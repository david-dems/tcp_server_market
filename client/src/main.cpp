#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QString>
#include <QObject>
#include <boost/asio.hpp>
#include <QSharedMemory>
#include <QMutex>

#include "Requester.h"
#include "Authorizator.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", "5556");
    tcp::resolver::iterator iterator = resolver.resolve(query);
    boost::shared_ptr<boost::asio::ip::tcp::socket> sock (new boost::asio::ip::tcp::socket(io_service));
    sock->connect(*iterator);

    std::vector<std::string> upds;

    QApplication a(argc, argv);

    QFile file("../client/resources/Combinear.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    a.setStyleSheet(styleSheet);
    
    Authorizator Auth;
    Auth.setSocket(sock);
    auto id = Auth.authorize();
    
    QMutex mutex;

    Requester r;
    r.setMutex(&mutex);
    r.setUserId(id);
    r.setSocket(sock);


    MainWindow w;
    w.setMutex(&mutex);
    QObject::connect(&r, &Requester::updated, &w, &MainWindow::onUpdate);
    QObject::connect(&w, &MainWindow::deleteSignal, &r, &Requester::onDelete);
    QObject::connect(&w, &MainWindow::publishSignal, &r, &Requester::onPublish);

    w.upds = &upds;
    r.upds = &upds;
    r.start();

    w.show();
    a.exec();
    
    r.terminate();
    r.wait();
    
    return 0;
}
