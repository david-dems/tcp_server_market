#include "HistoryProcessor.h"


std::string HistoryProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::History;
    
    SendMessage(s, req);
    auto resp = ReadMessage(s);
    
    auto vol = resp["vol"].get<std::vector<std::string>>();
    auto price = resp["price"].get<std::vector<std::string>>();
    auto direction = resp["direction"].get<std::vector<std::string>>();

    std::string reply;

    for (int i = 0; i < vol.size(); i++){
        reply += "USD: " + vol[i] + " Price: " + price[i] + " " + direction[i] + "\n";
    }
    
    return std::move(reply);
}