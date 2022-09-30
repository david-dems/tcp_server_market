#pragma once

#include <string>
#include "json.hpp"

class RequestHandler{
    virtual std::string makeReply(nlohmann::json j) = 0;
};