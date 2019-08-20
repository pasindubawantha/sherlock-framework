#ifndef _LSTMCNNETPROFILER_H_
#define _LSTMCNNETPROFILER_H_
// Abstract base class interface

#include <string>
#include "Profiler.h"
#include "LSTMCNnet.hpp"

class LSTMCNnetProfiler: public Profiler
{
private:
    
protected:
    LSTMCNNFCPredictionModel predictionModel;
    ModelStruct modelStruct;
    double trainingRatio = 1;

public:
    // Constructors
    LSTMCNnetProfiler(std::string identifyer);

    virtual ~LSTMCNnetProfiler() override;
    
    virtual void init();

    virtual double profile() override;

    virtual std::string getIdentifier() override;
};

#endif // _LSTMCNNETPROFILER_H_
