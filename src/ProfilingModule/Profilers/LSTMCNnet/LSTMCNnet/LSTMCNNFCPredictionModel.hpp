/* 
 * File:   LSTMCNNFCPredictionModel.hpp
 * Author: heshan
 *
 * Created on August 7, 2018, 10:10 PM
 */

/*
 * 
 * prediction methods: [predict:2, predictNorm:2, predictAdaptNorm:2]
 * 
 * 
 * predictNorm: Input is updated according to the normal pattern (training pattern)
 * predictAdaptNorm: stored normal pattern is updated
 * 
 * 
 */

#ifndef LSTMCNNFCPREDICTIONMODEL_HPP
#define LSTMCNNFCPREDICTIONMODEL_HPP

#include "PredictionModel.hpp"

class LSTMCNNFCPredictionModel {
public:
    LSTMCNNFCPredictionModel();
    
    /**
     * Initialize the LSTMCNNFC model
     * @param modelStruct: parameters for model
     */
    LSTMCNNFCPredictionModel(ModelStruct * modelStruct);
    LSTMCNNFCPredictionModel(const LSTMCNNFCPredictionModel& orig);
    virtual ~LSTMCNNFCPredictionModel();
    
    /**
     * Train the prediction model
     * @return 0
     */
    int train(double *trainingTimeSeriesArray, int trainingTimeSeriesArraySize);
    

    
   
    /**
     * Predict the given number of points and write the predicted values to given file
     * 
     * @param points: points to be predicted
     * @param expect: file path to write the expected values
     * @param predict: file path to write the predicted value
     * @param lstmW: prediction weight for the lstm (default value = 0.5, lstmW + cnnW = 1)
     * @param cnnW: prediction weight for the cnn (default value = 0.5, lstmW + cnnW = 1)
     * @param abs: get the absolute value of the prediction ( default = 0 : original prediction )
     * @return 0
     */
    double* predict(
        int points, double* inWondow, int inWindowSize, 
        double lstmW = 0.5, double cnnW = 0.5, int abs = 0
    );
    
    
    
private:
    LSTMNet * lstm;
    CNN * cnn;
    ModelStruct * modelStruct;
    FileProcessor * fileProc;
    DataProcessor * dataproc;
    std::vector<double> timeSeries;
    std::vector<double> timeSeries2;
    bool firstPrediction = true;

};

#endif /* LSTMCNNFCPREDICTIONMODEL_HPP */

