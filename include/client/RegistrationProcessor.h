#pragma once
#include "Processor.h"

class RegistrationProcessor : public Processor{
   std::string process(tcp::socket& s);
};