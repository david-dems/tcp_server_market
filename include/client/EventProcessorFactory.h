#pragma once

#include <map>
#include <string>
#include "ProcessorCreator.h"
#include "Processor.h"

class EventProcessorFactory{
public:
    Processor* makeProcessor(std::string processorName);
    template <typename T>
    void addProcessor(std::string processorName);
    ~EventProcessorFactory();
private:
    std::map<std::string, ProcessorCreator*> creators_map;
};

template <typename T>
void EventProcessorFactory::addProcessor(std::string processorName){
    if(creators_map.find(processorName) == creators_map.end())
        creators_map[processorName] = new TemplateProcessorCreator<T>;
}


Processor* EventProcessorFactory::makeProcessor(std::string processorName){
    if (creators_map.find(processorName) != creators_map.end())
        return creators_map[processorName]->create();
    return nullptr;
}

EventProcessorFactory::~EventProcessorFactory(){
    for (auto val : creators_map)
        delete val.second;
}