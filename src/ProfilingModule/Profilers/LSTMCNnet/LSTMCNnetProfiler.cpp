#include "LSTMCNnetProfiler.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include "LSTMCNnet.hpp"

#include <iostream>

// Constructors 
LSTMCNnetProfiler::LSTMCNnetProfiler(std::string identifyer)
{   
    // other params
    trainingRatio = (double)0.5; // real training data = traingDataSize + maxHight*maxWidth
    // get model structure and training ratio as params and directly set 


    this->identifyer = identifyer;
    std::cout << "[LSTMCNnetProfiler] Constructing {" << identifyer << "}"  << std::endl;
    // build LSTMCNnet

    // Initializing the structure
    // modelStruct.trainDataSize = 10;
    modelStruct.learningRate = 0.000001;
    modelStruct.trainingIterations = 10; 
    // modelStruct.numPredPoints = 1;
    
    // LSTM parameters
    modelStruct.memCells = 15;
    
    // CNN parameters
    modelStruct.matWidth = 15;
    modelStruct.matHeight = 2;
    modelStruct.targetC = 1;
    
    // Convolutional layer
    struct::ConvLayStruct CL1;
    CL1.filterSize = 2; // filter size: N x N
    CL1.filters = 1; // No of filters
    CL1.stride = 1;

    // Pooling layer
    struct::PoolLayStruct PL1;
    PL1.poolH = 1; // pool size: N x N
    PL1.poolW = 1;

    // Fully connected layers
    struct::FCLayStruct FCL1;
    FCL1.outputs = 20; // neurons in fully connected layer
    struct::FCLayStruct FCL2;
    FCL2.outputs = 5; // neurons in fully connected layer
    struct::FCLayStruct FCL3;
    FCL3.outputs = 1; // neurons in fully connected layer

    char *layerOrder = new char[5]; 
    layerOrder[0] = 'C';
    layerOrder[1] = 'P';
    layerOrder[2] = 'F';
    layerOrder[3] = 'F';
    layerOrder[4] = 'F';
    struct::ConvLayStruct *CLs = new struct::ConvLayStruct[1];
    CLs[0] = CL1;
    struct::PoolLayStruct *PLs = new struct::PoolLayStruct[1];
    PLs[0] = PL1;
    struct::FCLayStruct *FCLs = new struct::FCLayStruct[3];
    FCLs[0] = FCL1;
    FCLs[1] = FCL2;
    FCLs[2] = FCL3;

    modelStruct.netStruct.layers = 5;
    modelStruct.netStruct.layerOrder = layerOrder;
    modelStruct.netStruct.CL = CLs;
    modelStruct.netStruct.PL = PLs;
    modelStruct.netStruct.FCL = FCLs;
    
    std::cout << "[LSTMCNnetProfiler] Done Constructing {" << identifyer << "}"  << std::endl;
    // Done constructing
}

// Destroying
LSTMCNnetProfiler::~LSTMCNnetProfiler()
{
    std::cout << "[LSTMCNnetProfiler] Destroying {" << identifyer << "}"  << std::endl;
}

void LSTMCNnetProfiler::init()
{
    std::cout << "[LSTMCNnetProfiler] Initializing {" << identifyer << "}"  << std::endl;
    // modelStruct.trainDataSize = (int)(trainingRatio * (double)sharedMemory->history->size);
    modelStruct.trainDataSize = 50;
    modelStruct.learningRate = 0.000001;
    modelStruct.trainingIterations = 10; 
    modelStruct.numPredPoints = sharedMemory->profiler->OutWindowSize;


    LSTMCNNFCPredictionModel pm(&modelStruct);
    this->predictionModel = pm;
    std::cout << "[LSTMCNnetProfiler] Done Initializing {" << identifyer << "}"  << std::endl;
}

double LSTMCNnetProfiler::profile() 
{
    double profile = 0;
    // profile[0] = ;

    if( (modelStruct.trainDataSize + (modelStruct.matHeight*modelStruct.matWidth) -1) > sharedMemory->history->index){
        std::cout << "[LSTMCNnetProfiler] Collecting data to train ! {" << identifyer << "}"  << std::endl;
    } else {
        if(sharedMemory->profiler->training){
            std::cout << "[LSTMCNnetProfiler] Done Collecting data to train. Training ! {" << identifyer << "}"  << std::endl;

            // Training the networks in the model 
            predictionModel.train(sharedMemory->history->data, sharedMemory->history->index);
            sharedMemory->profiler->training = false;
            
            std::cout << "[LSTMCNnetProfiler] Done Training ! {" << identifyer << "}"  << std::endl;
        } else {

            
            // parameters for model outputs
            int predictions = 1;
            double lstmW = 0.8;
            double cnnW = 0.1;
            int abs = 1;
            
            double *inWondow = new double[sharedMemory->profiler->inWindowSize];

            for(int i=sharedMemory->profiler->inWindowSize-1; i >= 0; i--){
                inWondow[sharedMemory->profiler->inWindowSize -1 - i] = sharedMemory->history->data[sharedMemory->history->index - i];
            };

            // getting predicted time series data points
            profile = predictionModel.predict(predictions, inWondow, sharedMemory->profiler->inWindowSize,lstmW, cnnW, abs)[0];
        }
    }
    // std::cout << this->sharedMemory->history->data[this->sharedMemory->history->index] << std::endl;

    
    return profile;
}
