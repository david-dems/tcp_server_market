#include "MakeOrderProcessor.h"

std::string MakeOrderProcessor::process(tcp::socket& s){
int vol, price;
    std::string dir;

    std::cin >> vol >> price >> dir;

    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::Order;
    req["vol"] = vol;
    req["price"] = price;
    req["direction"] = dir;
                
    SendMessage(s, req);
    auto resp = ReadMessage(s);
    return resp["Message"].get<std::string>();
}