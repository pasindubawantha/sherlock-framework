#ifndef _ANOMALYTHRESHOLDSETTER_H_
#define _ANOMALYTHRESHOLDSETTER_H_
// Abstract base class interface

#include <string>

class AnomalyThresholdSetter
{
private:
    
protected:
    
public:
    // Constructors
    AnomalyThresholdSetter();

    // Destructor
    virtual ~AnomalyThresholdSetter();
    
    // Copy constructor
    AnomalyThresholdSetter(const AnomalyThresholdSetter &source);
    
    // Move constructor
    AnomalyThresholdSetter(AnomalyThresholdSetter &&source);
    
    // methods

};

#endif // _ANOMALYTHRESHOLDSETTER_H_
