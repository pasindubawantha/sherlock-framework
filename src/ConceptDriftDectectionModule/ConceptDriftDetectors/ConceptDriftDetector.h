#ifndef _CONCEPTDRIFTDETECTOR_H_
#define _CONCEPTDRIFTDETECTOR_H_
// Abstract base class interface

#include <string>

class ConceptDriftDetector
{
private:
    
protected:
    
public:
    // Constructors
    ConceptDriftDetector();

    // Destructor
    virtual ~ConceptDriftDetector();
    
    // Copy constructor
    ConceptDriftDetector(const ConceptDriftDetector &source);
    
    // Move constructor
    ConceptDriftDetector(ConceptDriftDetector &&source);
    
    // methods

};

#endif // _CONCEPTDRIFTDETECTOR_H_
