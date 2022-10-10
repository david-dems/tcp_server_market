#pragma once

#include <string>
#include "json.hpp"
#include "DataBase.h"
#include <boost/format.hpp>
#include <iostream>

class RequestHandler{
    public:
    virtual std::string makeReply(nlohmann::json j) = 0;
    bool isUserRegistred(std::string userid){
        std::string query_template = R"(
            BEGIN ISOLATION LEVEL READ COMMITTED; 
            select count(userID) from Users  
            where userID = %1%;
            COMMIT;
        )";
        boost::format fmt = boost::format(query_template) % userid;
        auto query = fmt.str();

        std::string count;
        auto C = DataBase::getDB()->Pool().getConnection();
        PQsendQuery(C->connection().get(), query.c_str());
        while(auto res = PQgetResult(C->connection().get())){
            if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
                count = PQgetvalue (res, 0, 0);
            }
            if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                std::cerr << PQresultErrorMessage(res)<<std::endl;
            }   
            PQclear(res);
        }
        DataBase::getDB()->Pool().freeConnection(C);

        if (count != "1")
            return false;

        return true;

    }
};