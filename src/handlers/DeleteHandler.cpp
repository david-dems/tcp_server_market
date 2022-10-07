#include "DeleteHandler.h"

#include <iostream>

std::string DeleteHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();

    std::string check_appid;
    auto fmt = boost::format(check_appid_template) % j["UserId"].get<std::string>() % j["AppId"]; //////////////
    check_appid = fmt.str();

    std::string rep = "Deleted successfull";
    std::string count = "";
    PQsendQuery(C->connection().get(), check_appid.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            count = PQgetvalue (res, 0, 0);
        }
        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
            rep = "error occures";
        }
    }
    

    std::string query;
    fmt = boost::format(query_template) % j["AppId"] % j["UserId"].get<std::string>();
    query = fmt.str();


    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
            rep = "error occures";
        }
        PQclear(res);
    }
    
    DataBase::getDB()->Pool().freeConnection(C);

    if (count != "1")
        rep = "Invalid appid";

    nlohmann::json reply;

    reply["Message"] = rep;
    return reply.dump();
}