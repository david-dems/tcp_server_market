#pragma once
#include "Processor.h"

class ActiveOrdersProcessor : public Processor{
   std::string process(tcp::socket& s);
};