#ifndef _DefaultAnomalyDetector_H_
#define _DefaultAnomalyDetector_H_
// Abstract base class interface

#include <string>
#include "AnomalyDetector.h"

class DefaultAnomalyDetector: public AnomalyDetector
{
private:
    
protected:
    
public:
    // Constructors
    DefaultAnomalyDetector(std::string identifyer);

    // Destructor
    virtual ~DefaultAnomalyDetector() override;
    
    // methods
    virtual void init() override;

    virtual bool detectWarnning() override;

    virtual bool detectAlarm() override;

};

#endif // _DefaultAnomalyDetector_H_
