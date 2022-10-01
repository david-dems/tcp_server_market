#pragma once

#include <string>
#include <json.hpp>
#include "RequestHandler.h"
#include "DataBase.h"
#include <boost/format.hpp>

class SignInHandler : public RequestHandler{
    public:
    std::string makeReply(nlohmann::json j);

    private:
    std::string query_template = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED; 
        select userid from users
        where login = '%1%' and password = '%2%';
        COMMIT;
    )";

};