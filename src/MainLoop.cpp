/**
 * TO-DO
 * - Extend History to multivariate
 * -- add support for history buffer
 * -- add profileMultivariate to Profiler interface
 * -- option to switch main loop for multivariate(if history has multivariate data otherwies no)
 * 
 * - Anomaly detecion module
 * 
 * - Concept drift detection module
 * */
#include "MainLoop.h"

#include <iostream>


// Constructors 
MainLoop::MainLoop()
{
    std::cout << "[MainLoop] Constructing"  << std::endl;
}

// Destructor
MainLoop::~MainLoop()
{
    std::cout << "[MainLoop] Destroying"  << std::endl;
}

// Copy constructor
MainLoop::MainLoop(const MainLoop &source)
{
    std::cout << "[MainLoop] Copying"  << std::endl;
}

// Move constructor
MainLoop::MainLoop(MainLoop &&source)// source is taken as an R value
{
    std::cout << "[MainLoop] Moving"  << std::endl;
}

void MainLoop::setProfiler(Profiler *profiler)
{
    std::cout << "[MainLoop] Setting Profiler {" << profiler->getIdentifier() << "}" << std::endl;
    this->profiler.reset(profiler);
}

void MainLoop::setAnomalyDistanceMeasure(AnomalyDistanceMeasure *anomalyDistanceMeasure)
{
    std::cout << "[MainLoop] Setting AnomalyDistanceMeasure {" << anomalyDistanceMeasure->getIdentifier() << "}" << std::endl;
    this->anomalyDistanceMeasure.reset(anomalyDistanceMeasure);
}

void MainLoop::setAnomalyThresholdSetter(AnomalyThresholdSetter *anomalyThresholdSetter)
{
    std::cout << "[MainLoop] Setting AnomalyThresholdSetter {" << anomalyThresholdSetter->getIdentifier() << "}" << std::endl;
    this->anomalyThresholdSetter.reset(anomalyThresholdSetter);
}

void MainLoop::setAnomalyDetector(AnomalyDetector *anomalyDetector)
{
    std::cout << "[MainLoop] Setting AnomalyDetector {" << anomalyDetector->getIdentifier() << "}" << std::endl;
    this->anomalyDetector.reset(anomalyDetector);
}


void MainLoop::setHistoryBuffer(HistoryBuffer *historyBuffer)
{
    this->historyBuffer = historyBuffer;
}

void MainLoop::setSharedMemory(SharedMemory *sharedMemory)
{
    this->sharedMemory = sharedMemory;
    
    this->profiler->setSharedMemory(sharedMemory);
    
    this->anomalyDistanceMeasure->setSharedMemory(sharedMemory);
    this->anomalyThresholdSetter->setSharedMemory(sharedMemory);
    this->anomalyDetector->setSharedMemory(sharedMemory);
    
}

void MainLoop::tick()
{
    int dataTickCount = historyBuffer->getRealIndex();
    std::cout << "[MainLoop] tick count :" << dataTickCount << std::endl;

    // Getting short term history data
    this->sharedMemory->history->data = historyBuffer->copyHistoryArray(); // copy history buffer;
    this->sharedMemory->history->size = historyBuffer->getSize();
    this->sharedMemory->history->index = historyBuffer->getCurrentIndexToRead();
    

    if(dataTickCount == 1){// initialize all components on first tick
        // profiler
        profiler->init();

        // anomaly distance
        anomalyDistanceMeasure->init();

        // anomaly threshold
        anomalyThresholdSetter->init();
        
        // anomaly detection
        anomalyDetector->init();

        // concept drift distance
        // TODO

        // concept drift threshold
        // TODO

        // conceot drit detection 
        // TODO

    } else {// after initilizing runnning tick
        
        std::cout << "actual :" << sharedMemory->history->data[sharedMemory->history->index] << std::endl;
        
        // # Profiling
        double profile = profiler->profile();
        sharedMemory->profiler->profile->enQueue(profile);
        
        std::cout << "profile "<< profile << std::endl;

        // # Detection
        if(sharedMemory->profiler->profiledCount > sharedMemory->anomalyDetector->inWindowSize){
            

            // ## Anomaly distance
            double anomalyDistance = anomalyDistanceMeasure->measureDistance();
            sharedMemory->anomalyDetector->distance->enQueue(anomalyDistance);
            std::cout << "distance "<< anomalyDistance << std::endl;

            // ## Concept distance
            // TODO

            // ## Anomaly thresholding
            double anomalyWarningTheshold = anomalyThresholdSetter->getWarningThreshold();
            sharedMemory->anomalyDetector->thresholdWarning = anomalyWarningTheshold;
            
            double anomalyAlarmTheshold = anomalyThresholdSetter->getAlarmThreshold();
            sharedMemory->anomalyDetector->thresholdAlram = anomalyAlarmTheshold;

            std::cout << "thesh warning " << anomalyWarningTheshold << std::endl;
            std::cout << "thesh alarm " << anomalyAlarmTheshold << std::endl;

            // ## Concept thresholding
            // TODO

            if(!sharedMemory->anomalyDetector->training){

                // ## Anomaly detection
                bool anomalyWarningLabel = anomalyDetector->detectWarnning();
                sharedMemory->anomalyDetector->warning->enQueue(anomalyWarningLabel);

                bool anomalyLabel = anomalyDetector->detectAlarm();
                sharedMemory->anomalyDetector->alarm->enQueue(anomalyLabel);

                std::cout << "warn " << anomalyWarningLabel << std::endl;
                std::cout << "alarm " << anomalyLabel << std::endl;

                // ## Concept detection
                // TODO

            }
        }
    }
    // Debugging prints
    // std::cout << "history ";
    // sharedMemory->history->printQueue(6);
    // std::cout << "profile ";
    // sharedMemory->profiler->profile->printQueue(6);

    // historyBuffer->print();
    std::cout << "------" << std::endl;
}