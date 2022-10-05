#include "HelloProcessor.h"

std::string HelloProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::Hello;

    SendMessage(s, req);
    auto rep = ReadMessage(s);

    return std::move(rep["Name"].get<std::string>());
}