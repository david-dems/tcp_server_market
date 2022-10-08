#include "HelloHandler.h"
#include <iostream>

std::string HelHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();
    
    boost::format fmt = boost::format(hello_query_template) % j["UserId"].get<std::string>();
    std::string query = fmt.str();

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
    
    nlohmann::json reply;
    std::string reply_str(ID);
    reply["Name"] = std::move(reply_str);

    return std::move(reply.dump());
}