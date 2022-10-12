#include "ForGuiRegistrationProcessor.h"

std::string ForGuiRegistrationProcessor::process(tcp::socket& s){
    nlohmann::json j;
    j["ReqType"] = Requests::Registration;
    j["FirstName"] = firstName;
    j["LastName"] = lastName;
    j["Login"] = login;
    j["Password"] = password;

    try
    {
        SendMessage(s, j);
        auto rep = ReadMessage(s);
        return std::move(rep["UserId"].get<std::string>());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return "null";
    }
    
}

void ForGuiRegistrationProcessor::setFLLP(
        std::string firstName_,
        std::string lastName_,
        std::string login_,
        std::string password_
){
    firstName = firstName_;
    lastName = lastName_;
    login = login_;
    password = password_;
}