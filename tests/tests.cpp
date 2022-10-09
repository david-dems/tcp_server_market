#include <gtest/gtest.h>

#include "Engine.h"


bool operator==(const deal& l, const deal& r){
    if (
        l.sellerid == r.sellerid && l.buyerid == r.buyerid &&
    l   .vol == r.vol && l.price == r.price &&
        l.s_status == r.s_status && l.p_status == r.p_status &&
        l.s_appid == r.s_appid && l.p_appid == r.p_appid
    )
        return true;
    else
        return false;
}


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

TEST(EngineTest, matchTwoOrders){
    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 20, 60, 2022100815025058, "active"} //id, userid, vol, price, status, date
    };
    
    purchases = {
        {2, 2, 20, 80, 2022100816025058, "active"} //id, userid, vol, price, status, date
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    q = {
        {1, 2, 20, 60, "closed", "closed", 1, 2},
    };

    ASSERT_TRUE(deals == q);
}

TEST(EngineTest, matchTwoOrdersCloseBuyer){
    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 30, 60, 2022100815025058, "active"} //id, userid, vol, price, status, date
    };
    
    purchases = {
        {2, 2, 20, 80, 2022100816025058, "active"} //id, userid, vol, price, status, date
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    q = {
        {1, 2, 20, 60, "active", "closed", 1, 2},
    };

    ASSERT_TRUE(deals == q);
}

TEST(EngineTest, matchTwoOrdersCloseSeller){

    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 20, 60, 2022100815025058, "active"} //id, userid, vol, price, status, date
    };
    
    purchases = {
        {2, 2, 30, 80, 2022100816025058, "active"} //id, userid, vol, price, status, date
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;
    q = {
        {1, 2, 20, 60, "closed", "active", 1, 2},
    };

    ASSERT_TRUE(deals == q);
}

TEST(EngineTest, matchManyOrdersDiffirentUsers){
    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 23, 65, 2022100816024444, "active"},
        {3, 3, 20, 70, 2022100817024444, "active"},
        {5, 5, 30, 74, 2022100817034444, "active"} 
    };

    purchases = {
        {2, 2, 23, 80, 2022100815024444, "active"},
        {4, 4, 20, 75, 2022100816024444, "active"},
        {6, 6, 35, 74, 2022100817004444, "active"} 
    };
    
    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;

    q = {
        {1, 2, 23, 80, "closed", "closed", 1, 2},
        {3, 4, 20, 75, "closed", "closed", 3, 4},
        {5, 6, 30, 74, "closed", "active", 5, 6}
    };

    ASSERT_TRUE(deals == q);
}

TEST(EngineTest, matchOneSellerCloseAllBuyers){

    std::vector<order> sales, purchases;
    sales = {
        {1, 1, 100, 60, 2022100916024444, "active"},
    };

    purchases = {
        {2, 2, 20, 85, 2022100815020444, "active"},
        {3, 3, 20, 80, 2022100815024444, "active"},
        {4, 4, 25, 75, 2022100816024444, "active"},
        {5, 5, 15, 74, 2022100816024444, "active"},
        {6, 6, 5,  73, 2022100817004444, "active"} 
    };
    
    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;

    q = {
        {1, 2, 20, 85, "active", "closed", 1, 2},
        {1, 3, 20, 80, "active", "closed", 1, 3},
        {1, 4, 25, 75, "active", "closed", 1, 4},
        {1, 5, 15, 74, "active", "closed", 1, 5},
        {1, 6, 5,  73, "active", "closed", 1, 6}
    };

    ASSERT_TRUE(deals == q);
}

