#pragma once

#include "HandlerCreator.h"

class RequestHandler;

template <class T>
class TemplateHandlerCreator : public HandlerCreator{
public:
    virtual RequestHandler* create() {return new T;};
};
