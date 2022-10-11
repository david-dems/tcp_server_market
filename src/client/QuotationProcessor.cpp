#include "QuotationProcessor.h"


std::string QuotationProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::Quotations;

    try{
        SendMessage(s, req);
        auto rep = ReadMessage(s);

        return std::move(rep["Quotation"].get<std::string>());
    } catch(std::exception const& ex){
        std::cerr << ex.what() << std::endl;
        return "null";
    }
}