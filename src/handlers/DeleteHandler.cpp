#include "DeleteHandler.h"

#include <iostream>

std::string DeleteHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();

    std::string query;
    nlohmann::json reply;
    auto fmt = boost::format(query_template) % j["AppId"] % j["UserId"].get<std::string>();
    query = fmt.str();

    std::string rep;
    rep = "Deleted successfull";

    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
            rep = "error occures";
        }
        PQclear(res);
    }

    

    DataBase::getDB()->Pool().freeConnection(C);
    reply["Message"] = rep;
    return reply.dump();
}