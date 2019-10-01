#ifndef _CONCEPTTHRESHOLDSETTER_H_
#define _CONCEPTTHRESHOLDSETTER_H_

// Abstract base class interface

#include <string>
#include "SharedMemory.h"


class ConceptThresholdSetter
{
private:
    
protected:
    std::string identifyer;
    SharedMemory *sharedMemory;
    
public:
    // Constructors
    // ConceptThresholdSetter();

    // Destructor
    virtual ~ConceptThresholdSetter() {
        sharedMemory = NULL;
    };
    
    // Copy constructor
    // ConceptThresholdSetter(const ConceptThresholdSetter &source);
    
    // Move constructor
    // ConceptThresholdSetter(ConceptThresholdSetter &&source);
    
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

#endif // _CONCEPTTHRESHOLDSETTER_H_