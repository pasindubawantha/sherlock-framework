#include "ConfidenceIntervalAnomalyThresholdSetter.h"

#include <iostream>

// Constructors 
ConfidenceIntervalAnomalyThresholdSetter::ConfidenceIntervalAnomalyThresholdSetter(std::string identifyer)
{   
    this->identifyer = identifyer;
    std::cout << "[ConfidenceIntervalAnomalyThresholdSetter] Constructing {" << identifyer << "}"  << std::endl;
}

ConfidenceIntervalAnomalyThresholdSetter::ConfidenceIntervalAnomalyThresholdSetter(std::string identifyer, double warningAlpha, double alarmAlpha){   
    this->identifyer = identifyer;
    this->warningAlpha = warningAlpha;
    this->alarmAlpha = alarmAlpha;


    std::cout << "[ConfidenceIntervalAnomalyThresholdSetter] Constructing {" << identifyer << "}"  << std::endl;
}

// Destroying
ConfidenceIntervalAnomalyThresholdSetter::~ConfidenceIntervalAnomalyThresholdSetter()
{
    std::cout << "[ConfidenceIntervalAnomalyThresholdSetter] Destroying {" << identifyer << "}"  << std::endl;
}

void ConfidenceIntervalAnomalyThresholdSetter::init()
{
    std::cout << "[ConfidenceIntervalAnomalyThresholdSetter] Initializing {" << identifyer << "}"  << std::endl;

    windowSize = sharedMemory->anomalyDetector->distance->size - 1;

    warningThreshold = -1;
    alarmThreshold = -1;

    std::cout << "[ConfidenceIntervalAnomalyThresholdSetter] Done initializing {" << identifyer << "}"  << std::endl;
}

double ConfidenceIntervalAnomalyThresholdSetter::calculateMean()
{
 // calculates Mean for distance [0 n-1 , n] 0 to n-1, n is left out
    double mean = 0;
    for(int i=0; i < this->windowSize; i++){
        mean += sharedMemory->anomalyDetector->distance->data[sharedMemory->anomalyDetector->distance->size -2 -i];
    }
    mean = mean/this->windowSize;
    return mean;

};

double ConfidenceIntervalAnomalyThresholdSetter::calculateSD(double mean)
{
 // calculates Standard deviation for distance [0 n-1 , n] 0 to n-1, n is left out

    double finalSD = 0;
    double SD;
    for(int i=0; i < this->windowSize; i++){
        SD += sharedMemory->anomalyDetector->distance->data[sharedMemory->anomalyDetector->distance->size -2 -i];
        SD = mean - SD;
        if(SD < 0){
            SD = 0 - SD;
        }
        finalSD += SD;
    }
    finalSD = finalSD/this->windowSize;
    return finalSD;
};

// void ConfidenceIntervalAnomalyThresholdSetter::setFirstMeanAndSD(bool firstMeanAndSD){
//     this->firstMeanAndSD = firstMeanAndSD;
// }

double ConfidenceIntervalAnomalyThresholdSetter::getWarningThreshold()
{
    //for distance [0 n-1 , n] 0 to n-1 used for calculateing minSD and minMean, n is left out
    double SD;
    double mean;
    firstMeanAndSD = sharedMemory->anomalyDetector->training;
    if((!sharedMemory->profiler->training) &&
        (sharedMemory->profiler->profiledCount + sharedMemory->anomalyDetector->inWindowSize) >= windowSize &&
        sharedMemory->anomalyDetector->distance->index +2 >= windowSize){
        
        mean = this->calculateMean();
        SD = this->calculateSD(mean);
        
        if(firstMeanAndSD){
            this->minMean = mean;
            this->minSD = SD;
            this->firstMeanAndSD = false;
        } 
        if(mean < this->minMean){
            this->minMean = mean;
        }
        if(SD < this->minSD){
            this->minSD = SD;
        }

        alarmThreshold = this->minMean + (this->alarmAlpha * this->minSD);
        warningThreshold = this->minMean + (this->warningAlpha * this->minSD);
        

        sharedMemory->anomalyDetector->training = false;

    }
    

    return warningThreshold;
}

double ConfidenceIntervalAnomalyThresholdSetter::getAlarmThreshold()
{
    return alarmThreshold;
}