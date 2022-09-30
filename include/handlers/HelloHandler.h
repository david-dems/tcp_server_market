#pragma once

#include <string>
#include <json.hpp>
#include "RequestHandler.h"
#include "DataBase.h"
#include <boost/format.hpp>

class HelHandler : public RequestHandler{
    public:
    std::string makeReply(nlohmann::json j);

    std::string hello_query_template = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED;
        select firstname from Users 
        where userid = %1%;
        COMMIT;
    )";
};