#ifndef _MAINLOOP_H_
#define _MAINLOOP_H_

#include <string>
#include <iostream>
#include <memory>

#include "HistoryBuffer.h"
#include "Profiler.h"
#include "SharedMemory.h"
// #include "AnomalyDetectionModule/AnomalyDetector.h"
// #include "AnomalyDetectionModule/DistanceMeasure.h"
// #include "AnomalyDetectionModule/ThresholdSetter.h"
// #include "ConceptDriftDectectionModule/ConceptDriftDetector.h"
// #include "ConceptDriftDectectionModule/ConceptThresholdSetter.h"
// #include "ConceptDriftDectectionModule/LossFunction.h"

class MainLoop
{
private:
    
protected:

    // history buffer
    HistoryBuffer *historyBuffer;
    SharedMemory *sharedMemory;

    // Profiling
    std::unique_ptr<Profiler> profiler; // pointer (profiler to be dynamically binded)

    // // Anomaly Detetion
    // AnomalyDetector *anomalyDetector; // pointer (anomalyDetector to be dynamically binded)
    // ThresholdSetter *thresholdSetter; // pointer (thresholdSetter to be dynamically binded)
    // DistanceMeasure *distanceMeasure; // pointer (distanceMeasure to be dynamically binded)

    // //
    // ConceptDriftDetector *conceptDriftDetector; // pointer (conceptDriftDetector to be dynamically binded)
    // ConceptThresholdSetter *conceptThresholdSetter; // pointer (conceptThresholdSetter to be dynamically binded)
    // LossFunction *lossFunction; // pointer (lossFunction to be dynamically binded)
    
public:
    // Constructors
    MainLoop();

    // Destructor
    ~MainLoop();
    
    // Copy constructor
    MainLoop(const MainLoop &source);
    
    // Move constructor
    MainLoop(MainLoop &&source);
    
    // methods
    void print(){
        std::cout << 1  << std::endl;
    }

    void setProfiler(Profiler *profiler);

    void setHistoryBuffer(HistoryBuffer *historyBuffer);
    void setSharedMemory(SharedMemory *sharedMemory);

    void tick();
};

#endif // _MAINLOOP_H_
