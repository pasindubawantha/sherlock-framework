#ifndef _ANOMALYDETECTOR_H_
#define _ANOMALYDETECTOR_H_
// Abstract base class interface

#include <string>
#include "SharedMemory.h"

class AnomalyDetector
{
private:
    
protected:
    std::string identifyer;
    SharedMemory *sharedMemory;

public:
    // Constructors
    // AnomalyDetector();

    // Destructor
    virtual ~AnomalyDetector() {
        sharedMemory = NULL;
    };
    
    // Copy constructor
    // AnomalyDetector(const AnomalyDetector &source);
    
    // Move constructor
    // AnomalyDetector(AnomalyDetector &&source);
    
    // methods
    void setSharedMemory(SharedMemory *sharedMemory){
        this->sharedMemory = sharedMemory;
    }

    std::string getIdentifier(){
        return this->identifyer;
    }

    virtual void init() = 0;

    virtual bool detectWarnning() = 0;

    virtual bool detectAlarm() = 0;

};

#endif // _ANOMALYDETECTOR_H_
