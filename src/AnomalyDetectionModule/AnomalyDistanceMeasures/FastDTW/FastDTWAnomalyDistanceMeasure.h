#ifndef _FASTDTWANOMALYDISTANCEMEASURE_H_
#define _FASTDTWANOMALYDISTANCEMEASURE_H_
// Abstract base class interface

#include <string>
#include "AnomalyDistanceMeasure.h"

class FastDTWAnomalyDistanceMeasure: public AnomalyDistanceMeasure
{
private:
    
protected:
    
public:
    // Constructors
    FastDTWAnomalyDistanceMeasure();

    // Destructor
    virtual ~FastDTWAnomalyDistanceMeasure() override;
    
    // methods

};

#endif // _FASTDTWANOMALYDISTANCEMEASURE_H_
