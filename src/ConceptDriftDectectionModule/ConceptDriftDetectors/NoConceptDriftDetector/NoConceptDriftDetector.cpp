#include "NoConceptDriftDetector.h"

#include <iostream>

// Constructors 
NoConceptDriftDetector::NoConceptDriftDetector(std::string identifyer)
{   
    this->identifyer = identifyer;
    std::cout << "[NoConceptDriftDetector] Constructing {" << identifyer << "}"  << std::endl;
}

// Destroying
NoConceptDriftDetector::~NoConceptDriftDetector()
{
    std::cout << "[NoConceptDriftDetector] Destroying {" << identifyer << "}"  << std::endl;
}

void NoConceptDriftDetector::init()
{
    std::cout << "[NoConceptDriftDetector] Initializing {" << identifyer << "}"  << std::endl;


    std::cout << "[NoConceptDriftDetector] Done initializing {" << identifyer << "}"  << std::endl;
}



int NoConceptDriftDetector::detect()
{
    int newConceptCount = -1;
    return newConceptCount;
}