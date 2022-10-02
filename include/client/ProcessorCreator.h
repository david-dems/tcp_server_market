#pragma once

class Processor;

class ProcessorCreator{
public:
    virtual Processor* create() = 0;
};

template <class T>
class TemplateProcessorCreator : public ProcessorCreator{
    Processor* create(){
        return new T;
    }
};