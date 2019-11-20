/**
 * This demonstration is used to debug the framework in single thread mode
 * 
 * */
#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <mutex> 
#include <memory>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <json.hpp>
#include <iomanip>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h> 


#include "MainLoop.h"
#include "HistoryBuffer.h"
#include "MutexLock.h"
#include "SharedMemory.h"

#include "Profiler.h"
#include "DefaultProfiler.h"
#include "LSTMCNnetProfiler.h"

#include "AnomalyDistanceMeasure.h"
#include "FastDTWAnomalyDistanceMeasure.h"

#include "AnomalyThresholdSetter.h"
#include "TrainingMaxAnomalyThresholdSetter.h"
#include "ConfidenceIntervalAnomalyThresholdSetter.h"

#include "AnomalyDetector.h"
#include "DefaultAnomalyDetector.h"
#include "ConfidenceIntervalAnomalyDetector.h"

#include "ConceptDistanceMeasure.h"
#include "FastDTWConceptDistanceMeasure.h"

#include "ConceptThresholdSetter.h"
#include "ConfidenceIntervalConceptThresholdSetter.h"

#include "ConceptDriftDetector.h"
// #include "NoConceptDriftDetector.h"
#include "DynamicWindowConceptDriftDetector.h"


void runLSTMCNnet(std::string inputFileName, std::string outputFileName);
void processConfigJSON(std::string fileJSON);
void processDataSet(std::string filePath, std::string fileName);
void configureMainLoop();

HistoryBuffer *historyBuffer;
Profiler *profiler;
SharedMemory *sharedMemory;
MainLoop *mainLoop;
ModelStruct *modelStruct;


std::string inputFileName = "../data/nab_tuned/test/Twitter_volume_AAPL.csv"; // set by args
std::string configurationFileName = "../data/nab_tuned/test/Twitter_volume_AAPL.json"; //set by args
std::string outputFileName = "../data/nab_tuned_results/test1/Twitter_volume_AAPL.csv"; // set by args

bool verbose = false;
int processedCount = 0;
int processesLimit = -1;
double predictionTrainingRatio = 0.1; // set by args
double thresholdTrainingRatio = 0.045; // set by args
double thresholdMaxMultipler = 2.0; // set by args

struct ROW {
    std::string timestamp;
    double value;
    bool prediction_training;
    double prediction;
    bool label;
    double warp_distance;
    bool threshold_training;
    double distance_threshold;
    bool positive_detection;
    
    ROW(){
        value = 0.0;
        prediction_training = true;
        prediction = 0.0;
        label = false;
        warp_distance = 0.0;
        threshold_training = true;
        distance_threshold = 0.0;
        positive_detection = false;
    }
};




int main(int argc, char** argv) 
{

    if(argc < 7){
        std::cout << "[main] Provide arguments : '$Sherlock inputFileName outputFileName configurationFileName predictionTrainingRatio thresholdTrainingRatio thresholdMaxMultipler'" <<std::endl;
        return 0;
    }
    inputFileName = argv[1];
    std::cout << "[main] inputFileName = " << inputFileName << std::endl;
    outputFileName = argv[2];
    std::cout << "[main] outputFileName = " << outputFileName << std::endl;
    configurationFileName = argv[3];
    std::cout << "[main] configurationFileName = " << configurationFileName << std::endl;

    predictionTrainingRatio = atof(argv[4]);
    std::cout << "[main] predictionTrainingRatio = " << predictionTrainingRatio << std::endl;
    thresholdTrainingRatio = atof(argv[5]);
    std::cout << "[main] thresholdTrainingRatio = " << thresholdTrainingRatio << std::endl;
    thresholdMaxMultipler = atof(argv[6]);
    std::cout << "[main] thresholdMaxMultipler = " << thresholdMaxMultipler << std::endl;

    std::cout << "[main] Starting" <<std::endl;


    processConfigJSON(configurationFileName); // it configures MainLoop too 
    runLSTMCNnet(inputFileName, outputFileName);
        
    
    std::cout << "[main] Ended" << std::endl;
    return 0;
}

