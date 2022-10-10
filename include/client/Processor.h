#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include "json.hpp"
#include "Common.hpp"
#include <stdexcept>

using boost::asio::ip::tcp;

class Processor{
public:
   virtual std::string process(tcp::socket& s) = 0; 
   void setId(std::string id){my_id = id;};

protected:   
   std::string ProcessErrors(nlohmann::json j){
      if (j["Error"] == nullptr)
         return "ok";

      return j["Error"].get<std::string>();
   }

   void SendMessage(
      tcp::socket& aSocket,
      nlohmann::json &j
   )
   {
      std::string request = j.dump();
      boost::asio::write(aSocket, boost::asio::buffer(request, request.size()));
   }
   
   nlohmann::json ReadMessage(tcp::socket& aSocket){
      boost::asio::streambuf b;
      boost::asio::read_until(aSocket, b, "\n");
      std::istream is(&b);
      std::string line(std::istreambuf_iterator<char>(is), {});
      auto j = nlohmann::json::parse(line);
      std::string error = ProcessErrors(j);
      if (error != "ok"){
         throw std::logic_error(error);
      }
      return j;
   }

   std::string my_id;
};