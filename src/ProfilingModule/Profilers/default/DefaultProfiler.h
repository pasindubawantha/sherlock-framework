#ifndef _DEAFULTPROFILER_H_
#define _DEAFULTPROFILER_H_
// Abstract base class interface

#include <string>
#include "Profiler.h"

class DefaultProfiler: public Profiler
{
private:
    
protected:
    int historySize;

public:
    // Constructors
    DefaultProfiler(std::string identifyer);

    // Destructor
    virtual ~DefaultProfiler() override;
    
    virtual void init() override;

    virtual double profile() override;

};

#endif // _DEAFULTPROFILER_H_