void processConfigJSON(std::string fileJSON){
    
    std::ifstream fileJSONStream(fileJSON);
    nlohmann::json JSONDocument;
    fileJSONStream >> JSONDocument;

    // Initializing the structure
    modelStruct = new ModelStruct();

    int input_size = (int)JSONDocument["input_size"];

    modelStruct->learningRate = (double)JSONDocument["prediction_model"]["learningRate"];
    std::cout << "[main:processConfigJSON] Got learningRate from : "<< fileJSON << " : " << modelStruct->learningRate << std::endl;

    modelStruct->trainingIterations = (int)JSONDocument["prediction_model"]["trainingIterations"]; 
    std::cout << "[main:processConfigJSON] Got trainingIterations from : "<< fileJSON << " : " << modelStruct->trainingIterations << std::endl;

    modelStruct->numPredPoints = 1;

    // LSTM parameters
    modelStruct->memCells = (int)JSONDocument["prediction_model"]["model"]["LSTM"]["memCells"];
    std::cout << "[main:processConfigJSON] Got LSTM params from : "<< fileJSON << std::endl;

    // CNN parameters
    modelStruct->matWidth = (int)JSONDocument["prediction_model"]["model"]["CNN"]["matWidth"];
    modelStruct->matHeight = (int)JSONDocument["prediction_model"]["model"]["CNN"]["matHeight"];
    modelStruct->targetC = (int)JSONDocument["prediction_model"]["model"]["CNN"]["targetC"];
    
    std::vector<nlohmann::json> ConvolutionLayers;
    JSONDocument["prediction_model"]["model"]["CNN"]["ConvolutionLayers"].get_to(ConvolutionLayers);

    struct::ConvLayStruct *CLs = new struct::ConvLayStruct[(int)ConvolutionLayers.size()];
    for(int i=0; i<(int)ConvolutionLayers.size(); i++) {
        CLs[i].filterSize = (int)ConvolutionLayers[i]["filterSize"]; // filter size: N x N
        CLs[i].filters = (int)ConvolutionLayers[i]["filters"]; // No of filters
        CLs[i].stride = (int)ConvolutionLayers[i]["stride"];
    }

    // Pooling layers
    std::vector<nlohmann::json> PoolingLayers;
    JSONDocument["prediction_model"]["model"]["CNN"]["PoolingLayers"].get_to(PoolingLayers);
    
    struct::PoolLayStruct *PLs = new struct::PoolLayStruct[(int)PoolingLayers.size()];
    for(int i=0; i<(int)PoolingLayers.size(); i++) {
        PLs[i].poolH = (int)PoolingLayers[i]["poolH"];  // pool size: N x N
        PLs[i].poolW = (int)PoolingLayers[i]["poolW"]; 
    }

    // Fully connected layers
    std::vector<nlohmann::json> FullyConnectedLayers;
    JSONDocument["prediction_model"]["model"]["CNN"]["FullyConnectedLayers"].get_to(FullyConnectedLayers);
    
    struct::FCLayStruct *FCLs = new struct::FCLayStruct[(int)FullyConnectedLayers.size()];
    for(int i=0; i<(int)FullyConnectedLayers.size(); i++) {
        FCLs[i].outputs = (int)FullyConnectedLayers[i]["outputs"];  // neurons in fully connected layer
    }

    // Setting layer order 
    std::vector<nlohmann::json> layerOrderJSON;
    JSONDocument["prediction_model"]["model"]["CNN"]["LayerOrder"].get_to(layerOrderJSON);
    
    char *layerOrder = new char[(int)layerOrderJSON.size()];
    std::string layer;
    for(int i=0; i<(int)layerOrderJSON.size(); i++) {
        layer =  (std::string)layerOrderJSON[i];
        layerOrder[i] = layer[0];  // neurons in fully connected layer
    }

    std::cout << "[main:processConfigJSON] Got CNN params from : "<< fileJSON << std::endl;
    modelStruct->netStruct.layers = (int)layerOrderJSON.size();
    modelStruct->netStruct.layerOrder = layerOrder;
    modelStruct->netStruct.CL = CLs;
    modelStruct->netStruct.PL = PLs;
    modelStruct->netStruct.FCL = FCLs;
    


    std::cout << "[main:processConfigJSON] Done building model in file : "<< fileJSON << " !" <<std::endl;


    modelStruct->trainDataSize = (int)(input_size*predictionTrainingRatio) + 1 - (int)(modelStruct->matHeight*modelStruct->matWidth);

    // # History Buffer
    historyBuffer = new HistoryBuffer();
    int historyBufferSize = modelStruct->trainDataSize+(modelStruct->matHeight*modelStruct->matWidth);
    double *bufferHistory = new double[historyBufferSize]; 
    historyBuffer->setSize(historyBufferSize, bufferHistory);

    double lstmW = (double)JSONDocument["prediction_model"]["model"]["lstmW"];
    // double lstmW = 1.0;
    std::cout << "[main:processConfigJSON] Got LSTM Weight from : "<< fileJSON << " : " << lstmW << std::endl;

    double cnnW = (double)JSONDocument["prediction_model"]["model"]["cnnW"];
    // double cnnW = 0.0;
    std::cout << "[main:processConfigJSON] Got CNN Weight from : "<< fileJSON << " : " << cnnW << std::endl;

    // # LSTMCNnetProfiler
    // setting profiler (do not delete pointers)
    profiler = new LSTMCNnetProfiler("lstmcnnnet 1", modelStruct, lstmW, cnnW);
    

    // # Shared Memory 

    // ## profiler
    int profilerSize = (int)JSONDocument["dtw_window"];
    ProfilerMemory *profilerMemory = new ProfilerMemory(profilerSize);
    profilerMemory->inWindowSize = modelStruct->matWidth*modelStruct->matHeight;
    profilerMemory->OutWindowSize = 1;
    profilerMemory->minTrainingWindowSize = modelStruct->trainDataSize + (modelStruct->matHeight*modelStruct->matWidth);

    // ### anomaly detector
    int anomlayDetectorInWindowSize = (int)JSONDocument["dtw_window"];
    int anomalyDistanceSize = (int)((input_size - (modelStruct->trainDataSize+(modelStruct->matHeight*modelStruct->matWidth)))*thresholdTrainingRatio) - anomlayDetectorInWindowSize;
    int anomlayWarrningSize = 1; 
    int anomalyAlarmSize = 1;
    AnomalyDetectorMemory *anomalyDetectorMemory = new AnomalyDetectorMemory(anomalyDistanceSize, anomlayWarrningSize, anomalyAlarmSize);
    anomalyDetectorMemory->inWindowSize = anomlayDetectorInWindowSize;

    // ### concept drift detector
    int conceptDetectorInWindowSize = anomlayDetectorInWindowSize;
    int conceptDistanceSize = anomalyDistanceSize;
    int conceptWarningSize = 1;
    int conceptAlarmSize = 1;
    ConceptDriftDetectorMemory *conceptDriftDetectorMemory = new ConceptDriftDetectorMemory(conceptDistanceSize, conceptWarningSize, conceptAlarmSize);
    conceptDriftDetectorMemory->inWindowSize=conceptDetectorInWindowSize;
    
    sharedMemory = new SharedMemory(profilerMemory, anomalyDetectorMemory, conceptDriftDetectorMemory);
    sharedMemory->history = new Queue<double>(historyBufferSize);

    configureMainLoop();
}

