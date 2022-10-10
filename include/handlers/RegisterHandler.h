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
    bool isLoginAvailable(std::string login);

    std::string insert_query_template = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED; 
        
        insert into Users (firstname, lastname, login, password)
        values ('%1%', '%2%', '%3%', '%4%');

        insert into balance (userid, usd, rub)
        values(
            (select userid from users
            order by userid desc
            limit 1),
            0,
            0
        );
        
        select userid from users
        order by userid desc
        limit 1;
        
        COMMIT;
    )";

};