/**
 * TO-DO
 * - Extend History to multivariate
 * -- add support for hostory buffer
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
    // p.reset();
}

void MainLoop::setHistoryBuffer(HistoryBuffer *historyBuffer)
{
    this->historyBuffer = historyBuffer;
}

void MainLoop::tick()
{
    std::cout << "[MainLoop] tick" << std::endl;

    // Getting short term history data
    const double *history = historyBuffer->copyHistoryArray(); // copy history buffer
    double finalValue;
    int finalValueIndex;
    int dataTickCount = historyBuffer->getRealIndex();
    int historySize = historyBuffer->getSize();
    if(dataTickCount < historySize){
        finalValueIndex = dataTickCount-1;
        finalValue = history[dataTickCount-1];
    } else {
        finalValueIndex = historySize;
        finalValue = history[historySize];
    }


    // Running main loop
    if(dataTickCount == 1){// initialize all components
        // profiler
        profiler->init(historySize);

    } else {// ususal tick
        double *profile = profiler->profile(history, finalValueIndex);
    
    }
}