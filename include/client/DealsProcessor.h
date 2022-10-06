#pragma once
#include "Processor.h"

class DealsProcessor : public Processor{
   std::string process(tcp::socket& s);
};