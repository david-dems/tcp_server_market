#include "Engine.h"
#include <boost/format.hpp>
#include <thread>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <tuple>

std::vector<order> MatchingEngine::getSaleOrders(){
    auto C = DataBase::getDB()->Pool().getConnection();

    PQsendQuery(C->connection().get(), sale_query.c_str());
    std::vector<order> orders;
    while(auto res = PQgetResult(C->connection().get())){
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res)) {
            for (int i = 0; i < PQntuples(res); i++){
                order tmp;
                tmp.id = std::stoi(PQgetvalue(res, i, 0));
                tmp.userid = std::stoi(PQgetvalue(res, i, 1));
                tmp.vol = std::stod(PQgetvalue(res, i, 2));
                tmp.price =round(std::stod(PQgetvalue(res, i, 3)) * 100) / 100;
                tmp.status = PQgetvalue(res, i, 4);
                tmp.date = std::stol(PQgetvalue(res, i, 5));
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
                tmp.id = std::stoi(PQgetvalue(res, i, 0));
                tmp.userid = std::stoi(PQgetvalue(res, i, 1));
                tmp.vol = round(std::stod(PQgetvalue(res, i, 2)) * 100) / 100;
                tmp.price =round(std::stod(PQgetvalue(res, i, 3)) * 100) / 100;
                tmp.status = PQgetvalue(res, i, 4);
                tmp.date = std::stol(PQgetvalue(res, i, 5));
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
    auto sales = getSaleOrders();
    auto purchases = getPurchaseOrders();

    auto deals = processDeals(std::move(sales), std::move(purchases));
    sendQuery(std::move(deals));

}


void MatchingEngine::run(){
    std::thread engine_thread(&MatchingEngine::repeatMatch, this);
    engine_thread.detach();
}

void MatchingEngine::repeatMatch(){
    while(true){
        match();
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
}


std::vector<deal> MatchingEngine::processDeals(std::vector<order> sales, std::vector<order> purchases){


    if (sales.size() == 0 || purchases.size() == 0){
        return {};
    }

    std::vector<deal> result;

    auto it_sales = sales.begin();
    auto it_purch = purchases.begin();

    std::string order_status_s;
    std::string order_status_p;

    auto head = purchases.begin();    
    bool backtohead = false;
    while(it_sales != sales.end() && it_purch != purchases.end()){
        if (it_sales->userid == it_purch->userid){
            if (std::next(it_purch,1) != purchases.end()){
                if (!backtohead){
                    backtohead = true;
                    head = it_purch;
                }
                it_purch++;
                continue;
            } else {
                it_purch = head;
                it_sales++;
                continue;
            } 
        }


        if (it_sales->price <= it_purch->price){
            deal d;
            if (it_sales->vol > it_purch->vol){

                d = {
                    it_sales->userid,
                    it_purch->userid,
                    it_purch->vol,
                    (it_sales->date > it_purch->date) ? it_purch->price : it_sales->price,
                    "active",
                    "closed",
                    it_sales->id,
                    it_purch->id,
                };

                it_purch->status = "closed";
                it_purch->vol -= d.vol;
                it_sales->vol -= d.vol;

            } else if (it_sales->vol < it_purch->vol){
                d = {
                    it_sales->userid,
                    it_purch->userid,
                    it_sales->vol,
                    (it_sales->date > it_purch->date) ? it_purch->price : it_sales->price,
                    "closed",
                    "active",
                    it_sales->id,
                    it_purch->id,
                };

                it_sales->status = "closed";
                it_purch->vol -= d.vol;
                it_sales->vol -= d.vol;

                if (backtohead){
                    it_purch = head;
                    backtohead = false;
                }
            } else if (it_sales->vol == it_purch->vol){
                d = {
                    it_sales->userid,
                    it_purch->userid,
                    it_sales->vol,
                    (it_sales->date > it_purch->date) ? it_purch->price : it_sales->price,
                    "closed",
                    "closed",
                    it_sales->id,
                    it_purch->id,
                };

                it_sales->status = "closed";
                it_purch->status = "closed";
                it_purch->vol -= d.vol;
                it_sales->vol -= d.vol;

                if (backtohead){
                    it_purch = head;
                    backtohead = false;
                }
            }

            result.push_back(d);

        } else {
            if (!backtohead){
                break;
            } else {
                it_purch = head;
                it_sales++;
                continue;
            }
        }
        
        if (it_sales->status != "active" && it_purch->status != "active"){
            it_sales++;
            it_purch++;
            continue;
        }

        if (it_sales->status != "active"){
            it_sales++;
            continue;
        }
        if (it_purch->status != "active"){
            it_purch++;
            continue;
        }
    } 
    
    return std::move(result);
}

void MatchingEngine::sendQuery(std::vector<deal> deals){
    auto C = DataBase::getDB()->Pool().getConnection();

    for (auto d : deals){
        std::string query;

        query = begin;
          
        auto [seller, buyer, vol, price, order_status_s, order_status_p, oIds, oIdp] = d;

        boost::format fmt;
        fmt = boost::format(insert_deal) % seller % buyer % vol % price;
        query += fmt.str(); 

        fmt = boost::format(update_balance) % "-" % vol % "+" % (vol * price) % seller;
        query += fmt.str(); 
        fmt = boost::format(update_balance) % "+" % vol % "-" % (vol * price) % buyer;
        query += fmt.str(); 


        fmt = boost::format(update_order) % vol % order_status_s % oIds;
        query += fmt.str(); 

        fmt = boost::format(update_order) % vol % order_status_p % oIdp;
        query += fmt.str(); 

        query += end;

        PQsendQuery(C->connection().get(), query.c_str());
        while(auto res = PQgetResult(C->connection().get())){

            if (PQresultStatus(res) == PGRES_FATAL_ERROR){
                std::cout<< PQresultErrorMessage(res)<<std::endl;
            }
            PQclear(res);   
        }
    }
    DataBase::getDB()->Pool().freeConnection(C);
}
