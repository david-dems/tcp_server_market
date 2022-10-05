#include "BalanceProcessor.h"


std::string BalanceProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::Balance;
    
    SendMessage(s, req);
    auto resp = ReadMessage(s);
    
    auto usd = resp["USD"].get<std::string>();
    auto rub = resp["RUB"].get<std::string>();
    
    return std::move("USD: " + usd + " RUB : " + rub);
}