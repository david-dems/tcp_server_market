#include <gtest/gtest.h>

#include "json.hpp"
#include "Common.hpp"
#include "HandlerFactory.h"
#include "RegisterHandler.h"
#include "HelloHandler.h"
#include "OrderHandler.h"
#include "ActiveOrdersHandler.h"
#include "BalanceHandler.h"
#include "SignInHandler.h"
#include "HistoryHandler.h"
#include "DealsHandler.h"
#include "DeleteHandler.h"
#include "QuotationsHandler.h"
#include "Engine.h"

HandlerFactory handlerFactory;

TEST(EngineTest, noOrders){
    std::vector<order> sales, purchases;
    sales = {};
    
    purchases = {};

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    ASSERT_TRUE(deals.size() == 0);
}

TEST(EngineTest, onlySales){
    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 20, 63, 2022100816325058, "active"}, //id, userid, vol, price, status, date
        {2, 1, 21, 65, 2022100812025458, "active"}, 
        {3, 2, 22, 60, 2022100816045458, "active"}, 
        {4, 2, 23, 65, 2022103816025458, "active"}, 
        {5, 3, 24, 69, 2022104816025358, "active"}, 
        {6, 4, 25, 65, 2022100816025448, "active"}, 
        {7, 5, 26, 67, 2022100816023458, "active"}, 
        {8, 6, 20, 68, 2022100816024444, "active"} 
    };
    
    purchases = {};

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    ASSERT_TRUE(deals.size() == 0);
}

TEST(EngineTest, onlyPurchases){
    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 20, 63, 2022100816325058, "active"}, //id, userid, vol, price, status, date
        {2, 1, 21, 65, 2022100812025458, "active"}, 
        {3, 2, 22, 60, 2022100816045458, "active"}, 
        {4, 2, 23, 65, 2022103816025458, "active"}, 
        {5, 3, 24, 69, 2022104816025358, "active"}, 
        {6, 4, 25, 65, 2022100816025448, "active"}, 
        {7, 5, 26, 67, 2022100816023458, "active"}, 
        {8, 6, 20, 68, 2022100816024444, "active"} 
    };
    
    purchases = {};

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    ASSERT_TRUE(deals.size() == 0);
}

TEST(EngineTest, sameSellerAndBuyer){
    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 20, 60, 2022100816025058, "active"} //id, userid, vol, price, status, date
    };
    
    purchases = {
        {2, 1, 20, 60, 2022100816025058, "active"} //id, userid, vol, price, status, date
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    ASSERT_TRUE(deals.size() == 0);
}

TEST(EngineTest, sameSellerAndBuyerManyOrders){
    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 20, 63, 2022100816325058, "active"}, //id, userid, vol, price, status, date
        {2, 1, 21, 65, 2022100812025458, "active"}, 
        {3, 1, 22, 60, 2022100816045458, "active"}, 
        {4, 1, 23, 65, 2022103816025458, "active"}, 
    };

    purchases = {
        {5, 1, 24, 69, 2022104816025358, "active"}, 
        {6, 1, 25, 65, 2022100816025448, "active"}, 
        {7, 1, 26, 67, 2022100816023458, "active"}, 
        {8, 1, 20, 68, 2022100816024444, "active"} 
    };
    
    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    ASSERT_TRUE(deals.size() == 0);
}

TEST(EngineTest, highPriceNoMatching){
    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 20, 80, 2022100816025058, "active"} //id, userid, vol, price, status, date
    };
    
    purchases = {
        {2, 2, 20, 60, 2022100816025058, "active"} //id, userid, vol, price, status, date
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    ASSERT_TRUE(deals.size() == 0);
}

TEST(EngineTest, highPriceNoMatchingManyOrders){
    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 20, 80, 2022100813025058, "active"}, //id, userid, vol, price, status, date
        {2, 2, 20, 80, 2022100812425058, "active"}, //id, userid, vol, price, status, date
        {3, 3, 20, 80, 2022100816225058, "active"}, //id, userid, vol, price, status, date
        {4, 4, 20, 80, 2022100216025058, "active"} //id, userid, vol, price, status, date
    };
    
    purchases = {
        {5, 5, 20, 60, 2022100816045058, "active"}, //id, userid, vol, price, status, date
        {6, 6, 20, 61, 2022100816035058, "active"}, //id, userid, vol, price, status, date
        {7, 7, 20, 62, 2022100816025058, "active"}, //id, userid, vol, price, status, date
        {8, 8, 20, 63, 2022100816015058, "active"} //id, userid, vol, price, status, date
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    ASSERT_TRUE(deals.size() == 0);
}


int main(int argc, char **argv)
{
    DataBase::getDB()->Pool().setConnectionParams("test_server", "server_role", "qwerty1234");

    handlerFactory.addHandler<HelHandler>("HelHandler");
    handlerFactory.addHandler<RegHandler>("RegHandler");
    handlerFactory.addHandler<OrderHandler>("OrderHandler");
    handlerFactory.addHandler<ActiveOrderHandler>("ActiveOrdersHandler");
    handlerFactory.addHandler<BalanceHandler>("BalanceHandler");
    handlerFactory.addHandler<SignInHandler>("SignInHandler");
    handlerFactory.addHandler<HistoryOrderHandler>("HistoryOrderHandler");
    handlerFactory.addHandler<DealsHandler>("DealsHandler");
    handlerFactory.addHandler<DeleteHandler>("DeleteHandler");
    handlerFactory.addHandler<QuotationsHandler>("QuotationsHandler");
  
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}