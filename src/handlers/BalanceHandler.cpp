#include "BalanceHandler.h"

#include <iostream>

std::string BalanceHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();

    std::string query;
    nlohmann::json reply;
    auto fmt = boost::format(query_template) % j["userid"].get<std::string>();
    query = fmt.str();

    std::vector<std::string> USD, RUB;

    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            for (int i = 0; i < PQntuples(res); i++){
                USD.push_back(PQgetvalue(res, i, 0));
                RUB.push_back(PQgetvalue(res, i, 1));
            }
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }
        PQclear(res);
    }

    reply["USD"] = USD;
    reply["RUB"] = RUB;
    

    DataBase::getDB()->Pool().freeConnection(C);

    return reply.dump();        
}