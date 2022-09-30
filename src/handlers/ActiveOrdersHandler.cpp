#include "ActiveOrdersHandler.h"

#include <iostream>

std::string ActiveOrderHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();

    std::string query;
    nlohmann::json reply;
    auto fmt = boost::format(query_template) % j["userid"].get<std::string>();
    query = fmt.str();

    std::vector<std::string> vol, price, direction;

    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            for (int i = 0; i < PQntuples(res); i++){
                vol.push_back(PQgetvalue(res, i, 0));
                price.push_back(PQgetvalue(res, i, 1));
                direction.push_back(PQgetvalue(res, i, 2));
            }
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }
        PQclear(res);
    }

    reply["vol"] = vol;
    reply["price"] = price;
    reply["direction"] = direction;
    

    DataBase::getDB()->Pool().freeConnection(C);

    return reply.dump();        
}