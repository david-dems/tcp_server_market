#pragma once
#include "Processor.h"

class ForGuiRegistrationProcessor : public Processor{
    std::string process(tcp::socket& s);

    std::string firstName, lastName, login, password;
public:
    void setFLLP(
        std::string firstName_,
        std::string lastName_,
        std::string login_,
        std::string password_
    );
};