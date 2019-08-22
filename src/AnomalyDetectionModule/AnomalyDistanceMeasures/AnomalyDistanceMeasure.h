#ifndef _ANOMALYDISTANCEMEASURE_H_
#define _ANOMALYDISTANCEMEASURE_H_
// Abstract base class interface

#include <string>
#include "SharedMemory.h"

class AnomalyDistanceMeasure
{
private:
    
protected:
    std::string identifyer;
    SharedMemory *sharedMemory;
    
public:
    // // Constructors
    // AnomalyDistanceMeasure();

    // Destructor
    virtual ~AnomalyDistanceMeasure() {};
    
    // // Copy constructor
    // AnomalyDistanceMeasure(const AnomalyDistanceMeasure &source);
    
    // // Move constructor
    // AnomalyDistanceMeasure(AnomalyDistanceMeasure &&source);
    
    // methods
    void setSharedMemory(SharedMemory *sharedMemory){
        this->sharedMemory = sharedMemory;
    }

    std::string getIdentifier(){
        return this->identifyer;
    }

    virtual void init() = 0;

    virtual double measureDistance() = 0;

};

#endif // _ANOMALYDISTANCEMEASURE_H_
