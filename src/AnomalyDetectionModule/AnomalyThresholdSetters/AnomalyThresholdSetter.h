#ifndef _ANOMALYTHRESHOLDSETTER_H_
#define _ANOMALYTHRESHOLDSETTER_H_
// Abstract base class interface

#include <string>
#include "SharedMemory.h"


class AnomalyThresholdSetter
{
private:
    
protected:
    std::string identifyer;
    SharedMemory *sharedMemory;
    
public:
    // Constructors
    // AnomalyThresholdSetter();

    // Destructor
    virtual ~AnomalyThresholdSetter() {};
    
    // Copy constructor
    // AnomalyThresholdSetter(const AnomalyThresholdSetter &source);
    
    // Move constructor
    // AnomalyThresholdSetter(AnomalyThresholdSetter &&source);
    
    // methods
    void setSharedMemory(SharedMemory *sharedMemory){
        this->sharedMemory = sharedMemory;
    }

    std::string getIdentifier(){
        return this->identifyer;
    }

    virtual void init() = 0;

    virtual double getWarningThreshold() = 0;
    
    virtual double getAlarmThreshold() = 0;

};

#endif // _ANOMALYTHRESHOLDSETTER_H_
