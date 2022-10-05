#include "BalanceHandler.h"

#include <iostream>

std::string BalanceHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();

    std::string query;
    nlohmann::json reply;
    auto fmt = boost::format(query_template) % j["UserId"].get<std::string>();
    query = fmt.str();

    std::string USD, RUB;

    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
                USD = PQgetvalue(res, 0, 0);
                RUB = PQgetvalue(res, 0, 1);
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }
        PQclear(res);
    }

    reply["USD"] = std::move(USD);
    reply["RUB"] = std::move(RUB);
    

    DataBase::getDB()->Pool().freeConnection(C);

    return std::move(reply.dump());        
}