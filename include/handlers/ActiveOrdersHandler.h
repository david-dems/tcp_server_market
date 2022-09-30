#pragma once

#include <string>
#include <json.hpp>
#include "RequestHandler.h"
#include "DataBase.h"
#include <boost/format.hpp>

class ActiveOrderHandler : public RequestHandler{
    public:
    std::string makeReply(nlohmann::json j);
    private:
    std::string query_template = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED;  
        select vol, price, direction  
        from applications 
        where userid = %1% and status = 'active'; 
        COMMIT; 
    )";
    
    std::string query_template_check_user = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED; 
        select count(userID) from Users  
        where userID = %1%;
        COMMIT;
    )";
};