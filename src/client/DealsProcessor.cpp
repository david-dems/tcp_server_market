#include "DealsProcessor.h"

#include <tuple>
#include <string_view>


std::string DealsProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::Deals;
    
    SendMessage(s, req);
    auto resp = ReadMessage(s);
    
    auto idVolPriceDir = resp["Data"].get<std::vector<std::tuple<std::string, 
                                                                 std::string, 
                                                                 std::string, 
                                                                 std::string, 
                                                                 std::string, 
                                                                 std::string, 
                                                                 std::string, 
                                                                 std::string, 
                                                                 std::string>>>();

    std::string reply;

    for (auto& [sellerid, buyerid, vol, price, date, sFName, sLName, bFName, bLName] : idVolPriceDir){
        if (my_id == sellerid)
            reply += "Sell to " + bFName + " " + bLName;
        else
            reply += "Bought from " + sFName + " " + sLName;

        reply += "USD: " + vol + "price: " + price + " date of deal: " + date.substr(0, 19) + "\n";
    }
    
    return std::move(reply);
}