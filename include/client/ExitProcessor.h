#pragma once
#include "Processor.h"

class ExitProcessor : public Processor{
   std::string process(tcp::socket& s);
};