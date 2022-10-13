#include "ForGuiLoginProcessor.h"

std::string ForGuiLoginProcessor::process(tcp::socket& s){
    nlohmann::json j;
    j["ReqType"] = Requests::SignIn;
    j["Login"] = login_;
    j["Password"] = password_;

    try{
        SendMessage(s, j);
        auto rep = ReadMessage(s);
        if (rep["Status"].get<std::string>() == "ok"){
            return rep["UserId"].get<std::string>();
        }
        return std::move(rep["UserId"].get<std::string>());
    } catch (std::exception const& ex){
        std::cerr << ex.what() << std::endl;
        return "null";
    } 
}

void ForGuiLoginProcessor::setLP(std::string login, std::string password){
    login_ = login;
    password_ = password;
} 