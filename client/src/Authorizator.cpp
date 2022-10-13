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
                msgBox.setText("Error occured. \nInvalid login and password!");
                msgBox.exec();
            }
            break;
        }
        case Flags::Registration:{
            regDialog = new RegistrationDialog;
            if (regDialog->exec() != QDialog::Accepted){
                delete regDialog;
                break;
            }

            auto [firstName, lastName, login, password] = regDialog->getFLLP();
            if (
                firstName.size() == 0,
                lastName.size() == 0,
                login.size() == 0,
                password.size() == 0
            ){
                QMessageBox msgBox;
                msgBox.setWindowTitle("Error");
                msgBox.setText("Error occured. \nFields cannot be empty!");
                msgBox.exec(); 
                delete regDialog;
                break;
            }

            auto proc = factory->makeProcessor("registration");
            static_cast<ForGuiRegistrationProcessor*>(proc)->setFLLP(firstName, lastName, login, password);
            auto reply = proc->process(*sock);
            delete proc;
            delete regDialog;
            id = reply;
            if (id == "null"){
                QMessageBox msgBox;
                msgBox.setWindowTitle("Error");
                msgBox.setText("Error occured. \nUnavailable login!");
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