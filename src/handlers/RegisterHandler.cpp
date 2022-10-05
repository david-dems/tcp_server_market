#include "RegisterHandler.h"
#include <iostream>

std::string RegHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();
    std::string query;

    std::string firstName, lastName, login, password;
    firstName = j["FirstName"].get<std::string>();
    lastName  = j["LastName"].get<std::string>();
    login     = j["Login"].get<std::string>();
    password  = j["Password"].get<std::string>();
    
    boost::format fmt_query = boost::format(insert_query_template) % firstName % lastName % login % password;
    query = fmt_query.str();
    PQsendQuery(C->connection().get(), query.c_str());
    
    char *ID;

    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            ID = PQgetvalue (res, 0, 0);
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }

        PQclear(res);
    }


    DataBase::getDB()->Pool().freeConnection(C);
    
    std::string reply_string(ID);
    nlohmann::json reply;
    reply["UserId"] = std::move(reply_string); 

    return std::move(reply.dump());
}