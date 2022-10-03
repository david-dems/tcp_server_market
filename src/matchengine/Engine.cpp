#include "Engine.h"
#include <boost/format.hpp>
#include <thread>

std::vector<order> MatchingEngine::getSaleOrders(){
    auto C = DataBase::getDB()->Pool().getConnection();

    PQsendQuery(C->connection().get(), sale_query.c_str());
    std::vector<order> orders;
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            for (int i = 0; i < PQntuples(res); i++){
                order tmp;
                tmp.id = std::stoi(PQgetvalue(res, 0, 0));
                tmp.userid = std::stoi(PQgetvalue(res, 0, 1));
                tmp.vol = std::stod(PQgetvalue(res, 0, 2));
                tmp.price = std::stod(PQgetvalue(res, 0, 3));
                tmp.status = PQgetvalue(res, 0, 4);
                orders.push_back(tmp);
            }
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }
        PQclear(res);   
    }

    DataBase::getDB()->Pool().freeConnection(C);

    return orders;
}


std::vector<order> MatchingEngine::getPurchaseOrders(){
    auto C = DataBase::getDB()->Pool().getConnection();

    PQsendQuery(C->connection().get(), purchase_query.c_str());
    std::vector<order> orders;
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            for (int i = 0; i < PQntuples(res); i++){
                order tmp;
                tmp.id = std::stoi(PQgetvalue(res, 0, 0));
                tmp.userid = std::stoi(PQgetvalue(res, 0, 1));
                tmp.vol = std::stod(PQgetvalue(res, 0, 2));
                tmp.price = std::stod(PQgetvalue(res, 0, 3));
                tmp.status = PQgetvalue(res, 0, 4);;
                orders.push_back(tmp);
            }
        }

        if (PQresultStatus(res) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res)<<std::endl;
        }
        PQclear(res);   
    }
    
    DataBase::getDB()->Pool().freeConnection(C);

    return orders;
}


void MatchingEngine::match(){
    auto C = DataBase::getDB()->Pool().getConnection();
    auto sales = getSaleOrders();
    auto purchases = getPurchaseOrders();

    if (sales.size() == 0 || purchases.size() == 0){
        DataBase::getDB()->Pool().freeConnection(C);
        return;
    }

    auto it_sales = sales.begin();
    auto it_purch = purchases.begin();

    std::string order_status_s;
    std::string order_status_p;

    while (it_sales != sales.end() || it_purch != purchases.end()){
        if (it_sales->price <= it_purch->price){
            deal d;
            int order_id_s = it_sales->id;
            int order_id_b = it_purch->id;
            if (it_sales->vol > it_purch->vol){
                d.sellerid = it_sales->userid;
                d.buyerid = it_purch->userid;
                d.vol = it_purch->vol;
                d.price = it_sales->price;
                order_status_s = "active";
                order_status_p = "closed";
                it_purch++;
            } else if (it_sales->vol < it_purch->vol){
                d.sellerid = it_sales->userid;
                d.buyerid = it_purch->userid;
                d.vol = it_sales->vol;
                d.price = it_sales->price;
                order_status_s = "closed";
                order_status_p = "active";
                it_sales++;
            } else if (it_sales->vol == it_purch->vol){
                d.sellerid = it_sales->userid;
                d.buyerid = it_purch->userid;
                d.vol = it_sales->vol;
                d.price = it_sales->price;
                order_status_s = "closed";
                order_status_p = "closed";
                it_sales++;
                it_purch++;
            }

            std::string query;

            query += begin;
            
            boost::format fmt;
            fmt = boost::format(insert_deal) % d.sellerid % d.buyerid % d.vol % d.price;
            query += fmt.str(); 

            fmt = boost::format(update_balance) % "-" % d.vol % "+" % (d.vol * d.price) % d.sellerid;
            query += fmt.str(); 
            fmt = boost::format(update_balance) % "+" % d.vol % "-" % (-d.vol * d.price) % d.buyerid;
            query += fmt.str(); 


            fmt = boost::format(update_order) % d.vol % order_status_s % order_id_s;
            query += fmt.str(); 

            fmt = boost::format(update_order) % d.vol % order_status_p % order_id_b;
            query += fmt.str(); 

            query += end;

            PQsendQuery(C->connection().get(), query.c_str());
            while(auto res = PQgetResult(C->connection().get())){

                if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                    std::cout<< PQresultErrorMessage(res)<<std::endl;
                }
                PQclear(res);   
            }

        
        } else {
            break;
        }
        
    }
    
    DataBase::getDB()->Pool().freeConnection(C);

}


void MatchingEngine::run(){
    std::thread engine_thread(&MatchingEngine::match, this);
    engine_thread.join();
}
