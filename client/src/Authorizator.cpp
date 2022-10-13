#include "Authorizator.h"

#include "EventProcessorFactory.h"
#include "ForGuiLoginProcessor.h"
#include "ForGuiRegistrationProcessor.h"

#include "commonflags.h"
#include <tuple>
#include <QMessageBox>


Authorizator::Authorizator(){
    factory = new EventProcessorFactory;
    factory->addProcessor<ForGuiLoginProcessor>("login");
    factory->addProcessor<ForGuiRegistrationProcessor>("registration");
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
            if (id == "null"){
                QMessageBox msgBox;
                msgBox.setWindowTitle("Error");
                msgBox.setText("Error occured. \n Invalid login and password!");
                msgBox.exec();
            }
            break;
        }
        case Flags::Registration:{
            regDialog = new RegistrationDialog;
            auto status = regDialog->exec();
            auto [firstName, lastName, login, password] = regDialog->getFLLP();
            auto proc = factory->makeProcessor("registration");
            static_cast<ForGuiRegistrationProcessor*>(proc)->setFLLP(firstName, lastName, login, password);
            auto reply = proc->process(*sock);
            delete proc;
            delete regDialog;
            id = reply;
            if (id == "null" && status != QDialog::Rejected){
                QMessageBox msgBox;
                msgBox.setWindowTitle("Error");
                msgBox.setText("Error occured. \n Unavailable login!");
                msgBox.exec();
            }
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