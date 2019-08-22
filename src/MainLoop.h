#ifndef _MAINLOOP_H_
#define _MAINLOOP_H_

#include <string>
#include <iostream>
#include <memory>

#include "HistoryBuffer.h"
#include "SharedMemory.h"

#include "Profiler.h"

#include "AnomalyDistanceMeasure.h"
#include "AnomalyThresholdSetter.h"
#include "AnomalyDetector.h"

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

    // Anomaly Detetion
    std::unique_ptr<AnomalyDistanceMeasure> anomalyDistanceMeasure; // pointer (anomalyDistanceMeasure to be dynamically binded)
    std::unique_ptr<AnomalyThresholdSetter> anomalyThresholdSetter; // pointer (anomalyThresholdSetter to be dynamically binded)
    std::unique_ptr<AnomalyDetector> anomalyDetector; // pointer (anomalyDetector to be dynamically binded)
    

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
    void setAnomalyDistanceMeasure(AnomalyDistanceMeasure *anomalyDistanceMeasure);
    void setAnomalyThresholdSetter(AnomalyThresholdSetter *anomalyThresholdSetter);
    void setAnomalyDetector(AnomalyDetector *anomalyDetector);

    void setHistoryBuffer(HistoryBuffer *historyBuffer);
    void setSharedMemory(SharedMemory *sharedMemory);

    void tick();
};

#endif // _MAINLOOP_H_
