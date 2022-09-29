#pragma once

#include <string>
#include "json.hpp"

class AbstractHandler{
    virtual std::string makeReply(nlohmann::json j) = 0;
};