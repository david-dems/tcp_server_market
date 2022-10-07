#pragma once
#include "Processor.h"

class QuotationProcessor : public Processor{
   std::string process(tcp::socket& s);
};