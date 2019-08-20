#ifndef _SHAREDMEMORY_H_
#define _SHAREDMEMORY_H_

template<typename T> struct Array {
    T *data;
    int size;
    int index;

    Array(int s){
        size = s;
        index = -1;
        data = new T[s];
    }
};

struct ProfilerMemory {
    Array<double> *profile;
    int inWindowSize;
    int OutWindowSize;
    bool training;
    ProfilerMemory(int size){
        profile = new Array<double>(size);
    }
};

struct AnomalyDetectorMemory {
    Array<double> *distance;
    int inWindowSize;
    double thresholdWarning;
    double thresholdAlram;
    Array<bool> *warning;
    Array<bool> *alarm;
    bool training;
    AnomalyDetectorMemory(int sizeDistance, int sizeWarning, int sizeAlarm){
        distance = new Array<double>(sizeDistance);
        warning = new Array<bool>(sizeWarning);
        alarm = new Array<bool>(sizeAlarm);
    }
};

struct ConceptDriftDetectorMemory {
    Array<double> *distance;
    int inWindowSize;
    double thresholdWarning;
    double thresholdAlram;
    Array<bool> *warning;
    Array<bool> *alarm;
    bool training;
    ConceptDriftDetectorMemory(int sizeDistance, int sizeWarning, int sizeAlarm){
        distance = new Array<double>(sizeDistance);
        warning = new Array<bool>(sizeWarning);
        alarm = new Array<bool>(sizeAlarm);
    }
};

struct SharedMemory {
    Array<double> *history;
    ProfilerMemory *profiler;
    AnomalyDetectorMemory *anomalyDetector;
    ConceptDriftDetectorMemory *conceptDriftDetector;
    SharedMemory(ProfilerMemory *pmem, AnomalyDetectorMemory *amem, ConceptDriftDetectorMemory *cmem){
        profiler = pmem;
        anomalyDetector = amem;
        conceptDriftDetector = cmem;
    };
};

#endif // _SHAREDMEMORY_H_