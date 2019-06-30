#ifndef _PROFILER_H_
#define _PROFILER_H_
// Abstract base class interface

#include <string>

class Profiler
{
private:
    
protected:
    std::string identifyer;
    
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
    virtual void init(int historySize) = 0;

    virtual double* profile(const double *history, int history_current_index) = 0;

    virtual std::string getIdentifier() = 0;
};

#endif // _PROFILER_H_
