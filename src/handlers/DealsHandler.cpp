#include "DealsHandler.h"

#include <iostream>

std::string DealsHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();

    std::string query;
    nlohmann::json reply;
    auto fmt = boost::format(query_template) % j["UserId"].get<std::string>();
    query = fmt.str();

    std::vector<std::tuple< std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string >> data;

    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            for (int i = 0; i < PQntuples(res); i++){
                data.push_back({
                    PQgetvalue(res, i, 0),   // sellerid
                    PQgetvalue(res, i, 1),   // buyerid
                    PQgetvalue(res, i, 2),   // vol
                    PQgetvalue(res, i, 3),   // price
                    PQgetvalue(res, i, 4),   // date
                    PQgetvalue(res, i, 5),   // seller.firstName
                    PQgetvalue(res, i, 6),   // seller.lastName
                    PQgetvalue(res, i, 7),   // buyer.firstName
                    PQgetvalue(res, i, 8),   // buyer.lastName
                });
            }
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }
        PQclear(res);
    }

    reply["Data"] = std::move(data);
    

    DataBase::getDB()->Pool().freeConnection(C);

    return std::move(reply.dump());        
}