#ifndef _ConfidenceIntervalAnomalyDetector_H_
#define _ConfidenceIntervalAnomalyDetector_H_
// Abstract base class interface

#include <string>
#include "AnomalyDetector.h"

class ConfidenceIntervalAnomalyDetector: public AnomalyDetector
{
private:
    
protected:
    int count = 0;
    int windowSize = 30;

    double calculateMean();

    double calculateSD(double mean);

    int getNewConceptCount();
    
public:
    // Constructors
    ConfidenceIntervalAnomalyDetector(std::string identifyer);

    // Destructor
    virtual ~ConfidenceIntervalAnomalyDetector() override;
    
    // methods
    virtual void init() override;

    virtual bool detectWarnning() override;

    virtual bool detectAlarm() override;

};

#endif // _ConfidenceIntervalAnomalyDetector_H_
