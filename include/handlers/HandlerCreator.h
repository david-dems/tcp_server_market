#pragma once

class RequestHandler;

class HandlerCreator{
public:
    virtual RequestHandler* create() = 0;
};
