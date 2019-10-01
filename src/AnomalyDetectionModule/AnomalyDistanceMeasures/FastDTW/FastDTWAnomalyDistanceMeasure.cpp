#include "FastDTWAnomalyDistanceMeasure.h"

#include <iostream>

// Constructors 
FastDTWAnomalyDistanceMeasure::FastDTWAnomalyDistanceMeasure(std::string identifyer)
{   
    this->identifyer = identifyer;
    std::cout << "[FastDTWAnomalyDistanceMeasure] Constructing {" << identifyer << "}"  << std::endl;
}

// Destroying
FastDTWAnomalyDistanceMeasure::~FastDTWAnomalyDistanceMeasure()
{
    std::cout << "[FastDTWAnomalyDistanceMeasure] Destroying {" << identifyer << "}"  << std::endl;
}

void FastDTWAnomalyDistanceMeasure::init()
{
    std::cout << "[FastDTWAnomalyDistanceMeasure] Initializing {" << identifyer << "}"  << std::endl;
    this->dtw = new DTW();
    std::cout << "[FastDTWAnomalyDistanceMeasure] Done initializing {" << identifyer << "}"  << std::endl;
}

double FastDTWAnomalyDistanceMeasure::measureDistance() 
{
    
    double distance = -1;
    int radius = 2;
    
    if(sharedMemory->profiler->profile->index >= sharedMemory->anomalyDetector->inWindowSize &&
    sharedMemory->profiler->profiledCount > sharedMemory->anomalyDetector->inWindowSize){
        Eigen::VectorXd expectedVec = Eigen::VectorXd::Zero(sharedMemory->anomalyDetector->inWindowSize);
        Eigen::VectorXd predictedVec = Eigen::VectorXd::Zero(sharedMemory->anomalyDetector->inWindowSize);

        // filling the values to compare the in window
        for (int i=0; i<sharedMemory->anomalyDetector->inWindowSize; i++){
            expectedVec(i) = sharedMemory->history->data[sharedMemory->history->index + 1 - sharedMemory->anomalyDetector->inWindowSize + i];
            predictedVec(i) = sharedMemory->profiler->profile->data[sharedMemory->profiler->profile->index - sharedMemory->anomalyDetector->inWindowSize + i];// ignore last prediction 
        }

        // distance = this->dtw->fastDTW(expectedVec, predictedVec, radius);
        distance = this->dtw->getSimilarity(expectedVec, predictedVec);
        
        // for debugging
        // for (int i=0; i<sharedMemory->anomalyDetector->inWindowSize; i++){
        //     std::cout << "r["<<i<<"]p["<<i<<"] "<< expectedVec(i) <<" | "<< predictedVec(i) << std::endl;
        // }
    }
    
    
    return distance;
}