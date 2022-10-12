#include "Authorizator.h"

#include "EventProcessorFactory.h"
#include "ForGuiLoginProcessor.h"
#include "RegistrationProcessor.h"


Authorizator::Authorizator(){
    factory = new EventProcessorFactory;
    factory->addProcessor<ForGuiLoginProcessor>("login");
    //factory->addProcessor<ForGuiRegistrationProcessor>("registration");
}

Authorizator::~Authorizator(){
    delete factory;
}


std::string Authorizator::authorize(){
    std::string id = "null";
    while(id == "null"){
        id = waitForAuth();
    }
    return id;
}

std::string Authorizator::waitForAuth(){
    std::string id = "null";
    logDialog = new LogInDialog;
    switch(logDialog->exec()){
        case QDialog::Rejected:{
            delete logDialog;
            exit(0);
            break;
        }
        case QDialog::Accepted:{
            logDialog->done(0);
            auto [login, password] = logDialog->getLP();
            auto proc = factory->makeProcessor("login");
            static_cast<ForGuiLoginProcessor*>(proc)->setLP(login, password);
            auto reply = proc->process(*sock);
            delete proc;
            id = reply;
            break;
        }
        case 20:{
            regDialog = new RegistrationDialog;
            regDialog->exec();
            delete regDialog;
            break;
        }
        default:
            break;
    }
    delete logDialog;

    return id;
}

void Authorizator::setSocket(boost::shared_ptr<boost::asio::ip::tcp::socket> socket){
    sock = socket;
}