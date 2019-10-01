#ifndef _DynamicWindowConceptDriftDetector_H_
#define _DynamicWindowConceptDriftDetector_H_
// Abstract base class interface

#include <string>
#include "ConceptDriftDetector.h"

class DynamicWindowConceptDriftDetector: public ConceptDriftDetector
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
    DynamicWindowConceptDriftDetector(std::string identifyer);

    // Destructor
    virtual ~DynamicWindowConceptDriftDetector() override;
    
    // methods
    virtual void init() override;

    virtual int detect() override;

};

#endif // _DynamicWindowConceptDriftDetector_H_
