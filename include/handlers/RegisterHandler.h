#pragma once

#include <string>
#include <json.hpp>
#include "RequestHandler.h"

class RegisterHandler : public RequestHandler{
    std::string makeReply(nlohmann::json j);
};