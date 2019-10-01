#include "ConfidenceIntervalAnomalyDetector.h"

#include <iostream>

// Constructors 
ConfidenceIntervalAnomalyDetector::ConfidenceIntervalAnomalyDetector(std::string identifyer)
{   
    this->identifyer = identifyer;
    std::cout << "[ConfidenceIntervalAnomalyDetector] Constructing {" << identifyer << "}"  << std::endl;
}

// Destroying
ConfidenceIntervalAnomalyDetector::~ConfidenceIntervalAnomalyDetector()
{
    std::cout << "[ConfidenceIntervalAnomalyDetector] Destroying {" << identifyer << "}"  << std::endl;
}

void ConfidenceIntervalAnomalyDetector::init()
{
    std::cout << "[ConfidenceIntervalAnomalyDetector] Initializing {" << identifyer << "}"  << std::endl;

    windowSize = sharedMemory->anomalyDetector->distance->size - 1;

    std::cout << "[ConfidenceIntervalAnomalyDetector] Done initializing {" << identifyer << "}"  << std::endl;
}

double ConfidenceIntervalAnomalyDetector::calculateMean()
{
 // calculates Mean for distance [0 n-1 , n] 1 to n, 0 is left out
    double mean = 0;
    for(int i=0; i < this->windowSize; i++){
        mean += sharedMemory->anomalyDetector->distance->data[sharedMemory->anomalyDetector->distance->size -1 -i];
    }
    mean = mean/this->windowSize;
    return mean;

};

double ConfidenceIntervalAnomalyDetector::calculateSD(double mean)
{
 // calculates Standard deviation for distance [0 n-1 , n] 1 to n, 0 is left out

    double finalSD = 0;
    double SD;
    for(int i=0; i < this->windowSize; i++){
        SD += sharedMemory->anomalyDetector->distance->data[sharedMemory->anomalyDetector->distance->size -1 -i];
        SD = mean - SD;
        if(SD < 0){
            SD = 0 - SD;
        }
        finalSD += SD;
    }
    finalSD = finalSD/this->windowSize;
    return finalSD;
};



bool ConfidenceIntervalAnomalyDetector::detectWarnning()
{
    double mean = this->calculateMean();
    double SD = this->calculateSD(mean);
    double metric = mean + SD;

    bool result = false;
    
    if(metric >= sharedMemory->anomalyDetector->thresholdWarning){
        // Warnning
        result = true;
    }
    
    return result;
}

bool ConfidenceIntervalAnomalyDetector::detectAlarm()
{
    double mean = this->calculateMean();
    double SD = this->calculateSD(mean);
    double metric = mean + SD;

    bool result = false;


    if(metric >= sharedMemory->anomalyDetector->thresholdAlram){
        // Alarm
        result = true;
    }

    return result;
}
