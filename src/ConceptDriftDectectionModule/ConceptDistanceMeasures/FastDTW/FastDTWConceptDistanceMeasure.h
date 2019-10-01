#ifndef _FASTDTWConceptDistanceMeasure_H_
#define _FASTDTWConceptDistanceMeasure_H_
// Abstract base class interface

#include <string>
#include "ConceptDistanceMeasure.h"
#include "DTW.hpp"
#include "Eigen/Dense"

class FastDTWConceptDistanceMeasure: public ConceptDistanceMeasure
{
private:
    
protected:
    DTW *dtw;
    
public:
    // Constructors
    FastDTWConceptDistanceMeasure(std::string identifyer);

    // Destructor
    virtual ~FastDTWConceptDistanceMeasure() override;
    
    // methods
    virtual void init() override;

    virtual double measureDistance() override;

};

#endif // _FASTDTWConceptDistanceMeasure_H_
