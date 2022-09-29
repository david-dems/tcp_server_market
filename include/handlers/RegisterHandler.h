#pragma once

#include <string>
#include <json.hpp>
#include "abstract_handler.h"

class RegisterHandler : public AbstractHandler{
    virtual std::string makeReply(nlohhman::json j);
};