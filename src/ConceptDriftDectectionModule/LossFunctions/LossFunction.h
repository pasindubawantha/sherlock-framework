#ifndef _LOSSFUNCTION_H_
#define _LOSSFUNCTION_H_
// Abstract base class interface

#include <string>

class LossFunction
{
private:
    
protected:
    
public:
    // Constructors
    LossFunction();

    // Destructor
    virtual ~LossFunction();
    
    // Copy constructor
    LossFunction(const LossFunction &source);
    
    // Move constructor
    LossFunction(LossFunction &&source);
    
    // methods

};

#endif // _LOSSFUNCTION_H_
