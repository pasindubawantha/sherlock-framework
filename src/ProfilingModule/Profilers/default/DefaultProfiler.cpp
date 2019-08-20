#include "DefaultProfiler.h"

#include <iostream>

// Constructors 
DefaultProfiler::DefaultProfiler(std::string identifyer)
{   
    this->identifyer = identifyer;
    std::cout << "[DafaultProfiler] Constructing {" << identifyer << "}"  << std::endl;
}

// Destroying
DefaultProfiler::~DefaultProfiler()
{
    std::cout << "[DefaultProfiler] Destroying {" << identifyer << "}"  << std::endl;
}

void DefaultProfiler::init()
{
    this->historySize = historySize;
}

double DefaultProfiler::profile() 
{
    std::cout << this->sharedMemory->history->data[this->sharedMemory->history->index] << std::endl;

    double profile = this->sharedMemory->history->data[this->sharedMemory->history->index];
    return profile;
}