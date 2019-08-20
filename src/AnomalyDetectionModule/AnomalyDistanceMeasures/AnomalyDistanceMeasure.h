#ifndef _ANOMALYDISTANCEMEASURE_H_
#define _ANOMALYDISTANCEMEASURE_H_
// Abstract base class interface

#include <string>

class AnomalyDistanceMeasure
{
private:
    
protected:
    
public:
    // Constructors
    AnomalyDistanceMeasure();

    // Destructor
    virtual ~AnomalyDistanceMeasure();
    
    // Copy constructor
    AnomalyDistanceMeasure(const AnomalyDistanceMeasure &source);
    
    // Move constructor
    AnomalyDistanceMeasure(AnomalyDistanceMeasure &&source);
    
    // methods

};

#endif // _ANOMALYDISTANCEMEASURE_H_
