#pragma once
#include "Processor.h"

class ForGuiLoginProcessor : public Processor{
   std::string process(tcp::socket& s);
   std::string login_, password_;
public:
   void setLP(std::string login, std::string password);
};