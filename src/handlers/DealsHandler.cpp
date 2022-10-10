#include "DealsHandler.h"

#include <iostream>

std::string DealsHandler::makeReply(nlohmann::json j){

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

    auto fmt = boost::format(query_template) % j["UserId"].get<std::string>();
    std::string query = fmt.str();

    std::vector<std::tuple< std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string,
                            std::string >> data;

    auto C = DataBase::getDB()->Pool().getConnection();

    try{ 

        PQsendQuery(C->connection().get(), query.c_str());
        while(auto res = PQgetResult(C->connection().get())){
            if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
                for (int i = 0; i < PQntuples(res); i++){
                    data.push_back({
                        PQgetvalue(res, i, 0),   // sellerid
                        PQgetvalue(res, i, 1),   // buyerid
                        PQgetvalue(res, i, 2),   // vol
                        PQgetvalue(res, i, 3),   // price
                        PQgetvalue(res, i, 4),   // date
                        PQgetvalue(res, i, 5),   // seller.firstName
                        PQgetvalue(res, i, 6),   // seller.lastName
                        PQgetvalue(res, i, 7),   // buyer.firstName
                        PQgetvalue(res, i, 8),   // buyer.lastName
                    });
                }
            }

            if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                std::logic_error(PQresultErrorMessage(res));
            }
            PQclear(res);
        }
        reply["Data"] = data;
    
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