#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>

static short port = 5556;

namespace Requests
{
    static std::string Registration = "Reg";
    static std::string Hello = "Hel";
    static std::string Order = "Order";
    static std::string ActiveOrders = "ActiveOrders";
    static std::string Balance = "Balance";
    static std::string SignIn = "SignIn";
    static std::string History = "HistoryOrder";
    static std::string Deals = "Deals";
    static std::string Delete = "Delete";
    static std::string Quotations = "Quotations";
}

#endif //CLIENSERVERECN_COMMON_HPP
