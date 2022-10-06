#pragma once

#include <string>
#include <json.hpp>
#include "RequestHandler.h"
#include "DataBase.h"
#include <boost/format.hpp>

class DealsHandler : public RequestHandler{
    public:
    std::string makeReply(nlohmann::json j);
    private:
    std::string query_template = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED;  
        select sellerid, buyerid, vol, price, date,
        seller.firstName, seller.lastName,
        buyer.firstName, buyer.lastName
        from deals 
        join users as seller on seller.userid = sellerid
        join users as buyer on buyer.userid = buyerid
        where sellerid = %1% or buyerid = %1%; 
        COMMIT; 
    )";
    
    std::string query_template_check_user = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED; 
        select count(userID) from Users  
        where userID = %1%;
        COMMIT;
    )";
};