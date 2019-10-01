#ifndef _DynamicWindowConceptDriftDetector_H_
#define _DynamicWindowConceptDriftDetector_H_
// Abstract base class interface

#include <string>
#include "ConceptDriftDetector.h"

class NoConceptDriftDetector: public ConceptDriftDetector
{
private:
    
protected:
    
public:
    // Constructors
    NoConceptDriftDetector(std::string identifyer);

    // Destructor
    virtual ~NoConceptDriftDetector() override;
    
    // methods
    virtual void init() override;

    virtual int detect() override;

};

#endif // _DynamicWindowConceptDriftDetector_H_
