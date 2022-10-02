#pragma once
#include "Processor.h"

class BalanceProcessor : public Processor{
   std::string process(tcp::socket& s);
};