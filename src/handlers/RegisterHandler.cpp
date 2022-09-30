#include "RegisterHandler.h"
#include <iostream>

std::string RegHandler::makeReply(nlohmann::json j){
    auto C = DataBase::getDB()->Pool().getConnection();
    std::string query;
    query  = "insert into Users (firstname, lastname, login, password) ";
    query += "values ('";
    query += j["Message"];
    query += "', 'asdfg', 'login', 'qwertylogin');";
    
    PQsendQuery(C->connection().get(), query.c_str());
    
    while(auto res = PQgetResult(C->connection().get()));

    query = "select userID from Users ";
    query += "where firstname = \'";
    query += j["Message"];
    query += "\';";

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