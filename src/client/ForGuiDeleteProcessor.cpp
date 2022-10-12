#include "ForGuiDeleteProcessor.h"

std::string ForGuiDeleteProcessor::process(tcp::socket& s){
    try {
        nlohmann::json req_del;
        req_del["UserId"] = my_id;
        req_del["ReqType"] = Requests::Delete;
        req_del["AppId"] = appid;
    
        SendMessage(s, req_del);
        auto reply = ReadMessage(s);

        return std::move(reply["Message"].get<std::string>());
    } catch (std::exception const& ex){
        return "null";
    }
}

void ForGuiDeleteProcessor::setAppId(int appid_){
    appid = std::to_string(appid_);
}
