#pragma once
#include "Processor.h"

class MakeOrderProcessor : public Processor{
   std::string process(tcp::socket& s);
};