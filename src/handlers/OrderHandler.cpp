#include "OrderHandler.h"
#include <iostream>

std::string OrderHandler::makeReply(nlohmann::json j){
    //Check if the user in DB

    auto C = DataBase::getDB()->Pool().getConnection();

    std::string query;
    auto fmt = boost::format(query_template_check_user) % j["UserId"].get<std::string>(); 
    query = fmt.str();

    PQsendQuery(C->connection().get(), query.c_str());

    char *count;

    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            count = PQgetvalue (res, 0, 0);
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }

        PQclear(res);
    }

    if (count == 0)
        return "This user is not registred";

    std::string reply = "Order has been published!";
    fmt = boost::format(query_template) % j["UserId"] % j["vol"] % j["price"] % j["direction"] % j["status"] % "CURRENT_TIMESTMP";

    query = fmt.str();

    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
            reply = "error";
        }
        PQclear(res);
    }

    DataBase::getDB()->Pool().freeConnection(C);

    return reply;        
}