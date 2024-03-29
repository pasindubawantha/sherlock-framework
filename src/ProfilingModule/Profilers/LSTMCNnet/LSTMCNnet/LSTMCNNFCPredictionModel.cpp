/* 
 * File:   LSTMCNNFCPredictionModel.cpp
 * Author: heshan
 * 
 * Created on August 7, 2018, 10:10 PM
 */

#include "LSTMCNNFCPredictionModel.hpp"
#include <math.h>   

LSTMCNNFCPredictionModel::LSTMCNNFCPredictionModel() { }

LSTMCNNFCPredictionModel::LSTMCNNFCPredictionModel(ModelStruct * modelStruct) {
    this->modelStruct = modelStruct;
}

LSTMCNNFCPredictionModel::LSTMCNNFCPredictionModel(const LSTMCNNFCPredictionModel& orig) { }

LSTMCNNFCPredictionModel::~LSTMCNNFCPredictionModel() {
    std::cout << "[LSTMCNNFCPredictionModel] Destroying !" << std::endl;
    delete lstm;
    delete cnn;
    // delete modelStruct;
    // delete fileProc;
    // delete dataproc;
 }

int LSTMCNNFCPredictionModel::train(double *trainingTimeSeriesArray, int trainingTimeSeriesArraySize) {

    int trainDataSize = modelStruct->trainDataSize; // train data size
    double learningRate = modelStruct->learningRate;
    int iterations = modelStruct->trainingIterations; // training iterations with training data
    
    // CNN parameters
    int height = modelStruct->matHeight;
    int width = modelStruct->matWidth;
    int targetsC = modelStruct->targetC;
    
    // LSTM parameters
    int memCells = modelStruct->memCells; // number of memory cells
    int inputVecSize = height*width; // input vector size
    int timeSteps = inputVecSize; // unfolded time steps

    // Adding the time series in to a vector and preprocessing
    dataproc = new DataProcessor();
    fileProc = new FileProcessor();

    // timeSeries2 = fileProc->read(modelStruct->dataFile,1);
    std::vector<double> trainingTimeSeries;
    for(int i=0; i<=trainingTimeSeriesArraySize; i++){
        trainingTimeSeries.push_back(trainingTimeSeriesArray[i]);
    }
    timeSeries2 = trainingTimeSeries;
    timeSeries =  dataproc->process(timeSeries2,1);

    // Creating the input vector Array for LSTM
    std::vector<double> * input;
    input = new std::vector<double>[trainDataSize];
    std::vector<double> inputVec;

    for (int i = 0; i < trainDataSize; i++) {
        inputVec.clear();
        for (int j = 0; j < inputVecSize; j++) {
            inputVec.push_back(timeSeries.at(i+j));
        }
        inputVec =  dataproc->process(inputVec,0);
        input[i] = inputVec; 
    }


    // Creating the target vector for LSTM
    std::vector<double>::const_iterator first = timeSeries.begin() + inputVecSize;
    std::vector<double>::const_iterator last = timeSeries.begin() + inputVecSize + trainDataSize;
    std::vector<double> targetVector(first, last);

    // Training the LSTM net
    this->lstm = new LSTMNet(memCells,inputVecSize);
    lstm->train(input, targetVector, trainDataSize, timeSteps, learningRate, iterations);
    

    // CNN network structure
    std::tuple<int, int, int> dimensions = std::make_tuple(1,height,width);

    // Generating input matrices for CNN
    Eigen::MatrixXd ** inMatArr;
    Eigen::MatrixXd * inLblArr;
    Eigen::MatrixXd inMat;
    Eigen::MatrixXd inLbl;
    inMatArr = new Eigen::MatrixXd * [trainDataSize];
    inLblArr = new Eigen::MatrixXd[trainDataSize];

    for (int i = 0; i < trainDataSize; i++) {
        // inputs
        inMatArr[i] = new Eigen::MatrixXd[1]; // image depth
        inMat = Eigen::MatrixXd(height,width);
        for (int a = 0; a < height; a++) {
            for (int b = 0; b < width; b++) {
                inMat(a,b) = timeSeries.at(i + ( a * width ) + b);
            }
        }
        inMatArr[i][0] = inMat;
        
        // labels
        inLbl = Eigen::MatrixXd::Zero(targetsC,1);
        for (int a = 0; a < targetsC; a++) {
            inLbl(a,0) = timeSeries.at(i + (width*height));
        }
        inLblArr[i] = inLbl;
    }

    // Generating the CNN
    this->cnn = new CNN(dimensions, modelStruct->netStruct);
    // Training the network
    cnn->train(inMatArr, inLblArr, trainDataSize, iterations, learningRate);

    // delete input;
    return 0;
}


