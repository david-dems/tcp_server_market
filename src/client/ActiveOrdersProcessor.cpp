#include "ActiveOrdersProcessor.h"

std::string ActiveOrdersProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::ActiveOrders;

    try{
        SendMessage(s, req);
    
        auto rep = ReadMessage(s);
        auto idVolPriceDir = rep["IdVolPriceDir"].get<std::vector<std::tuple<std::string, std::string, std::string, std::string>>>();
    
        std::string reply;
        for (auto& [id, vol, price, dir] : idVolPriceDir){
            reply += "USD: " + vol + " Price: " + price + " " + dir + "\n";
        }
        
        return std::move(reply);
    
    } catch (std::exception const& ex){
        std::cerr << ex.what() << std::endl;
        return "null";
    }
}