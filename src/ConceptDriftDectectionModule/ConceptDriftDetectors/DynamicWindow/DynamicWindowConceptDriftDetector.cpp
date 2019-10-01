#include "DynamicWindowConceptDriftDetector.h"

#include <iostream>

// Constructors 
DynamicWindowConceptDriftDetector::DynamicWindowConceptDriftDetector(std::string identifyer)
{   
    this->identifyer = identifyer;
    std::cout << "[DynamicWindowConceptDriftDetector] Constructing {" << identifyer << "}"  << std::endl;
}

// Destroying
DynamicWindowConceptDriftDetector::~DynamicWindowConceptDriftDetector()
{
    std::cout << "[DynamicWindowConceptDriftDetector] Destroying {" << identifyer << "}"  << std::endl;
}

void DynamicWindowConceptDriftDetector::init()
{
    std::cout << "[DynamicWindowConceptDriftDetector] Initializing {" << identifyer << "}"  << std::endl;

    windowSize = sharedMemory->conceptDriftDetector->distance->size - 1;

    std::cout << "[DynamicWindowConceptDriftDetector] Done initializing {" << identifyer << "}"  << std::endl;
}

double DynamicWindowConceptDriftDetector::calculateMean()
{
 // calculates Mean for distance [0 n-1 , n] 1 to n, 0 is left out
    double mean = 0;
    for(int i=0; i < this->windowSize; i++){
        mean += sharedMemory->conceptDriftDetector->distance->data[sharedMemory->conceptDriftDetector->distance->size -1 -i];
    }
    mean = mean/this->windowSize;
    return mean;

};

double DynamicWindowConceptDriftDetector::calculateSD(double mean)
{
 // calculates Standard deviation for distance [0 n-1 , n] 1 to n, 0 is left out

    double finalSD = 0;
    double SD;
    for(int i=0; i < this->windowSize; i++){
        SD += sharedMemory->conceptDriftDetector->distance->data[sharedMemory->conceptDriftDetector->distance->size -1 -i];
        SD = mean - SD;
        if(SD < 0){
            SD = 0 - SD;
        }
        finalSD += SD;
    }
    finalSD = finalSD/this->windowSize;
    return finalSD;
};


int DynamicWindowConceptDriftDetector::getNewConceptCount()
{
    if(!sharedMemory->conceptDriftDetector->warning->data[sharedMemory->conceptDriftDetector->warning->index]){
        return -1;
    }
    int newConceptCount = 1;
    int i = sharedMemory->conceptDriftDetector->warning->index;
    while(i >= 0 &&
        sharedMemory->conceptDriftDetector->warning->data[i]){
            newConceptCount += 1;
            i -= 1;
        }
    return newConceptCount;
}

int DynamicWindowConceptDriftDetector::detect()
{
    int newConceptCount = -1;
    double mean = this->calculateMean();
    double SD = this->calculateSD(mean);
    double metric = mean + SD;

    if(metric >= sharedMemory->conceptDriftDetector->thresholdAlram){
        // Alarm
        newConceptCount = this->getNewConceptCount();

        sharedMemory->conceptDriftDetector->warning->enQueue(true);
        sharedMemory->conceptDriftDetector->alarm->enQueue(true);
    } else if(metric >= sharedMemory->conceptDriftDetector->thresholdWarning){
        // Warnning
        sharedMemory->conceptDriftDetector->warning->enQueue(true);
        sharedMemory->conceptDriftDetector->alarm->enQueue(false);
    } else {
        // None
        sharedMemory->conceptDriftDetector->warning->enQueue(false);
        sharedMemory->conceptDriftDetector->alarm->enQueue(false);

    }

    return newConceptCount;
}