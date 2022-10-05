#pragma once
#include "Processor.h"

class HistoryProcessor : public Processor{
   std::string process(tcp::socket& s);
};