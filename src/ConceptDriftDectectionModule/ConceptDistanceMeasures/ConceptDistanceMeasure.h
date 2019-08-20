#ifndef _CONCEPTDISTANCEMEASURE_H_
#define _CONCEPTDISTANCEMEASURE_H_
// Abstract base class interface

#include <string>

class ConceptDistanceMeasure
{
private:
    
protected:
    
public:
    // Constructors
    ConceptDistanceMeasure();

    // Destructor
    virtual ~ConceptDistanceMeasure();
    
    // Copy constructor
    ConceptDistanceMeasure(const ConceptDistanceMeasure &source);
    
    // Move constructor
    ConceptDistanceMeasure(ConceptDistanceMeasure &&source);
    
    // methods

};

#endif // _CONCEPTDISTANCEMEASURE_H_
