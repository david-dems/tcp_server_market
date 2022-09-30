#include "HelloHandler.h"
#include <iostream>

std::string HelHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();
    std::string query;
    
    boost::format fmt = boost::format(hello_query_template) % j["UserId"].get<std::string>();
    query = fmt.str();
    std::cout << query << std::endl;
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
    
    std::string reply(ID);

    return reply;
}