#include "DeleteHandler.h"

#include <iostream>

std::string DeleteHandler::makeReply(nlohmann::json j){
    nlohmann::json reply;
    std::string userid, appid;

    try{
        userid = j["UserId"].get<std::string>();
        appid = j["AppId"].get<std::string>();
        if (!isUserRegistred(userid)){
            throw std::logic_error("Registration required!");
        }
    } catch (nlohmann::json::exception const& ex){
        std::cerr << ex.what() << std::endl;
        reply["Error"] = "bad json, null value instead of string! ";
        return std::move(reply.dump());
    } catch (std::logic_error const& e){
        reply["Error"] = e.what();
        return reply.dump();
    } catch (std::exception const& e){
        std::cerr << e.what();
        reply["Error"] = "Server error";
        return reply.dump();
    }      
    
    auto fmt = boost::format(check_appid_template) % userid % appid;
    std::string check_appid = fmt.str();

    std::string count = "";
    auto C = DataBase::getDB()->Pool().getConnection();
    
    try{
        PQsendQuery(C->connection().get(), check_appid.c_str());
        while(auto res = PQgetResult(C->connection().get())){
            if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
                count = PQgetvalue (res, 0, 0);
            }
            if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                throw std::logic_error(PQresultErrorMessage(res));
            }
        }
    } catch (std::logic_error const& e) {
        DataBase::getDB()->Pool().freeConnection(C);
        std::cerr << e.what(); 
        reply["Error"] = "Data base error";
        return reply.dump();
    } catch (std::exception const& e){
        DataBase::getDB()->Pool().freeConnection(C);
        std::cerr << e.what();
        reply["Error"] = "Server error";
        return reply.dump();
    }
    
    if (count != "1"){
        DataBase::getDB()->Pool().freeConnection(C);
        reply["Error"] = "Invalid appid";
        return reply.dump();
    }

    fmt = boost::format(query_template) % appid % userid;
    std::string query = fmt.str();

    try{ 

        PQsendQuery(C->connection().get(), query.c_str());
        while(auto res = PQgetResult(C->connection().get())){
            if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                throw std::logic_error(PQresultErrorMessage(res));
            }   
            PQclear(res);
        }
        reply["Message"] = "Deleted successfully";

    } catch (std::logic_error const& e) {
        std::cerr << e.what(); 
        reply["Error"] = "Data base error";
    } catch (std::exception const& e){
        std::cerr << e.what();
        reply["Error"] = "Server error";
    }

    DataBase::getDB()->Pool().freeConnection(C);

    return reply.dump();
}