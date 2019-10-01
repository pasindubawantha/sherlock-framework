#include "ConfidenceIntervalConceptThresholdSetter.h"

#include <iostream>

// Constructors 
ConfidenceIntervalConceptThresholdSetter::ConfidenceIntervalConceptThresholdSetter(std::string identifyer)
{   
    this->identifyer = identifyer;
    std::cout << "[ConfidenceIntervalConceptThresholdSetter] Constructing {" << identifyer << "}"  << std::endl;
}

ConfidenceIntervalConceptThresholdSetter::ConfidenceIntervalConceptThresholdSetter(std::string identifyer, double warningAlpha, double alarmAlpha){   
    this->identifyer = identifyer;
    this->warningAlpha = warningAlpha;
    this->alarmAlpha = alarmAlpha;


    std::cout << "[ConfidenceIntervalConceptThresholdSetter] Constructing {" << identifyer << "}"  << std::endl;
}

// Destroying
ConfidenceIntervalConceptThresholdSetter::~ConfidenceIntervalConceptThresholdSetter()
{
    std::cout << "[ConfidenceIntervalConceptThresholdSetter] Destroying {" << identifyer << "}"  << std::endl;
}

void ConfidenceIntervalConceptThresholdSetter::init()
{
    std::cout << "[ConfidenceIntervalConceptThresholdSetter] Initializing {" << identifyer << "}"  << std::endl;

    windowSize = sharedMemory->conceptDriftDetector->distance->size - 1;

    warningThreshold = -1;
    alarmThreshold = -1;

    std::cout << "[ConfidenceIntervalConceptThresholdSetter] Done initializing {" << identifyer << "}"  << std::endl;
}

double ConfidenceIntervalConceptThresholdSetter::calculateMean()
{
 // calculates Mean for distance [0 n-1 , n] 0 to n-1, n is left out
    double mean = 0;
    for(int i=0; i < this->windowSize; i++){
        mean += sharedMemory->conceptDriftDetector->distance->data[sharedMemory->conceptDriftDetector->distance->size -2 -i];
    }
    mean = mean/this->windowSize;
    return mean;

};

double ConfidenceIntervalConceptThresholdSetter::calculateSD(double mean)
{
 // calculates Standard deviation for distance [0 n-1 , n] 0 to n-1, n is left out

    double finalSD = 0;
    double SD;
    for(int i=0; i < this->windowSize; i++){
        SD += sharedMemory->conceptDriftDetector->distance->data[sharedMemory->conceptDriftDetector->distance->size -2 -i];
        SD = mean - SD;
        if(SD < 0){
            SD = 0 - SD;
        }
        finalSD += SD;
    }
    finalSD = finalSD/this->windowSize;
    return finalSD;
};

// void ConfidenceIntervalConceptThresholdSetter::setFirstMeanAndSD(bool firstMeanAndSD){
//     this->firstMeanAndSD = firstMeanAndSD;
// }

double ConfidenceIntervalConceptThresholdSetter::getWarningThreshold()
{
    //for distance [0 n-1 , n] 0 to n-1 used for calculateing minSD and minMean, n is left out
    double SD;
    double mean;
    firstMeanAndSD = sharedMemory->conceptDriftDetector->training;
    if((!sharedMemory->profiler->training) &&
        (sharedMemory->profiler->profiledCount + sharedMemory->conceptDriftDetector->inWindowSize) >= windowSize &&
        sharedMemory->conceptDriftDetector->distance->index +2 >= windowSize){
        
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
        

        sharedMemory->conceptDriftDetector->training = false;

    }
    

    return warningThreshold;
}

double ConfidenceIntervalConceptThresholdSetter::getAlarmThreshold()
{
    return alarmThreshold;
}