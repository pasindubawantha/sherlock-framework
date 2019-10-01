#ifndef _ConfidenceIntervalAnomalyThresholdSetter_H_
#define _ConfidenceIntervalAnomalyThresholdSetter_H_
// Abstract base class interface

#include <string>
#include "AnomalyThresholdSetter.h"

class ConfidenceIntervalAnomalyThresholdSetter: public AnomalyThresholdSetter
{
private:
    
protected:
    int windowSize = 30;
    double warningAlpha = 2;
    double alarmAlpha = 3;
    double warningThreshold;
    double alarmThreshold;
    double minMean;
    double minSD;
    double firstMeanAndSD = true;

    double calculateMean();

    double calculateSD(double mean);

public:
    // Constructors
    ConfidenceIntervalAnomalyThresholdSetter(std::string identifyer);

    ConfidenceIntervalAnomalyThresholdSetter(std::string identifyer, double warningAlpha, double alarmAlpha);

    // Destructor
    virtual ~ConfidenceIntervalAnomalyThresholdSetter() override;
    
    // methods
    virtual void init() override;

    virtual double getWarningThreshold() override;
    
    virtual double getAlarmThreshold() override;

    // void setFirstMeanAndSD(bool firstMeanAndSD);

};

#endif // _ConfidenceIntervalAnomalyThresholdSetter_H_
