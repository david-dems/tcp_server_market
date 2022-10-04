#include "SignInHandler.h"

#include "RegisterHandler.h"
#include <iostream>

std::string SignInHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();
    std::string query;

    std::string login, password;
    login     = j["Login"].get<std::string>();
    password  = j["Password"].get<std::string>();
    
    boost::format fmt_query = boost::format(query_template) % login % password;
    query = fmt_query.str();
    PQsendQuery(C->connection().get(), query.c_str());
    
    char *ID;
    int count = 0;

    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            ID = PQgetvalue (res, 0, 0);
            count = PQntuples(res);
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }

        PQclear(res);
    }

    nlohmann::json resp;
    std::string id(ID);

    if (count == 0){
        resp["Status"] = "err";
        resp["Message"] = "User is not registered.";
        resp["UserId"] = "null";
    } else {
        resp["Status"] = "ok";
        resp["Message"] = "Authorized";
        resp["UserId"] = id;
    }

    return resp.dump();
}
