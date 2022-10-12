#include "EventProcessorFactory.h"

Processor* EventProcessorFactory::makeProcessor(std::string_view processorName){
    if (creators_map.find(processorName) != creators_map.end())
        return creators_map[processorName]->create();
    return nullptr;
}

EventProcessorFactory::~EventProcessorFactory(){
    for (auto val : creators_map)
        delete val.second;
}