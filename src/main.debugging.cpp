/**
 * This demonstration is used to debug the framework in single thread mode
 * 
 * */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <mutex> 
#include <memory>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>

#include "MainLoop.h"
#include "HistoryBuffer.h"
#include "MutexLock.h"
#include "SharedMemory.h"

#include "Profiler.h"
#include "DefaultProfiler.h"
#include "LSTMCNnetProfiler.h"

#include "AnomalyDistanceMeasure.h"
#include "FastDTWAnomalyDistanceMeasure.h"

#include "AnomalyThresholdSetter.h"
#include "TrainingMaxAnomalyThresholdSetter.h"

#include "AnomalyDetector.h"
#include "DefaultAnomalyDetector.h"

int main()
{
    std::cout << "[main] Starting" <<std::endl;
    
    // History Buffer
    HistoryBuffer *historyBuffer = new HistoryBuffer();
    int historyBufferSize = 100; // set buffer size to 100 (Short memoty size)
    double *bufferHistory = new double[historyBufferSize]; 
    historyBuffer->setSize(historyBufferSize, bufferHistory); // set buffer size to 100 (Short memoty size)


    // # MainLoop
    MainLoop mainLoop;
    
    // set historyBuffer
    mainLoop.setHistoryBuffer(historyBuffer);

    // ## Anomaly Detection
    // setting anomaly distance measure
    AnomalyDistanceMeasure *anomalyDestanceMeasure = new FastDTWAnomalyDistanceMeasure("fastDTW 1");
    mainLoop.setAnomalyDistanceMeasure(anomalyDestanceMeasure);

    // setting anomaly threshold setter
    AnomalyThresholdSetter *anomalyThresholdSetter = new TrainingMaxAnomalyThresholdSetter("maxAnomalyThrehsolder 1");
    mainLoop.setAnomalyThresholdSetter(anomalyThresholdSetter);

    // setting anomaly threshold setter
    AnomalyDetector *anomalyDetector = new DefaultAnomalyDetector("defaultAnomalyDetector 1");
    mainLoop.setAnomalyDetector(anomalyDetector);

    // ## Concept Drift Detection
    // TODO

    // ## Profiling
    // setting profiler (do not delete pointers)
    Profiler *profiler = new LSTMCNnetProfiler("lstmcnnnet 1");
    mainLoop.setProfiler(profiler);


    // ## Shared Memory 

    // ### profiler
    int profilerSize = 6;
    ProfilerMemory profilerMemory(profilerSize);
    profilerMemory.inWindowSize = 30; // inWindowSize > mathight*matwidth + outWindowsize & inWindowSize < historyBufferSize
    profilerMemory.OutWindowSize = 1;

    // ### anomaly detector
    int anomalyDistanceSize = 1;
    int anomlayWarrningSize = 1; 
    int anomalyAlarmSize = 1;
    AnomalyDetectorMemory anomalyDetectorMemory(anomalyDistanceSize, anomlayWarrningSize, anomalyAlarmSize);
    anomalyDetectorMemory.inWindowSize=profilerSize-1; // inWindowSize < profilerSize

    // ### concept drift detector
    int conceptDistanceSize = 1;
    int conceptWarningSize = 1;
    int conceptAlarmSize = 1;
    ConceptDriftDetectorMemory conceptDriftDetectorMemory(conceptDistanceSize, conceptWarningSize, conceptAlarmSize);
    conceptDriftDetectorMemory.inWindowSize=profilerSize-1; // inWindowSize < profilerSize
    
    SharedMemory sharedMemory(&profilerMemory, &anomalyDetectorMemory, &conceptDriftDetectorMemory);
    sharedMemory.history = new Queue<double>(historyBufferSize);

    mainLoop.setSharedMemory(&sharedMemory);


    // Done Configuring

    std::fstream in_file {"../data/test/Twitter_volume_AAPL.200.txt", std::ios::in};
    
    if(in_file.is_open()) {
        std::string line;
        double line_double;
        while(!in_file.eof()){
            std::getline(in_file,line);
            if(line != "")
            {   
                line_double = std::stod(line);
                historyBuffer->writeSafe(line_double); // wtite to history buffer (last just before the lock)
                mainLoop.tick();

                historyBuffer->setTickedPrevious(true);
            } 
        }
        } else {
            std::cout << "[main] Failed to open file !" << std::endl;
        }
        in_file.close();
        std::cout << "[main] Ended" << std::endl;


    return 0;
}