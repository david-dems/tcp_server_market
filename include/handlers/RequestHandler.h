#pragma once

#include <string>
#include "json.hpp"

class RequestHandler{
    public:
    virtual std::string makeReply(nlohmann::json j) = 0;
};