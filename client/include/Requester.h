#pragma once
#include <QThread>
#include <QSharedMemory>
#include <QMutex>
#include <QMutexLocker>

#include <string>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include <exception>
#include <regex>
#include <string_view>
#include "commonflags.h"


using boost::asio::ip::tcp;

class EventProcessorFactory;

class Requester : public QThread{
    Q_OBJECT
public:
    Requester();
    ~Requester();

    void setUserId(std::string id);
    void setSocket(boost::shared_ptr<boost::asio::ip::tcp::socket> socket);
    std::vector<std::string> *upds;

    void setMutex(QMutex* mutex_);    
    
    static const int Success = 1;    
signals:
    void updated(int);
private:
    void Run();
    void run();    
    boost::shared_ptr<boost::asio::ip::tcp::socket> sock;
    EventProcessorFactory *main_factory;
    std::string userid;
    QMutex *mutex;
public slots:
    void onDelete(int appid);
    void onPublish(int vol, int price, int dir);


};