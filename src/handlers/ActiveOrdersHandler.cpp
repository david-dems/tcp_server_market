#include "ActiveOrdersHandler.h"

#include <iostream>
#include <tuple>

std::string ActiveOrderHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();

    std::string query;
    nlohmann::json reply;
    auto fmt = boost::format(query_template) % j["UserId"].get<std::string>();
    query = fmt.str();

    std::vector<std::tuple<std::string, std::string, std::string, std::string>> idVolPriceDir;

    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            for (int i = 0; i < PQntuples(res); i++){
                idVolPriceDir.push_back({
                    PQgetvalue(res, i, 3),   //orderId
                    PQgetvalue(res, i, 0),   //vol
                    PQgetvalue(res, i, 1),   //size
                    PQgetvalue(res, i, 2),   //dir
                });
            }
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }
        PQclear(res);
    }

    reply["IdVolPriceDir"] = idVolPriceDir;
    
    DataBase::getDB()->Pool().freeConnection(C);

    return std::move(reply.dump());        
}