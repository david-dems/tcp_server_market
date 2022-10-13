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


    QApplication a(argc, argv);

    QFile file("../client/resources/Combinear.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    a.setStyleSheet(styleSheet);
    
    //Authorization here
    //If user close window or reject login dialog
    //Auth will call exit(0)
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
    QObject::connect(&r, &Requester::updated, &w, &MainWindow::onUpdate); // connect main window updates to requester
    QObject::connect(&w, &MainWindow::deleteSignal, &r, &Requester::onDelete); //connect deletions requests
    QObject::connect(&w, &MainWindow::publishSignal, &r, &Requester::onPublish); //connect publish orders requests


    //upds --- shared memory, that contains updates from requester
    //requester make requests to to server to update userinfo
    //upds protected by mutex
    std::vector<std::string> upds;
    w.upds = &upds;
    r.upds = &upds;
    r.start();

    w.show();
    auto res = a.exec();
    
    r.terminate();
    r.wait();
    
    return res;
}
