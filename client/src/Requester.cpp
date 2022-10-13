#include "Requester.h"
#include <QDebug>
#include <QMessageBox>

#include "Common.hpp"
#include "json.hpp"
#include "commonflags.h"

#include "HelloProcessor.h"
#include "ActiveOrdersProcessor.h"
#include "BalanceProcessor.h"
#include "HistoryProcessor.h"
#include "DealsProcessor.h"
#include "QuotationProcessor.h"
#include "ForGuiDeleteProcessor.h"
#include "ForGuiOrderProcessor.h"

#include "EventProcessorFactory.h"

void Requester::Run(){
    QMutexLocker locker(mutex);
    upds->clear();
    for (int i = 1; i <=6; i++){
        Processor *proc = main_factory->makeProcessor(std::to_string(i));
        proc->setId(userid);
        auto response = proc->process(*sock);
        upds->push_back(response);
        delete proc; 
    }

    emit updated(Flags::Success);
}

void Requester::run(){
    while(1){
        Run();
        msleep(2000);
    }

}

Requester::Requester(){
    main_factory = new EventProcessorFactory;
    main_factory->addProcessor<HelloProcessor>("1");
    main_factory->addProcessor<BalanceProcessor>("2");
    main_factory->addProcessor<ActiveOrdersProcessor>("3");
    main_factory->addProcessor<HistoryProcessor>("4");
    main_factory->addProcessor<DealsProcessor>("5");
    main_factory->addProcessor<QuotationProcessor>("6");
    main_factory->addProcessor<ForGuiDeleteProcessor>("delete");
    main_factory->addProcessor<ForGuiOrderProcessor>("order");
    
}

Requester::~Requester(){
    delete main_factory;
}

void Requester::setUserId(std::string id){
    userid = id;
}

void Requester::setSocket(boost::shared_ptr<boost::asio::ip::tcp::socket> socket){
    sock = socket;
}

void Requester::onDelete(int appid){
    Processor *proc = main_factory->makeProcessor("delete");
    proc->setId(userid);
    static_cast<ForGuiDeleteProcessor*>(proc)->setAppId(appid);
    auto resp = proc->process(*sock);
    if (resp != "Deleted successfully"){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("Error occured. \n Check Id!");
        msgBox.exec();
    }
}

void Requester::setMutex(QMutex* mutex_){
    mutex = mutex_;
}

void Requester::onPublish(double vol, double price, int dir){
    if (vol == 0 || price == 0){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("USD and Price cannot be 0\n and should contain only digits!");
        msgBox.exec();
        return; 
    }
    Processor *proc = main_factory->makeProcessor("order");
    proc->setId(userid);
    static_cast<ForGuiOrderProcessor*>(proc)->setVolPriceDir(
        std::to_string(vol),
        std::to_string(price),
        dir ? "purchase" : "sale"
    );
    auto reply = proc->process(*sock);
}