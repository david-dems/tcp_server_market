#include "RegistrationProcessor.h"

std::string RegistrationProcessor::process(tcp::socket& s){
    std::string firstName, lastName, login, pswd;
    std::cout << "Registration required!" << std::endl;
    std::cout << "Enter your first name, last name, login, password: " << std::endl;
    std::cin >> firstName >> lastName >> login >>  pswd;

    nlohmann::json j;
    j["UserId"] = "0";
    j["ReqType"] = Requests::Registration;
    j["FirstName"] = firstName;
    j["LastName"] = lastName;
    j["Login"] = login;
    j["Password"] = pswd;

    // Для регистрации Id не нужен, заполним его нулём
    SendMessage(s, j);
    auto rep = ReadMessage(s);
    return rep["UserId"].get<std::string>();
}