TEST(EngineTest, matchOneBuyerCloseAllSellers){
    std::vector<order> sales, purchases;
    
    sales = {
        {6, 6, 5,  73, 2022100817004444, "active"},
        {5, 5, 15, 74, 2022100816024444, "active"},
        {4, 4, 25, 75, 2022100816024444, "active"},
        {3, 3, 20, 80, 2022100815024444, "active"},
        {2, 2, 20, 85, 2022100815020444, "active"}
    };
   
    purchases = {
        {1, 1, 100, 90, 2022100916024444, "active"},
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;

    q = {
        {6, 1, 5, 73, "closed", "active", 6, 1},
        {5, 1, 15, 74, "closed", "active", 5, 1},
        {4, 1, 25, 75, "closed", "active", 4, 1},
        {3, 1, 20, 80, "closed", "active", 3, 1},
        {2, 1, 20,  85, "closed", "active", 2, 1}
    };

    ASSERT_TRUE(deals == q);
}

TEST(EngineTest, sameSellerAndBuyerWithMatching){
    std::vector<order> sales, purchases;
    
    sales = {
        {6, 6, 5,  73, 2022100817004444, "active"},
        {5, 5, 15, 74, 2022100816024444, "active"},
        {4, 4, 25, 75, 2022100816024444, "active"},
        {3, 3, 20, 80, 2022100815024444, "active"},
        {2, 2, 20, 85, 2022100815020444, "active"}
    };
   
    purchases = {
        {7, 6, 5,  93, 2022100717000444, "active"},
        {8, 6, 5,  93, 2022100717004044, "active"},
        {1, 1, 100, 90, 2022100916024444, "active"},
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;

    q = {
        {6, 1, 5, 73, "closed", "active", 6, 1},
        {5, 6, 5, 93, "active", "closed", 5, 7},
        {5, 6, 5, 93, "active", "closed", 5, 8},
        {5, 1, 5, 74, "closed", "active", 5, 1},
        {4, 1, 25, 75, "closed", "active", 4, 1},
        {3, 1, 20, 80, "closed", "active", 3, 1},
        {2, 1, 20, 85, "closed", "active", 2, 1},

    };

    ASSERT_TRUE(deals == q);
}

TEST(EngineTest, partiallyComplitedOrders){
    std::vector<order> sales, purchases;
    
    sales = {
        {1, 1, 10, 60, 2022100817004444, "active"},
        {4, 4, 15, 65, 2022100817104444, "active"},
    };
   
    purchases = {
        {2, 2, 5,  80, 2022100717000444, "active"},
        {3, 3, 15, 79, 2022100717010444, "active"},
        {5, 5, 10, 78, 2022100717020444, "active"},
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;

    q = {
        {1, 2, 5, 80, "active", "closed", 1, 2},
        {1, 3, 5, 79, "closed", "active", 1, 3},
        {4, 3, 10, 79, "active", "closed", 4, 3},
        {4, 5, 5, 78, "closed", "active", 4, 5},

    };

    ASSERT_TRUE(deals == q);
}

TEST(EngineTest, partiallyComplitedOrdersSameUser){
    std::vector<order> sales, purchases;
    
    sales = {
        {1, 1, 10, 60, 2022100817004444, "active"},
        {4, 4, 15, 65, 2022100817104444, "active"},
        {6, 6, 10, 67, 2022100817104444, "active"},
        {8, 8, 10, 68, 2022100817104444, "active"},
    };
   
    purchases = {
        {2, 2, 5,  80, 2022100717000444, "active"},
        {3, 3, 15, 79, 2022100717010444, "active"},
        {5, 5, 10, 78, 2022100717020444, "active"},
        {10, 6, 5, 77, 2022100837004444, "active"},
        {11, 6, 5, 76, 2022100837004444, "active"},
        {12, 6, 5, 75, 2022100837004444, "active"},
        {7, 7, 10, 72, 2022100717020444, "active"},
        {9, 9, 10, 71, 2022100717020444, "active"},
    };

    MatchingEngine e;
    auto deals = e.processDeals(sales, purchases);
    std::vector<deal> q;

    q = {
        {1, 2, 5, 80, "active", "closed", 1, 2},
        {1, 3, 5, 79, "closed", "active", 1, 3},
        {4, 3, 10, 79, "active", "closed", 4, 3},
        {4, 5, 5, 78, "closed", "active", 4, 5},
        {6, 5, 5, 78, "active", "closed", 6, 5},
        {6, 7, 5, 72, "closed", "active", 6, 7},
        {8, 6, 5, 68, "active", "closed", 8, 10},
        {8, 6, 5, 68, "closed", "closed", 8, 11},

    };

    ASSERT_TRUE(deals == q);
}


int main(int argc, char **argv)
{

  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}