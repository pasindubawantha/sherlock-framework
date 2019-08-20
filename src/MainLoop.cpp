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

void MainLoop::setHistoryBuffer(HistoryBuffer *historyBuffer)
{
    this->historyBuffer = historyBuffer;
}

void MainLoop::setSharedMemory(SharedMemory *sharedMemory)
{
    this->sharedMemory = sharedMemory;
    this->profiler->setSharedMemory(sharedMemory);
}

void MainLoop::tick()
{
    // std::cout << "[MainLoop] tick" << std::endl;

    // Getting short term history data
    double *history = historyBuffer->copyHistoryArray(); // copy history buffer
;
    int historyIndex = historyBuffer->getCurrentIndexToRead();
    int dataTickCount = historyBuffer->getRealIndex();
    int historySize = historyBuffer->getSize();

    this->sharedMemory->history->data = history;
    this->sharedMemory->history->size = historySize;
    this->sharedMemory->history->index = historyIndex;

    std::cout << "[MainLoop] tick count :" << dataTickCount << std::endl;
    // Running main loop
    if(dataTickCount == 1){// initialize all components
        // profiler (array)
        profiler->init();

        // anomaly distance (array)

        // anomaly threshold 

        // anomaly detection


        // concept drift distance

        // concept drift threshold

        // conceot drit detection 

    } else {// ususal tick
        std::cout << "[MainLoop] actual :" << sharedMemory->history->data[sharedMemory->history->index] << std::endl << "--------" << std::endl;
        // profiling
        double profile = profiler->profile();
        if(sharedMemory->profiler->profile->index < sharedMemory->profiler->profile->size - 1){
            sharedMemory->profiler->profile->index++;
        } else {
            for(int i=0; i<sharedMemory->profiler->profile->size-1;i++){
                sharedMemory->profiler->profile->data[i] = sharedMemory->profiler->profile->data[i+1];
            }
        }
        sharedMemory->profiler->profile->data[sharedMemory->profiler->profile->index] = profile;
        std::cout << "profile "<< profile << std::endl;

        
    }
    
    // historyBuffer->print();
}