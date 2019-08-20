#ifndef _PROFILER_H_
#define _PROFILER_H_
// Abstract base class interface

#include <string>
#include "SharedMemory.h"

class Profiler
{
private:
    
protected:
    std::string identifyer;
    SharedMemory *sharedMemory;
    
public:
    // // Constructors
    // virtual Profiler();

    // // Destructor
    virtual ~Profiler() {};
    
    // // Copy constructor
    // Profiler(const Profiler &source);
    
    // // Move constructor
    // Profiler(Profiler &&source);
    
    // methods
    void setSharedMemory(SharedMemory *sharedMemory){
        this->sharedMemory = sharedMemory;
    }

    virtual void init() = 0;

    virtual double profile() = 0;

    virtual std::string getIdentifier() = 0;
};

#endif // _PROFILER_H_
