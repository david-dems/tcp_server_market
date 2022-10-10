#include "MakeOrderProcessor.h"

std::string MakeOrderProcessor::process(tcp::socket& s){
    std::string vol, price, dir;

    std::cin >> vol >> price >> dir;

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
        return ex.what();
    }
}