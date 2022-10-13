#include "OrderHandler.h"
#include <iostream>
#include <regex>

std::string OrderHandler::makeReply(nlohmann::json j){
    nlohmann::json reply;
    std::string userid;
    std::string vol, price, dir;

    try{
        static const std::regex regexDir(R"((sale|purchase))");
        userid = j["UserId"].get<std::string>();
        vol = j["Vol"].get<std::string>();
        price = j["Price"].get<std::string>();
        dir = j["Direction"].get<std::string>();
        if (!isUserRegistred(userid)){
            throw std::logic_error("Registration required!");
        }
        if (!regex_search(dir, regexDir)){
            throw std::logic_error("Bad input of order params!");
        } 

        auto d = std::stod(vol);
             d = std::stod(price);

    } catch (nlohmann::json::exception const& ex){
        std::cerr << ex.what() << std::endl;
        reply["Error"] = "bad json, null value instead of string! ";
        return std::move(reply.dump());
    } catch (std::invalid_argument const& e) {
        std::cerr << e.what() << std::endl;
        reply["Error"] = "Bad input of order params! ";
        return std::move(reply.dump()); 
    } catch (std::logic_error const& e){
        reply["Error"] = e.what();
        return reply.dump();
    } catch (std::exception const& e){
        std::cerr << e.what();
        reply["Error"] = "Server error";
        return reply.dump();
    }  


    auto fmt = boost::format(query_template) % userid % vol % price % dir % "active" % "CURRENT_TIMESTAMP";
    std::string query = fmt.str();

    auto C = DataBase::getDB()->Pool().getConnection();

    try {
        
        PQsendQuery(C->connection().get(), query.c_str());
        while(auto res = PQgetResult(C->connection().get())){
            if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                throw std::logic_error(PQresultErrorMessage(res));
            }
            PQclear(res);
        }
        reply["Message"] = "Order is published";

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