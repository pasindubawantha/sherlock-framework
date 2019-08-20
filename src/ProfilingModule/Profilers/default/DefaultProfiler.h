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

    virtual ~DefaultProfiler() override;
    
    virtual void init();

    virtual double profile() override;

    virtual std::string getIdentifier() override;
};

#endif // _DEAFULTPROFILER_H_
