#ifndef _CONCEPTDISTANCEMEASURE_H_
#define _CONCEPTDISTANCEMEASURE_H_
// Abstract base class interface

#include <string>
#include "SharedMemory.h"

class ConceptDistanceMeasure
{
private:
    
protected:
    std::string identifyer;
    SharedMemory *sharedMemory;
    
public:
    // // Constructors
    // ConceptDistanceMeasure();

    // Destructor
    virtual ~ConceptDistanceMeasure() {
        sharedMemory = NULL;
    };
    
    // // Copy constructor
    // ConceptDistanceMeasure(const ConceptDistanceMeasure &source);
    
    // // Move constructor
    // ConceptDistanceMeasure(ConceptDistanceMeasure &&source);
    
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

#endif // _CONCEPTDISTANCEMEASURE_H_