#pragma once
#include "Processor.h"

class ForGuiDeleteProcessor : public Processor{
   std::string process(tcp::socket& s);
   std::string appid;
public:
   void setAppId(int appid_);
};