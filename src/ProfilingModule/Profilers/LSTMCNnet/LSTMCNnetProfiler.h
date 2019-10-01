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
    LSTMCNNFCPredictionModel *predictionModel;
    ModelStruct *modelStruct;
    double trainingRatio = 1;
    double lstmW;
    double cnnW;
    bool verbose = false;

public:
    // Constructors
    LSTMCNnetProfiler(std::string identifyer);

    LSTMCNnetProfiler(std::string identifyer, ModelStruct *model, double lstmWeight, double cnnWeight);

    virtual ~LSTMCNnetProfiler() override;
    
    virtual void init();

    virtual double profile() override;

};

#endif // _LSTMCNNETPROFILER_H_
