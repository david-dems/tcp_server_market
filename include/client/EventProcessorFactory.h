#pragma once

#include <map>
#include <string>
#include <string_view>
#include "ProcessorCreator.h"
#include "Processor.h"

class EventProcessorFactory{
public:
    Processor* makeProcessor(std::string_view processorName);
    template <typename T>
    void addProcessor(std::string_view processorName);
    ~EventProcessorFactory();
private:
    std::map<std::string_view, ProcessorCreator*> creators_map;
};

template <typename T>
void EventProcessorFactory::addProcessor(std::string_view processorName){
    if(creators_map.find(processorName) == creators_map.end())
        creators_map[processorName] = new TemplateProcessorCreator<T>;
}
