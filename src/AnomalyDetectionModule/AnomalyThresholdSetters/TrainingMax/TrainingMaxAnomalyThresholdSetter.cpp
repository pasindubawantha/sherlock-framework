#include "TrainingMaxAnomalyThresholdSetter.h"

#include <iostream>

// Constructors 
TrainingMaxAnomalyThresholdSetter::TrainingMaxAnomalyThresholdSetter(std::string identifyer)
{   
    this->identifyer = identifyer;
    std::cout << "[TrainingMaxAnomalyThresholdSetter] Constructing {" << identifyer << "}"  << std::endl;
}

TrainingMaxAnomalyThresholdSetter::TrainingMaxAnomalyThresholdSetter(std::string identifyer, double maxMultiplierWarning, double maxMultiplierAlarm){   
    this->identifyer = identifyer;
    this->maxMultiplierWarning = maxMultiplierWarning;
    this->maxMultiplierAlarm = maxMultiplierAlarm;


    std::cout << "[TrainingMaxAnomalyThresholdSetter] Constructing {" << identifyer << "}"  << std::endl;
}

// Destroying
TrainingMaxAnomalyThresholdSetter::~TrainingMaxAnomalyThresholdSetter()
{
    std::cout << "[TrainingMaxAnomalyThresholdSetter] Destroying {" << identifyer << "}"  << std::endl;
}

void TrainingMaxAnomalyThresholdSetter::init()
{
    std::cout << "[TrainingMaxAnomalyThresholdSetter] Initializing {" << identifyer << "}"  << std::endl;

    thresholdTrainingWindow = sharedMemory->anomalyDetector->distance->size;

    warningThreshold = -1;
    alarmThreshold = -1;

    thresholdSet = false;

    std::cout << "[TrainingMaxAnomalyThresholdSetter] Done initializing {" << identifyer << "}"  << std::endl;
}

double TrainingMaxAnomalyThresholdSetter::getWarningThreshold()
{
    
    if((!thresholdSet) &&
        (!sharedMemory->profiler->training) &&
        (sharedMemory->profiler->profiledCount + sharedMemory->anomalyDetector->inWindowSize) >= thresholdTrainingWindow &&
        sharedMemory->anomalyDetector->distance->index +1 >= thresholdTrainingWindow){
        for(int i=0; i < thresholdTrainingWindow; i++){
            if(warningThreshold < sharedMemory->anomalyDetector->distance->data[sharedMemory->anomalyDetector->distance->size - thresholdTrainingWindow + i]){
                warningThreshold = sharedMemory->anomalyDetector->distance->data[sharedMemory->anomalyDetector->distance->size - thresholdTrainingWindow + i];
            }
        }

        alarmThreshold = warningThreshold*maxMultiplierAlarm;
        warningThreshold = warningThreshold*maxMultiplierWarning;
        
        thresholdSet = true;
        sharedMemory->anomalyDetector->training = false;

    }
    

    return warningThreshold;
}

double TrainingMaxAnomalyThresholdSetter::getAlarmThreshold()
{
    return alarmThreshold;
}