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

MainLoop::MainLoop(bool verbose)
{
    std::cout << "[MainLoop] Constructing"  << std::endl;
    this->verbose = verbose;
}

// Destructor
MainLoop::~MainLoop()
{
    std::cout << "[MainLoop] Destroying"  << std::endl;
    delete this->profiler;
    delete this->anomalyDetector;
    delete this->anomalyDistanceMeasure;
    delete this->anomalyThresholdSetter;
    delete this->conceptDestanceMeasure;
    delete this->conceptThresholdSetter;
    delete this->conceptDriftDetector;
    delete this->historyBuffer;
    delete this->sharedMemory;

    profiler = NULL;
    anomalyDetector = NULL;
    anomalyDistanceMeasure = NULL;
    anomalyThresholdSetter = NULL;
    conceptDestanceMeasure = NULL;
    conceptThresholdSetter = NULL;
    conceptDriftDetector = NULL;
    historyBuffer = NULL;
    sharedMemory = NULL;

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
    // this->profiler.reset(profiler);
    // delete this->profiler;
    this->profiler = profiler;
}

void MainLoop::setAnomalyDistanceMeasure(AnomalyDistanceMeasure *anomalyDistanceMeasure)
{
    std::cout << "[MainLoop] Setting AnomalyDistanceMeasure {" << anomalyDistanceMeasure->getIdentifier() << "}" << std::endl;
    // this->anomalyDistanceMeasure.reset(anomalyDistanceMeasure);\delete this->anomalyDistanceMeasure;
    // delete this->anomalyDistanceMeasure;
    this->anomalyDistanceMeasure = anomalyDistanceMeasure;
    
}

void MainLoop::setAnomalyThresholdSetter(AnomalyThresholdSetter *anomalyThresholdSetter)
{
    std::cout << "[MainLoop] Setting AnomalyThresholdSetter {" << anomalyThresholdSetter->getIdentifier() << "}" << std::endl;
    // this->anomalyThresholdSetter.reset(anomalyThresholdSetter);
    // delete this->anomalyThresholdSetter;
    this->anomalyThresholdSetter = anomalyThresholdSetter;
    
}

void MainLoop::setAnomalyDetector(AnomalyDetector *anomalyDetector)
{
    std::cout << "[MainLoop] Setting AnomalyDetector {" << anomalyDetector->getIdentifier() << "}" << std::endl;
    // this->anomalyDetector.reset(anomalyDetector);
    // delete this->anomalyDetector;
    this->anomalyDetector = anomalyDetector;
}

void MainLoop::setConceptDistanceMeasure(ConceptDistanceMeasure *conceptDestanceMeasure)
{
    std::cout << "[MainLoop] Setting ConceptDistanceMeasure {" << conceptDestanceMeasure->getIdentifier() << "}" << std::endl;
    this->conceptDestanceMeasure = conceptDestanceMeasure;
    
}

void MainLoop::setConceptThresholdSetter(ConceptThresholdSetter *conceptThresholdSetter)
{
    std::cout << "[MainLoop] Setting ConceptThresholdSetter {" << conceptThresholdSetter->getIdentifier() << "}" << std::endl;
    this->conceptThresholdSetter = conceptThresholdSetter;
    
}

void MainLoop::setConceptDriftDetector(ConceptDriftDetector *conceptDriftDetector)
{
    std::cout << "[MainLoop] Setting ConceptDriftDetector {" << conceptDriftDetector->getIdentifier() << "}" << std::endl;
    this->conceptDriftDetector = conceptDriftDetector;
    
}

void MainLoop::setHistoryBuffer(HistoryBuffer *historyBuffer)
{
    // delete this->historyBuffer;
    this->historyBuffer = historyBuffer;
}

void MainLoop::setSharedMemory(SharedMemory *sharedMemory)
{
    this->sharedMemory = sharedMemory;
    
    this->profiler->setSharedMemory(sharedMemory);
    
    this->anomalyDistanceMeasure->setSharedMemory(sharedMemory);
    this->anomalyThresholdSetter->setSharedMemory(sharedMemory);
    this->anomalyDetector->setSharedMemory(sharedMemory);

    this->conceptDestanceMeasure->setSharedMemory(sharedMemory);
    this->conceptThresholdSetter->setSharedMemory(sharedMemory);
    this->conceptDriftDetector->setSharedMemory(sharedMemory);;
    
}

