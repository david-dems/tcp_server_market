#pragma once

#include <string>
#include <json.hpp>
#include "RequestHandler.h"
#include "DataBase.h"
#include <boost/format.hpp>

class RegHandler : public RequestHandler{
    public:
    std::string makeReply(nlohmann::json j);

    private:
    std::string insert_query_template = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED; 
        
        insert into Users (firstname, lastname, login, password)
        values ('%1%', '%2%', '%3%', '%4%');
        
        select userid from users
        order by userid desc
        limit 1;
        
        COMMIT;
    )";

};