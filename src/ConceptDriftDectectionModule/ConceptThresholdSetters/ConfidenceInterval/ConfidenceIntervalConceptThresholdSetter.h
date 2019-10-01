#ifndef _ConfidenceIntervalConceptThresholdSetter_H_
#define _ConfidenceIntervalConceptThresholdSetter_H_
// Abstract base class interface

#include <string>
#include "ConceptThresholdSetter.h"

class ConfidenceIntervalConceptThresholdSetter: public ConceptThresholdSetter
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
    ConfidenceIntervalConceptThresholdSetter(std::string identifyer);

    ConfidenceIntervalConceptThresholdSetter(std::string identifyer, double warningAlpha, double alarmAlpha);

    // Destructor
    virtual ~ConfidenceIntervalConceptThresholdSetter() override;
    
    // methods
    virtual void init() override;

    virtual double getWarningThreshold() override;
    
    virtual double getAlarmThreshold() override;

    // void setFirstMeanAndSD(bool firstMeanAndSD);

};

#endif // _ConfidenceIntervalConceptThresholdSetter_H_
