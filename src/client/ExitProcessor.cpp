#include "ExitProcessor.h"

std::string ExitProcessor::process(tcp::socket& s){
    exit(0); 
    return "";
}