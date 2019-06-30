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
    
    virtual void init(int historySize);

    virtual double* profile(const double *history, int history_current_index) override;

    virtual std::string getIdentifier() override;
};

#endif // _DEAFULTPROFILER_H_
