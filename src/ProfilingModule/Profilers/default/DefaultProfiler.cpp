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

void DefaultProfiler::init(int historySize)
{
    this->historySize = historySize;
}

double* DefaultProfiler::profile(const double *history, int history_current_index) 
{
    std::cout << history[history_current_index] << std::endl;

    double *profile = new double[historySize];
    return profile;
}

std::string DefaultProfiler::getIdentifier()
{
    return this->identifyer;
}