void configureMainLoop(){

    mainLoop = new MainLoop(verbose);

    // set historyBuffer
    mainLoop->setHistoryBuffer(historyBuffer);

    // ## Anomaly Detection
    // setting anomaly distance measure
    AnomalyDistanceMeasure *anomalyDestanceMeasure = new FastDTWAnomalyDistanceMeasure("fastDTW 1");
    mainLoop->setAnomalyDistanceMeasure(anomalyDestanceMeasure);

    // setting anomaly threshold setter
    double maxMultiplierAlarm = thresholdMaxMultipler;
    double maxMultiplierWarning = 1.5;
    AnomalyThresholdSetter *anomalyThresholdSetter = new TrainingMaxAnomalyThresholdSetter("maxAnomalyThrehsolder 1", maxMultiplierWarning, maxMultiplierAlarm);
    // double alphaWarrningAnomaly = 2;
    // double alphaAlarmAnomaly = 3;
    // AnomalyThresholdSetter *anomalyThresholdSetter = new ConfidenceIntervalAnomalyThresholdSetter("Confidence Interval Anomaly Threshold Setter", alphaWarrningAnomaly, alphaAlarmAnomaly);
    mainLoop->setAnomalyThresholdSetter(anomalyThresholdSetter);

    // setting anomaly threshold setter
    AnomalyDetector *anomalyDetector = new DefaultAnomalyDetector("defaultAnomalyDetector 1");
    // AnomalyDetector *anomalyDetector = new ConfidenceIntervalAnomalyDetector("ConfidenceIntervalAnomalyDetector");
    mainLoop->setAnomalyDetector(anomalyDetector);

    // ## Concept Drift Detection
    // setting concept distance measure
    ConceptDistanceMeasure *conceptDestanceMeasure = new FastDTWConceptDistanceMeasure("fastDTW Concept1");
    mainLoop->setConceptDistanceMeasure(conceptDestanceMeasure);

    // setting concept threshold setter
    double alphaWarrning = 2;
    double alphaAlarm = 3;
    ConceptThresholdSetter *conceptThresholdSetter = new ConfidenceIntervalConceptThresholdSetter("Confidence Interval Concept Threshold Setter", alphaWarrning, alphaAlarm);
    mainLoop->setConceptThresholdSetter(conceptThresholdSetter);

    // setting concept drift detector
    // ConceptDriftDetector *conceptDriftDetector = new NoConceptDriftDetector("NoCDD");
    ConceptDriftDetector *conceptDriftDetector = new DynamicWindowConceptDriftDetector("CDD");
    mainLoop->setConceptDriftDetector(conceptDriftDetector);


    mainLoop->setProfiler(profiler);

    mainLoop->setSharedMemory(sharedMemory);
}

