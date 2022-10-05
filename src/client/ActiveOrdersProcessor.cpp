#include "ActiveOrdersProcessor.h"

std::string ActiveOrdersProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::ActiveOrders;

    SendMessage(s, req);
    auto rep = ReadMessage(s);

    auto vol = rep["vol"].get<std::vector<std::string>>();
    auto price = rep["price"].get<std::vector<std::string>>();
    auto direction = rep["direction"].get<std::vector<std::string>>();

    std::string reply;
    for (int i = 0; i < vol.size(); i++){
        reply += "USD: " + vol[i] + " Price: " + price[i] + " " + direction[i] + "\n";
    }

    return std::move(reply);

}