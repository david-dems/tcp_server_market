#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>

static short port = 5555;

namespace Requests
{
    static std::string Registration = "Reg";
    static std::string Hello = "Hel";
    static std::string Order = "Order";
    static std::string ActiveOrders = "ActiveOrders";
    static std::string Balance = "Balance";
}

#endif //CLIENSERVERECN_COMMON_HPP
