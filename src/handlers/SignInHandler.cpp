#include "SignInHandler.h"

#include "RegisterHandler.h"
#include <iostream>

std::string SignInHandler::makeReply(nlohmann::json j){
    nlohmann::json reply;
    std::string login, password;
   
    try{
        login     = j["Login"].get<std::string>();
        password  = j["Password"].get<std::string>();
    } catch (nlohmann::json::exception const& ex){
        std::cerr << ex.what() << std::endl;
        reply["Error"] = "bad json, null value instead of string! ";
        return std::move(reply.dump());
    } catch (std::exception const& e){
        std::cerr << e.what();
        reply["Error"] = "Server error";
        return reply.dump();
    } 
    
    boost::format fmt_query = boost::format(query_template) % login % password;
    std::string query = fmt_query.str();
    
    auto C = DataBase::getDB()->Pool().getConnection();
        std::string ID;
        int count = 0;
    try{
        PQsendQuery(C->connection().get(), query.c_str());

        while(auto res = PQgetResult(C->connection().get())){
            if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
                ID = PQgetvalue (res, 0, 0);
                count = PQntuples(res);
            }

            if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                throw std::logic_error(PQresultErrorMessage(res));
            }

            PQclear(res);
        }
        if (count == 0){
            reply["Status"] = "err";
            reply["Message"] = "User is not registered.";
            reply["UserId"] = "null";
        } else {
            reply["Status"] = "ok";
            reply["Message"] = "Authorized";
            reply["UserId"] = ID;
        }
    } catch (std::logic_error const& e) {
        std::cerr << e.what() << std::endl;
        reply["Error"] = "Data base error";
    } catch (std::exception const& e){
        std::cerr << e.what();
        reply["Error"] = "Server error";
    }
    
    DataBase::getDB()->Pool().freeConnection(C);

    return std::move(reply.dump());
}
