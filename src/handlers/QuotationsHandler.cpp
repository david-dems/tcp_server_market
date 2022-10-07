#include "QuotationsHandler.h"
#include <iostream>

std::string QuotationsHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();
    std::string query;
    
    PQsendQuery(C->connection().get(), query_template.c_str());

    char *q;

    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            q = PQgetvalue (res, 0, 0);
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }

        PQclear(res);
    }


    DataBase::getDB()->Pool().freeConnection(C);
    
    nlohmann::json reply;
    std::string reply_str(q);
    reply["Quotation"] = q;

    return std::move(reply.dump());
}