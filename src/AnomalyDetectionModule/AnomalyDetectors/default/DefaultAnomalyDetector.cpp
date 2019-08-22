#include "DefaultAnomalyDetector.h"

#include <iostream>

// Constructors 
DefaultAnomalyDetector::DefaultAnomalyDetector(std::string identifyer)
{   
    this->identifyer = identifyer;
    std::cout << "[DefaultAnomalyDetector] Constructing {" << identifyer << "}"  << std::endl;
}

// Destroying
DefaultAnomalyDetector::~DefaultAnomalyDetector()
{
    std::cout << "[DefaultAnomalyDetector] Destroying {" << identifyer << "}"  << std::endl;
}

void DefaultAnomalyDetector::init()
{
    std::cout << "[DefaultAnomalyDetector] Initializing {" << identifyer << "}"  << std::endl;



    std::cout << "[DefaultAnomalyDetector] Done initializing {" << identifyer << "}"  << std::endl;
}

bool DefaultAnomalyDetector::detectWarnning()
{
    bool result = false;

    if(sharedMemory->anomalyDetector->thresholdWarning < sharedMemory->anomalyDetector->distance->data[sharedMemory->anomalyDetector->distance->index]){
        result = true;
    }

    return result;
}

bool DefaultAnomalyDetector::detectAlarm()
{
    bool result = false;

    if(sharedMemory->anomalyDetector->thresholdAlram < sharedMemory->anomalyDetector->distance->data[sharedMemory->anomalyDetector->distance->index]){
        result = true;
    }

    return result;
}