#ifndef _TrainingMaxAnomalyThresholdSetter_H_
#define _TrainingMaxAnomalyThresholdSetter_H_
// Abstract base class interface

#include <string>
#include "AnomalyThresholdSetter.h"

class TrainingMaxAnomalyThresholdSetter: public AnomalyThresholdSetter
{
private:
    
protected:
    int thresholdTrainingWindow;
    double maxMultiplierWarning = 1;
    double maxMultiplierAlarm = 1.5;
    double warningThreshold;
    double alarmThreshold;
    bool thresholdSet;

    
public:
    // Constructors
    TrainingMaxAnomalyThresholdSetter(std::string identifyer);

    TrainingMaxAnomalyThresholdSetter(std::string identifyer, double maxMultiplierWarning, double maxMultiplierAlarm);

    // Destructor
    virtual ~TrainingMaxAnomalyThresholdSetter() override;
    
    // methods
    virtual void init() override;

    virtual double getWarningThreshold() override;
    
    virtual double getAlarmThreshold() override;

};

#endif // _TrainingMaxAnomalyThresholdSetter_H_
