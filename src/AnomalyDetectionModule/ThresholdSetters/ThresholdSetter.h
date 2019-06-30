#ifndef _THRESHOLDSETTER_H_
#define _THRESHOLDSETTER_H_
// Abstract base class interface

#include <string>

class ThresholdSetter
{
private:
    
protected:
    
public:
    // Constructors
    ThresholdSetter();

    // Destructor
    virtual ~ThresholdSetter();
    
    // Copy constructor
    ThresholdSetter(const ThresholdSetter &source);
    
    // Move constructor
    ThresholdSetter(ThresholdSetter &&source);
    
    // methods

};

#endif // _THRESHOLDSETTER_H_
