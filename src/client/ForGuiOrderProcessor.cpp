#include "ForGuiOrderProcessor.h"

std::string ForGuiOrderProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::Order;
    req["Vol"] = vol;
    req["Price"] = price;
    req["Direction"] = dir;
                
    try{
        SendMessage(s, req);
        auto resp = ReadMessage(s);
        return std::move(resp["Message"].get<std::string>());
    } catch(std::exception const& ex){
        std::cerr << ex.what() << std::endl;
        return "null";
    }
}

void ForGuiOrderProcessor::setVolPriceDir(std::string vol_, std::string price_, std::string dir_){
    vol = vol_;
    price = price_;
    dir = dir_;
}