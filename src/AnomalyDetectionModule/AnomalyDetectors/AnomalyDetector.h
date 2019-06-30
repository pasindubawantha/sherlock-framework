#ifndef _ANOMALYDETECTOR_H_
#define _ANOMALYDETECTOR_H_
// Abstract base class interface

#include <string>

class AnomalyDetector
{
private:
    
protected:
    
public:
    // Constructors
    AnomalyDetector();

    // Destructor
    virtual ~AnomalyDetector();
    
    // Copy constructor
    AnomalyDetector(const AnomalyDetector &source);
    
    // Move constructor
    AnomalyDetector(AnomalyDetector &&source);
    
    // methods

};

#endif // _ANOMALYDETECTOR_H_
