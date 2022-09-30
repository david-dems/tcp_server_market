#include "RegisterHandler.h"
#include <iostream>

std::string RegHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();
    std::string query;
    
    boost::format fmt_insert = boost::format(insert_query_template) % j["Message"] % j["Message"] % j["Message"] % j["Message"];
    query = fmt_insert.str();
    PQsendQuery(C->connection().get(), query.c_str());
    
    while(auto res = PQgetResult(C->connection().get()));


    boost::format fmt_id = boost::format(id_query_template) % j["Message"];
    query = fmt_id.str();
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
    reply["UserId"] = reply_string; 

    return reply.dump();
}