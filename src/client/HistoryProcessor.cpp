#include "HistoryProcessor.h"
#include <tuple>


std::string HistoryProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::History;
    
    SendMessage(s, req);
    auto resp = ReadMessage(s);
    
    auto idVolPriceDir = resp["IdVolPriceDir"].get<std::vector<std::tuple<std::string, std::string, std::string, std::string>>>();

    std::string reply;

    for (auto& [id, vol, price, dir] : idVolPriceDir){
        reply += "USD: " + vol + " Price: " + price + " " + dir + "\n";
    }
    
    return std::move(reply);
}