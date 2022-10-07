#pragma once
#include "Processor.h"

class DeleteProcessor : public Processor{
   std::string process(tcp::socket& s);
};