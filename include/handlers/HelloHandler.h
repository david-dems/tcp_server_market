#pragma once

#include <string>
#include <json.hpp>
#include "RequestHandler.h"
#include "DataBase.h"

class HelHandler : public RequestHandler{
    public:
    std::string makeReply(nlohmann::json j);
};