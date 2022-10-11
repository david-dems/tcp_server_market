#include "LoginProcessor.h"

std::string LoginProcessor::process(tcp::socket& s){
    std::string login, pswd;
    std::cout << "Hello! Enter your login and password: " << std::endl;
    std::cin >> login >>  pswd;

    nlohmann::json j;
    j["ReqType"] = Requests::SignIn;
    j["Login"] = login;
    j["Password"] = pswd;

    try{
        SendMessage(s, j);
        auto rep = ReadMessage(s);
        if (rep["Status"].get<std::string>() == "ok"){
            std::cout << rep["Message"].get<std::string>() << std::endl;
            return rep["UserId"].get<std::string>();
        }
        return std::move(rep["UserId"].get<std::string>());
    } catch (std::exception const& ex){
        std::cerr << ex.what() << std::endl;
        return "null";
    } 
}