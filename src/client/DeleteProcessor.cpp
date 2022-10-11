#include "DeleteProcessor.h"



std::string DeleteProcessor::process(tcp::socket& s){
    nlohmann::json req;
    req["UserId"] = my_id;
    req["ReqType"] = Requests::ActiveOrders;
    
    try {
        SendMessage(s, req);
        auto resp = ReadMessage(s);
    
        auto idVolPriceDir = resp["IdVolPriceDir"].get<std::vector<std::tuple<std::string, std::string, std::string, std::string>>>();

        std::cout << "Choose order to delete: " << std::endl;
    
        for (auto& [id, vol, price, dir] : idVolPriceDir){
            std::cout << "id: " << id << " USD: " + vol + " Price: " + price + " " + dir + "\n";
        }
        std::string del_id;
        std::cin >> del_id;

        nlohmann::json req_del;
        req_del["UserId"] = my_id;
        req_del["ReqType"] = Requests::Delete;
        req_del["AppId"] = del_id;
    
        SendMessage(s, req_del);
        auto reply = ReadMessage(s);

        return std::move(reply["Message"].get<std::string>());
    } catch (std::exception const& ex){
        std::cerr << ex.what() << std::endl;
        return "null";
    }
}