void runLSTMCNnet(std::string inputFileName, std::string outputFileName){
    /**
     * Processes input files in NAB format.
     * CSV files with header "timestamp,value,anomaly_score,label,S(t)_reward_low_FP_rate,S(t)_reward_low_FN_rate,S(t)_standard"
     * 
     */
    
    // File pointer 
    std::fstream inputStream {inputFileName, std::ios::in}; // open inputFileName
    std::fstream outputStream {outputFileName, std::ios::out}; // open inputFileName
  
    // Read the Data from the file 
    // as String Vector 
    std::vector<std::string> row; 
    std::string line, word, temp;
    int lineNumber = 1;
    bool correctHeader = false;

    ROW writeROW;

    while (!inputStream.eof()) { 
  
        row.clear(); 
  
        // read an entire row and 
        // store it in a string variable 'line' 
        std::getline(inputStream, line); 
        // used for breaking words 
        std::stringstream stringStream(line); 

        if(line != ""){
            // read every column data of a row and 
            // store it in a string variable, 'word' 
            while(std::getline(stringStream, word, ',')) { 
                // add all the column data 
                // of a row to a vector 
                row.push_back(word); 
            } 
            
            // check if header it is in correct format
            if(lineNumber == 1){// header
                if(row[0] == "timestamp" &&
                    row[1] == "value" &&
                    row[2] == "anomaly_score" &&
                    row[3] == "label"){
                        correctHeader = true;
                    std::cout << "[main:runLSTMCNnet] Input file header is correct !" << std::endl;
                } else {
                    std::cout << "[main:runLSTMCNnet] Input file header is Incorrect !" << std::endl; 
                }
                outputStream << "timestamp,value,prediction_training,prediction,label,warp_distance,threshold_training,distance_threshold,positive_detection" << std::endl;
                lineNumber++; 
            } else if(correctHeader){
                writeROW.timestamp = row[0];
                writeROW.value = std::stod(row[1]); // get double
                writeROW.label = std::stoi(row[3]); // get int
                lineNumber++;

                // Run MainLoop Tick
                historyBuffer->writeSafe(writeROW.value); // wtite to history
                mainLoop->tick();
                historyBuffer->setTickedPrevious(true);

                // Get data and write to outputfile
                outputStream << writeROW.timestamp << ",";
                outputStream << writeROW.value << ",";

                writeROW.prediction_training = sharedMemory->profiler->training;
                
                // outputStream << std::setprecision(1);
                outputStream << (double)writeROW.prediction_training << ",";

                if(sharedMemory->profiler->profile->index < 0){
                    writeROW.prediction = 0.0;
                } else {
                    writeROW.prediction = sharedMemory->profiler->profile->data[sharedMemory->profiler->profile->index];
                }
                outputStream << (double)writeROW.prediction << ",";
                outputStream << ((double)writeROW.label) << ",";
                
                if(sharedMemory->anomalyDetector->distance->index < 0){
                    writeROW.warp_distance = 0.0;
                } else {
                    writeROW.warp_distance = sharedMemory->anomalyDetector->distance->data[sharedMemory->anomalyDetector->distance->index];
                }
                outputStream << (double)writeROW.warp_distance << ",";

                writeROW.threshold_training = sharedMemory->anomalyDetector->training;
                if(sharedMemory->profiler->training){
                    writeROW.threshold_training = false;
                }
                outputStream << (double)writeROW.threshold_training << ",";

                writeROW.distance_threshold = sharedMemory->anomalyDetector->thresholdAlram;
                // if(writeROW.distance_threshold < 0){
                //     writeROW.distance_threshold = 0.0;
                // }
                outputStream << (double)writeROW.distance_threshold << ",";

                if(sharedMemory->anomalyDetector->alarm->index < 0){
                    writeROW.positive_detection = false;
                } else {
                    writeROW.positive_detection = sharedMemory->anomalyDetector->alarm->data[sharedMemory->anomalyDetector->alarm->index];
                }
                outputStream << (double)writeROW.positive_detection ;
                
                outputStream << std::endl;; 
                if (isnan(writeROW.prediction)){
                    break;
                }
            }
        }
    }

    delete mainLoop;
    mainLoop = NULL;
    // delete historyBuffer;
    // delete sharedMemory;
    // delete profiler;
    std::cout << "[main] Done Processing file !" << std::endl;
}