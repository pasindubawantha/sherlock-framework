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

#include "ConceptDistanceMeasure.h"
#include "ConceptThresholdSetter.h"
#include "ConceptDriftDetector.h"

class MainLoop
{
private:
    
protected:
    bool verbose = true;

    // history buffer
    HistoryBuffer *historyBuffer;
    SharedMemory *sharedMemory;

    // Profiling
    // std::unique_ptr<Profiler> profiler; // pointer (profiler to be dynamically binded)
    Profiler *profiler;

    // Anomaly Detetion
    // std::unique_ptr<AnomalyDistanceMeasure> anomalyDistanceMeasure; // pointer (anomalyDistanceMeasure to be dynamically binded)
    AnomalyDistanceMeasure *anomalyDistanceMeasure;
    // std::unique_ptr<AnomalyThresholdSetter> anomalyThresholdSetter; // pointer (anomalyThresholdSetter to be dynamically binded)
    AnomalyThresholdSetter *anomalyThresholdSetter;
    // std::unique_ptr<AnomalyDetector> anomalyDetector; // pointer (anomalyDetector to be dynamically binded)
    AnomalyDetector *anomalyDetector;
    

    ConceptDistanceMeasure *conceptDestanceMeasure;

    ConceptThresholdSetter *conceptThresholdSetter;

    ConceptDriftDetector *conceptDriftDetector;

    
public:
    // Constructors
    MainLoop();

    MainLoop(bool verbose);

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
    void setConceptDistanceMeasure(ConceptDistanceMeasure *conceptDestanceMeasure);
    void setConceptThresholdSetter(ConceptThresholdSetter *conceptThresholdSetter);
    void setConceptDriftDetector(ConceptDriftDetector *conceptDriftDetector);

    void setHistoryBuffer(HistoryBuffer *historyBuffer);
    void setSharedMemory(SharedMemory *sharedMemory);

    void tick();
};

#endif // _MAINLOOP_H_
