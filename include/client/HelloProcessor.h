#pragma once
#include "Processor.h"

class HelloProcessor : public Processor{
   std::string process(tcp::socket& s);
};