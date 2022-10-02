#pragma once
#include "Processor.h"

class LoginProcessor : public Processor{
   std::string process(tcp::socket& s);
};