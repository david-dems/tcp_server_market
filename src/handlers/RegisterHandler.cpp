#include "RegisterHandler.h"
#include <iostream>

std::string RegHandler::makeReply(nlohmann::json j){
    nlohmann::json reply;
    std::string firstName, lastName, login, password;

    try{
        firstName = j["FirstName"].get<std::string>();
        lastName  = j["LastName"].get<std::string>();
        login     = j["Login"].get<std::string>();
        password  = j["Password"].get<std::string>();
        if (!isLoginAvailable(login)){
            throw std::logic_error("Login is unavailable");
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
    
    boost::format fmt_query = boost::format(insert_query_template) % firstName % lastName % login % password;
    std::string query = fmt_query.str();
    
    auto C = DataBase::getDB()->Pool().getConnection();

    try{

        PQsendQuery(C->connection().get(), query.c_str());
        std::string ID;
        while(auto res = PQgetResult(C->connection().get())){
            if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
                ID = PQgetvalue (res, 0, 0);
            }
            if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                throw std::logic_error(PQresultErrorMessage(res));
            }
            PQclear(res);
        }
        reply["UserId"] = ID;

    } catch (std::logic_error const& e) {
        std::cerr << e.what(); 
        reply["Error"] = "Data base error";
    } catch (std::exception const& e){
        std::cerr << e.what();
        reply["Error"] = "Server error";
    }

    DataBase::getDB()->Pool().freeConnection(C);
    
    return std::move(reply.dump());
}


bool RegHandler::isLoginAvailable(std::string login){
    std::string query_template = R"(
            BEGIN ISOLATION LEVEL READ COMMITTED; 
            select count(userID) from Users  
            where login = '%1%';
            COMMIT;
        )";
    boost::format fmt = boost::format(query_template) % login;
    auto query = fmt.str();

    std::string count;
    auto C = DataBase::getDB()->Pool().getConnection();
    PQsendQuery(C->connection().get(), query.c_str());
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            count = PQgetvalue (res, 0, 0);
        }
        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cerr << PQresultErrorMessage(res)<<std::endl;
        }   
        PQclear(res);
    }
    DataBase::getDB()->Pool().freeConnection(C);

    if (count != "1")
        return true;

    return false;
}