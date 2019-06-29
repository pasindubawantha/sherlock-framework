#include <iostream>
#include "logger.hpp"

int 
logger::log(std::string contex, std::string message)
{
    std::cout << "[" << contex << "] " << message << std::endl;
    return 0;
}