void MainLoop::tick()
{
    int dataTickCount = historyBuffer->getRealIndex();
    if(verbose)
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
        conceptDestanceMeasure->init();

        // concept drift threshold
        conceptThresholdSetter->init();

        // conceot drit detection 
        conceptDriftDetector->init();

    } else {// after initilizing runnning tick
        
        if(verbose)
        std::cout << "actual :" << sharedMemory->history->data[sharedMemory->history->index] << std::endl;
        
        // # Profiling
        double profile = profiler->profile();
        sharedMemory->profiler->profile->enQueue(profile);
        
        if(verbose)
        std::cout << "profile "<< profile << std::endl;

        // # Detection
        if(sharedMemory->profiler->profiledCount > sharedMemory->anomalyDetector->inWindowSize){
            
            
            // ## Anomaly distance
            double anomalyDistance = anomalyDistanceMeasure->measureDistance();
            sharedMemory->anomalyDetector->distance->enQueue(anomalyDistance);
            if(verbose)
            std::cout << "distance "<< anomalyDistance << std::endl;

            // ## Concept distance
            double conceptDestance = conceptDestanceMeasure->measureDistance();
            sharedMemory->conceptDriftDetector->distance->enQueue(conceptDestance);
            if(verbose)
            std::cout << "concept distance "<< conceptDestance << std::endl;

            // ## Anomaly thresholding
            double anomalyWarningTheshold = anomalyThresholdSetter->getWarningThreshold();
            sharedMemory->anomalyDetector->thresholdWarning = anomalyWarningTheshold;
            
            double anomalyAlarmTheshold = anomalyThresholdSetter->getAlarmThreshold();
            sharedMemory->anomalyDetector->thresholdAlram = anomalyAlarmTheshold;

            if(verbose)
            std::cout << "thesh warning " << anomalyWarningTheshold << std::endl;
            if(verbose)
            std::cout << "thesh alarm " << anomalyAlarmTheshold << std::endl;

            // ## Concept thresholding
            double conceptWarningThreshold = conceptThresholdSetter->getWarningThreshold();
            sharedMemory->conceptDriftDetector->thresholdWarning = conceptWarningThreshold;
            
            double conceptAlarmTheshold = conceptThresholdSetter->getAlarmThreshold();
            sharedMemory->conceptDriftDetector->thresholdAlram = conceptAlarmTheshold;

            if(verbose)
            std::cout << "thesh concept warning " << conceptWarningThreshold << std::endl;
            if(verbose)
            std::cout << "thesh concept alarm " << conceptAlarmTheshold << std::endl;

            // if(!sharedMemory->anomalyDetector->training && !sharedMemory->profiler->training){
            if(!sharedMemory->anomalyDetector->training){
                // ## Anomaly detection
                bool  anomalyWarningLabel =  anomalyDetector->detectWarnning();
                sharedMemory->anomalyDetector->warning->enQueue(anomalyWarningLabel);

                bool  anomalyLabel = anomalyDetector->detectAlarm();
                sharedMemory->anomalyDetector->alarm->enQueue(anomalyLabel);

                if(verbose)
                std::cout << "warn " << anomalyWarningLabel << std::endl;
                if(verbose)
                std::cout << "alarm " << anomalyLabel << std::endl;
            }

            if(!sharedMemory->conceptDriftDetector->training  && !sharedMemory->profiler->training){
                // ## Concept detection
                // TODO
                // if concept drift return number of points in new concept  
                int newConceptPoints = conceptDriftDetector->detect();
                if(newConceptPoints < 0){
                    if(verbose)
                    std::cout << "no concept drift " << std::endl;
                } else {
                    std::cout << "concept drift !" << std::endl;
                    sharedMemory->conceptDriftDetector->training = true;
                    sharedMemory->profiler->training = true;
                    sharedMemory->profiler->trainingDataCollected = newConceptPoints;
                }
            }
        }
    }
    // Debugging prints
    // std::cout << "history ";
    // sharedMemory->history->printQueue(6);
    // std::cout << "profile ";
    // sharedMemory->profiler->profile->printQueue(6);

    // historyBuffer->print();
    if(verbose)
    std::cout << "------" << std::endl;
    delete this->sharedMemory->history->data;
}