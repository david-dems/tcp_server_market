#include "DealsHandler.h"

#include <iostream>

std::string DealsHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();

    std::string query;
    nlohmann::json reply;
    auto fmt = boost::format(query_template) % j["UserId"].get<std::string>();
    query = fmt.str();

    std::vector<std::string> sellerid, buyerid, vol, price;

    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            for (int i = 0; i < PQntuples(res); i++){
                sellerid.push_back(PQgetvalue(res, i, 0));
                buyerid.push_back(PQgetvalue(res, i, 1));
                vol.push_back(PQgetvalue(res, i, 2));
                price.push_back(PQgetvalue(res, i, 3));
            }
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }
        PQclear(res);
    }

    reply["sellerid"] = std::move(sellerid);
    reply["buyerid"] = std::move(buyerid);
    reply["vol"] = std::move(vol);
    reply["price"] = std::move(price);
    

    DataBase::getDB()->Pool().freeConnection(C);

    return std::move(reply.dump());        
}