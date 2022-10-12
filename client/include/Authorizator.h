#pragma once

#include <QObject>
#include <boost/asio.hpp>
#include <string>

#include "logindialog.h"
#include "registrationdialog.h"

class EventProcessorFactory;

class Authorizator : public QObject{
    Q_OBJECT
public:
    Authorizator();
    ~Authorizator();
    std::string authorize();
    void setSocket(boost::shared_ptr<boost::asio::ip::tcp::socket> socket);
private:
    std::string waitForAuth();
    LogInDialog *logDialog;
    RegistrationDialog *regDialog;
    boost::shared_ptr<boost::asio::ip::tcp::socket> sock;

    EventProcessorFactory *factory;
};