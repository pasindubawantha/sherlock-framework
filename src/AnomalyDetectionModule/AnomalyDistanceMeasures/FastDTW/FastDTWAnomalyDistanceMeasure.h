#ifndef _FASTDTWANOMALYDISTANCEMEASURE_H_
#define _FASTDTWANOMALYDISTANCEMEASURE_H_
// Abstract base class interface

#include <string>
#include "AnomalyDistanceMeasure.h"
#include "DTW.hpp"
#include "Eigen/Dense"

class FastDTWAnomalyDistanceMeasure: public AnomalyDistanceMeasure
{
private:
    
protected:
    DTW *dtw;
    
public:
    // Constructors
    FastDTWAnomalyDistanceMeasure(std::string identifyer);

    // Destructor
    virtual ~FastDTWAnomalyDistanceMeasure() override;
    
    // methods
    virtual void init() override;

    virtual double measureDistance() override;

};

#endif // _FASTDTWANOMALYDISTANCEMEASURE_H_
