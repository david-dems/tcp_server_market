#include "HelloHandler.h"
#include <iostream>

std::string HelHandler::makeReply(nlohmann::json j){
    
    nlohmann::json reply;
    std::string userid;
   
    try{
        userid = j["UserId"].get<std::string>();
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

    boost::format fmt = boost::format(hello_query_template) % userid;
    std::string query = fmt.str();

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
        reply["Name"] = std::move(ID);

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