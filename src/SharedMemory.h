#ifndef _SHAREDMEMORY_H_
#define _SHAREDMEMORY_H_

#include <string>
#include <iostream>

template<typename T> class Queue {
    public:
    
    T *data;
    int size;
    int index;

    Queue(int size){
        this->size = size;
        index = -1;
        data = new T[size];
    }

    void enQueue(T value){
        if(index < size - 1){
            index++;
        } else {
            for(int i=0; i<size-1; i++){
                data[i] = data[i+1];
            }
        }
        data[index] = value;
    }

    void printQueue(int lookBack){
        if(index +1 - lookBack < 0){
            lookBack = index + 1;
        }
         std::cout << "{" ;
        for(int i=0; i<lookBack; i++){
            std::cout << data[index + 1 - lookBack + i];
            if(i != lookBack-1){
                std::cout << ", ";
            }
        }
        std::cout << "}" << std::endl;
    }
    
};

struct ProfilerMemory {
    Queue<double> *profile;
    int inWindowSize;
    int OutWindowSize;
    bool training;
    int profiledCount;
    int minTrainingWindowSize;
    int trainingDataCollected;
    ProfilerMemory(int size){
        profile = new Queue<double>(size);
        profiledCount = 0;
        trainingDataCollected = 1;
        training = true;
    }
};

struct AnomalyDetectorMemory {
    Queue<double> *distance;
    int inWindowSize;
    double thresholdWarning;
    double thresholdAlram;
    Queue<bool> *warning;
    Queue<bool> *alarm;
    bool training;
    AnomalyDetectorMemory(int sizeDistance, int sizeWarning, int sizeAlarm){
        distance = new Queue<double>(sizeDistance);
        warning = new Queue<bool>(sizeWarning);
        alarm = new Queue<bool>(sizeAlarm);
        training = true;
        thresholdWarning = -1;
        thresholdAlram = -1;
    }
};

struct ConceptDriftDetectorMemory {
    Queue<double> *distance;
    int inWindowSize;
    double thresholdWarning;
    double thresholdAlram;
    Queue<bool> *warning;
    Queue<bool> *alarm;
    bool training;
    ConceptDriftDetectorMemory(int sizeDistance, int sizeWarning, int sizeAlarm){
        distance = new Queue<double>(sizeDistance);
        warning = new Queue<bool>(sizeWarning);
        alarm = new Queue<bool>(sizeAlarm);
        training = true;
        thresholdWarning = -1;
        thresholdAlram = -1;
    }
};

struct SharedMemory {
    Queue<double> *history;
    ProfilerMemory *profiler;
    AnomalyDetectorMemory *anomalyDetector;
    ConceptDriftDetectorMemory *conceptDriftDetector;
    SharedMemory(ProfilerMemory *pmem, AnomalyDetectorMemory *amem, ConceptDriftDetectorMemory *cmem){
        profiler = pmem;
        anomalyDetector = amem;
        conceptDriftDetector = cmem;
    };
    ~SharedMemory(){
        std::cout << "[SharedMemory] Destroying"  << std::endl;
    }
};

#endif // _SHAREDMEMORY_H_