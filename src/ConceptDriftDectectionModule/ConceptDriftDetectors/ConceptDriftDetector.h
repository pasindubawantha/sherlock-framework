#ifndef _CONCEPTDRIFTDETECTOR_H_
#define _CONCEPTDRIFTDETECTOR_H_
// Abstract base class interface

#include <string>
#include "SharedMemory.h"

class ConceptDriftDetector
{
private:
    
protected:
    std::string identifyer;
    SharedMemory *sharedMemory;

public:
    // Constructors
    // ConceptDriftDetector();

    // Destructor
    virtual ~ConceptDriftDetector() {
        sharedMemory = NULL;
    };
    
    // Copy constructor
    // ConceptDriftDetector(const ConceptDriftDetector &source);
    
    // Move constructor
    // ConceptDriftDetector(ConceptDriftDetector &&source);
    
    // methods
    void setSharedMemory(SharedMemory *sharedMemory){
        this->sharedMemory = sharedMemory;
    }

    std::string getIdentifier(){
        return this->identifyer;
    }

    virtual void init() = 0;

    virtual int detect() = 0;

};

#endif // _CONCEPTDRIFTDETECTOR_H_