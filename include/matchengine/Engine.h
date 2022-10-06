#include "DataBase.h"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

struct order {
    int id;
    int userid;
    double vol;
    double price;
    long int date;
    std::string status;
};

struct deal {
     int sellerid;
     int buyerid;
     double vol;
     double price;
};

class MatchingEngine{
public:
    void run();
private:
    void match();
    void repeatMatch();
    std::vector<order> getSaleOrders();
    std::vector<order> getPurchaseOrders();
    //void processDeal();

    int time = 3000;

    std::string sale_query = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED;
        select appid, userid, vol, price, status, to_char(date, 'YYYYMMDDHHMMSSmm')
        from applications
        where status = 'active' and direction = 'sale'
        order by price, date;
        COMMIT;
    )"; 

    std::string purchase_query = R"(
        BEGIN ISOLATION LEVEL READ COMMITTED;
        select appid, userid, vol, price, status, to_char(date, 'YYYYMMDDHHMMSSmm')
        from applications
        where status = 'active' and direction = 'purchase'
        order by price desc, date asc;
        COMMIT;
    )";
    
    std::string begin = R"(BEGIN ISOLATION LEVEL READ COMMITTED;)";
    std::string end = R"(COMMIT;)";

    std::string insert_deal = R"(
        insert into deals (sellerid, buyerid, vol, price, date)
        values(%1%, %2%, %3%, %4%, CURRENT_TIMESTAMP);
    )";

    std::string update_balance = R"(
        update balance
        set
        usd = usd %1% %2%,
        rub = rub %3% %4%
        where userid = %5%;
    )";

    std::string update_order = R"(
        update applications
        set
        vol = vol - %1%, 
        status = '%2%'
        where appid = %3%;
    )";

};