double* LSTMCNNFCPredictionModel::predict(int predSize, double* inWindow, int inWindowSize, double lstmW, double cnnW, int abs) {
    
    double  val;
    
    // LSTM parameters
    double result;
    double *returnResult = new double[predSize];
    
    // CNN parameters
    int height = modelStruct->matHeight;
    int width = modelStruct->matWidth;
    Eigen::MatrixXd tstMatArr[1];
    
    int inputVecSize = height*width; // input vector size
    int trainingDataSize = modelStruct->trainDataSize; 
    int inputSize = modelStruct->trainDataSize; 
    int numPredPoints = modelStruct->numPredPoints;

    // Predictions
    Eigen::MatrixXd prediction;
    


    // CNN Inputs
    std::vector<double> inVec;
    
    // LSTM Inputs
    std::vector<double> * input;
    input = new std::vector<double>[1];
    
    double predPoints[numPredPoints];
    double lstmPredPoints[numPredPoints];

    for (int j = 0; j < numPredPoints; j++) {
        predPoints[j] = 0;
        lstmPredPoints[j] = 0;
    }
    
    
    // max and min training values [ CNN ]
    double trainMax = *std::max_element(timeSeries.begin(), timeSeries.begin()+(trainingDataSize+(width*height)));
    double trainMin = *std::min_element(timeSeries.begin(), timeSeries.begin()+(trainingDataSize+(width*height)));
    // max and min predicted values [ CNN ]
    double predictMax = std::numeric_limits<double>::min();
    double predictMin = std::numeric_limits<double>::max();

    if(firstPrediction){
        for (int i = 0; i < trainingDataSize; i++) {
        
            inVec.clear();
            // filling the input vector using time series data
            for (int j = 0; j < inputVecSize; j++) {
                inVec.push_back(timeSeries2.at(i+j));
            }
            inVec = dataproc->process(inVec,0);
        
            
            // LSTM network predictions for the trained data set
            input[0] = inVec;
            for (int j = 0; j < numPredPoints; j++) {          
                result = lstm->predict(input); 
                input[0] = std::vector<double>(inVec.begin()+1, inVec.begin()+inputVecSize);
                input[0].push_back(result);
                lstmPredPoints[((i+inputVecSize+j)%numPredPoints)] += result;     
            }
            lstmPredPoints[((i+inputVecSize)%numPredPoints)] = 0;

            
            // CNN predictions for the trained data set
            tstMatArr[0] = Eigen::MatrixXd::Zero(height,width);
            for (int a = 0; a < height; a++) {
                for (int b = 0; b < width; b++) {
                    tstMatArr[0](a,b) = inVec.at(( a * width ) + b);
                }
            }

            for (int j = 0; j < numPredPoints; j++) {      
                prediction = cnn->predict(tstMatArr);
                inVec = std::vector<double>(inVec.begin()+1, inVec.begin()+inputVecSize);
                inVec.push_back(prediction(0,0));
                for (int a = 0; a < height; a++) {
                    for (int b = 0; b < width; b++) {
                        tstMatArr[0](a,b) = inVec.at(( a * width ) + b);
                    }
                }
                predPoints[((i+inputVecSize+j)%numPredPoints)] += prediction(0,0);     
            }

            if (i >= numPredPoints-1) {
                prediction(0,0) = predPoints[((i+inputVecSize)%numPredPoints)]/(double)numPredPoints;
                if (prediction(0,0) > predictMax) predictMax = prediction(0,0);
                if (prediction(0,0) < predictMin) predictMin = prediction(0,0);
            }
            predPoints[((i+inputVecSize)%numPredPoints)] = 0;
        }
        firstPrediction = false;
    }
    

    // std::vector<double> newTimeSeries2;

    // for(int i=inWindowSize-1; i<trainingDataSize; i++){
    //     newTimeSeries2.push_back(timeSeries2.at(i));
    // }

    std::vector<double> inWindowVector;
    for(int i=0; i<inWindowSize; i++){
        inWindowVector.push_back(inWindow[i]);
        // newTimeSeries2.push_back(inWindow[i]);
    }

    // timeSeries =  dataproc->process(timeSeries2,1);


    for (int i = 0; i < predSize; i++) {

        inVec.clear();
        for (int j = 0; j < inputVecSize; j++) {
            inVec.push_back(inWindowVector.at(i+j));
        }
        inVec = dataproc->process(inVec,0);
        
        // Filling the matrix for the CNN input
        tstMatArr[0] = Eigen::MatrixXd::Zero(height,width);
        for (int a = 0; a < height; a++) {
            for (int b = 0; b < width; b++) {
                tstMatArr[0](a,b) = inVec.at(( a * width ) + b);
            }
        }
        
        // LSTM predictions
        input[0] = inVec;
        for (int j = 0; j < numPredPoints; j++) {          
            result = lstm->predict(input); 
            input[0] = std::vector<double>(inVec.begin()+1, inVec.begin()+inputVecSize);
            input[0].push_back(result);
            lstmPredPoints[((i+inputVecSize+j)%numPredPoints)] += result;     
        }
        result = lstmPredPoints[((i+inputVecSize)%numPredPoints)]/(double)numPredPoints;
        lstmPredPoints[((i+inputVecSize)%numPredPoints)] = 0;

        // CNN predictions
        for (int j = 0; j < numPredPoints; j++) {      
            prediction = cnn->predict(tstMatArr);
            inVec = std::vector<double>(inVec.begin()+1, inVec.begin()+inputVecSize);
            inVec.push_back(prediction(0,0));
            for (int a = 0; a < height; a++) {
                for (int b = 0; b < width; b++) {
                    tstMatArr[0](a,b) = inVec.at(( a * width ) + b);
                }
            }
            predPoints[((i+inputVecSize+j)%numPredPoints)] += prediction(0,0);     
        }
        prediction(0,0) = predPoints[((i+inputVecSize)%numPredPoints)]/(double)numPredPoints;
        predPoints[((i+inputVecSize)%numPredPoints)] = 0;
        
        // post process CNN prediction
        val = prediction(0,0);
        val = (val - predictMin)*((trainMax - trainMin)/(predictMax - predictMin)) + trainMin;
        
        // combining the results LSTM and CNN
        // check if cnn output is NaN
        if(val == val){
            // check if cnn output is NaN
            if(result == result){
                // combining the results LSTM and CNN
                val = (result*lstmW + val*cnnW);
            }
            // final output is only CNN
        }
        else{
            // final output is only CNN
            val = result;
        }
         
        // calculating the Mean Squared Error
        // expected = timeSeries.at(i+inputVecSize);
        // get the absolute value
        if (abs) val = std::abs(val);
        // errorSq += std::pow(expected-val,2);
        result = dataproc->postProcess(val);
        
        returnResult[i] = result;

    }
    
    // delete input;
    return returnResult;
}
