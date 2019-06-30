#ifndef _DISTANCEMEASURE_H_
#define _DISTANCEMEASURE_H_
// Abstract base class interface

#include <string>

class DistanceMeasure
{
private:
    
protected:
    
public:
    // Constructors
    DistanceMeasure();

    // Destructor
    virtual ~DistanceMeasure();
    
    // Copy constructor
    DistanceMeasure(const DistanceMeasure &source);
    
    // Move constructor
    DistanceMeasure(DistanceMeasure &&source);
    
    // methods

};

#endif // _DISTANCEMEASURE_H_
