#pragma once

#include <map>
#include <string>
#include <typeinfo>
#include <string_view>

#include "RequestHandler.h"
#include "HandlerCreator.h"
#include "TemplateHandlerCreator.h"

class HandlerFactory{
public:
    template <typename T>
    void addHandler(std::string_view HandlerName);
    RequestHandler* make(std::string_view HandlerName);
    ~HandlerFactory();
private:
    std::map<std::string_view, HandlerCreator*> creator_map;
};

template <typename T>
void HandlerFactory::addHandler(std::string_view HandlerName){
    if(creator_map.find(HandlerName) == creator_map.end())
        creator_map[HandlerName] = new TemplateHandlerCreator<T>;
}


RequestHandler* HandlerFactory::make(std::string_view HandlerName){
    return creator_map[HandlerName]->create();
}

HandlerFactory::~HandlerFactory(){
    for(auto val : creator_map){
        delete val.second;
    }
}