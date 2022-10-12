#pragma once
#include "Processor.h"

class ForGuiOrderProcessor : public Processor{
   std::string process(tcp::socket& s);
   std::string vol, price, dir;
public:
   void setVolPriceDir(std::string vol_, std::string price_, std::string dir_);
};