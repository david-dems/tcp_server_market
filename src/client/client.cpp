#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <exception>
#include <regex>
#include <string_view>

#include "Common.hpp"
#include "json.hpp"

#include "EventProcessorFactory.h"
#include "LoginProcessor.h"
#include "RegistrationProcessor.h"
#include "ExitProcessor.h"
#include "MakeOrderProcessor.h"
#include "HelloProcessor.h"
#include "ActiveOrdersProcessor.h"
#include "BalanceProcessor.h"
#include "HistoryProcessor.h"
#include "DealsProcessor.h"
#include "DeleteProcessor.h"
#include "QuotationProcessor.h"

using boost::asio::ip::tcp;

int main(){

    try{
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        EventProcessorFactory signin_factory;
        signin_factory.addProcessor<LoginProcessor>("1");
        signin_factory.addProcessor<RegistrationProcessor>("2");
        signin_factory.addProcessor<ExitProcessor>("3");

        std::string my_id = "null";
        
        static const std::regex r(R"(\d)");

        while (my_id == "null"){
            std::cout << "Menu:\n"
                         "1) Login\n"
                         "2) Registration\n"
                         "3) Exit\n"
                        << std::endl;

            std::string menu_option;
            std::cin >> menu_option;
            
            if (!regex_search(menu_option, r)){
                std::cout << "Bad input" << std::endl;
                continue;
            }
            
            Processor* proc = signin_factory.makeProcessor(menu_option);
            if (proc){
                my_id = proc->process(s);
                delete proc;
            }else{
                std::cout << "Invalid option!" << std::endl;
            }
        }


        EventProcessorFactory main_factory;
        main_factory.addProcessor<HelloProcessor>("1");
        main_factory.addProcessor<MakeOrderProcessor>("2");
        main_factory.addProcessor<BalanceProcessor>("3");
        main_factory.addProcessor<ActiveOrdersProcessor>("4");
        main_factory.addProcessor<HistoryProcessor>("5");
        main_factory.addProcessor<DealsProcessor>("6");
        main_factory.addProcessor<DeleteProcessor>("7");
        main_factory.addProcessor<QuotationProcessor>("8");
        main_factory.addProcessor<ExitProcessor>("9");

        while (true)
        {
            // Тут реализовано "бесконечное" меню.
            std::cout << "Menu:\n"
                         "1) Hello Request\n"
                         "2) Make Order\n"
                         "3) Show balance\n"
                         "4) Show active orders\n"
                         "5) Show closed orders\n"
                         "6) Show deals\n"
                         "7) Delete order\n"
                         "8) Quotations\n"
                         "9) Exit\n"
                         << std::endl;

            std::string menu_option;
            std::cin >> menu_option;
            
            if (!regex_search(menu_option, r)){
                std::cout << "Bad input" << std::endl;
                continue;
            }
            
            Processor* proc = main_factory.makeProcessor(menu_option);
            if (proc){
                proc->setId(my_id);
                std::string reply;
                reply = proc->process(s);
                delete proc;
                std::cout << reply << std::endl;
            } else {
                std::cout << "Invalid option" << std :: endl;
            }
        }

    } 
    catch(std::exception const& e){
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

