#include "QuotationsHandler.h"
#include <iostream>

std::string QuotationsHandler::makeReply(nlohmann::json j){
    nlohmann::json reply;

    auto C = DataBase::getDB()->Pool().getConnection();

    try{

        std::string q;
        PQsendQuery(C->connection().get(), query_template.c_str());
        while(auto res = PQgetResult(C->connection().get())){
            if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
                q = PQgetvalue (res, 0, 0);
            }
            if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                throw std::logic_error(PQresultErrorMessage(res));
            }
            PQclear(res);
        }
        reply["Quotation"] = q;

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