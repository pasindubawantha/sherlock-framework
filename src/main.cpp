#include <iostream>
#include <algorithm>
#include <vector>
#include "LSTMCNnet.hpp"
#include "logger.hpp"
#include <json.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

rapidjson::Document parseInputFile(std::string input_file_name);
void printJSON(rapidjson::Document* document);

// Parse input JSON file to document object
rapidjson::Document parseInputFile(std::string input_file_name) {
    // Reading input JSON file to a string
    std::string line;
    std::string input_file_string = "";
    std::ifstream input_file (input_file_name);
    if (input_file.is_open())
    {
        while ( getline (input_file,line) )
        {
            input_file_string = input_file_string + line;
        }
        input_file.close();
        // std::cout << input_file_string << "\n";
    }
    else std::cout << "Unable to open file : " << input_file_name << std::endl; 

    // converting string to char array
    char json[input_file_string.length()]; 
    int i; 
    for (i = 0; i < sizeof(json); i++) { 
        json[i] = input_file_string[i];  
    } 

    // parsing char array to JSON document
    rapidjson::Document input_document;
    input_document.Parse(json);
    printJSON(&input_document);
    return input_document;
}

// Print JSON document object
void printJSON(rapidjson::Document* document) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document->Accept(writer);

    std::cout << buffer.GetString() << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "a";

    if(argc < 2)
        return 0;
    
    std::string input_file_name = argv[1];

    std::ifstream input_file_json(input_file_name);
    nlohmann::json input_document;
    input_file_json >> input_document;

    // Building model
    std::cout << "Building model in file : "<< input_file_name << std::endl;
    
    // Initializing the structure
    ModelStruct modelStruct;
    modelStruct.trainDataSize = (int)input_document["prediction_model"]["trainDataSize"];
    std::cout << "Got trainDataSize from : "<< input_file_name << " : " << modelStruct.trainDataSize << std::endl;

    modelStruct.learningRate = (double)input_document["prediction_model"]["learningRate"];
    std::cout << "Got learningRate from : "<< input_file_name << " : " << modelStruct.learningRate << std::endl;

    modelStruct.trainingIterations = (int)input_document["prediction_model"]["trainingIterations"]; 
    std::cout << "Got trainingIterations from : "<< input_file_name << " : " << modelStruct.trainingIterations << std::endl;

    modelStruct.numPredPoints = (int)input_document["prediction_model"]["numPredPoints"];
    std::cout << "Got numPredPoints from : "<< input_file_name << " : " << modelStruct.numPredPoints << std::endl;

    modelStruct.dataFile = (std::string)input_document["input_file"];
    std::cout << "Got dataFile from : "<< input_file_name << " : " << modelStruct.dataFile << std::endl;
    
    // LSTM parameters
    modelStruct.memCells = (int)input_document["prediction_model"]["model"]["LSTM"]["memCells"];
    std::cout << "Got LSTM params from : "<< input_file_name << std::endl;

    // CNN parameters
    modelStruct.matWidth = (int)input_document["prediction_model"]["model"]["CNN"]["matWidth"];
    modelStruct.matHeight = (int)input_document["prediction_model"]["model"]["CNN"]["matHeight"];
    modelStruct.targetC = (int)input_document["prediction_model"]["model"]["CNN"]["targetC"];

    std::vector<nlohmann::json> ConvolutionLayers;
    input_document["prediction_model"]["model"]["CNN"]["ConvolutionLayers"].get_to(ConvolutionLayers);

    struct::ConvLayStruct CLs[(int)ConvolutionLayers.size()];
    for(int i=0; i<(int)ConvolutionLayers.size(); i++) {
        CLs[i].filterSize = (int)ConvolutionLayers[i]["filterSize"]; // filter size: N x N
        CLs[i].filters = (int)ConvolutionLayers[i]["filters"]; // No of filters
        CLs[i].stride = (int)ConvolutionLayers[i]["stride"];
    }

    // Pooling layers
    std::vector<nlohmann::json> PoolingLayers;
    input_document["prediction_model"]["model"]["CNN"]["PoolingLayers"].get_to(PoolingLayers);
    
    struct::PoolLayStruct PLs[(int)PoolingLayers.size()];
    for(int i=0; i<(int)PoolingLayers.size(); i++) {
        PLs[i].poolH = (int)PoolingLayers[i]["poolH"];  // pool size: N x N
        PLs[i].poolW = (int)PoolingLayers[i]["poolW"]; 
    }

    // Fully connected layers
    std::vector<nlohmann::json> FullyConnectedLayers;
    input_document["prediction_model"]["model"]["CNN"]["FullyConnectedLayers"].get_to(FullyConnectedLayers);
    
    struct::FCLayStruct FCLs[(int)FullyConnectedLayers.size()];
    for(int i=0; i<(int)FullyConnectedLayers.size(); i++) {
        FCLs[i].outputs = (int)FullyConnectedLayers[i]["outputs"];  // neurons in fully connected layer
    }

    // Setting layer order 
    std::vector<nlohmann::json> layerOrderJSON;
    input_document["prediction_model"]["model"]["CNN"]["LayerOrder"].get_to(layerOrderJSON);
    
    char layerOrder[(int)layerOrderJSON.size()];
    std::string layer;
    for(int i=0; i<(int)layerOrderJSON.size(); i++) {
        layer =  (std::string)layerOrderJSON[i];
        layerOrder[i] = layer[0];  // neurons in fully connected layer
    }

    std::cout << "Got CNN params from : "<< input_file_name << std::endl;
    modelStruct.netStruct.layers = (int)layerOrderJSON.size();
    modelStruct.netStruct.layerOrder = layerOrder;
    modelStruct.netStruct.CL = CLs;
    modelStruct.netStruct.PL = PLs;
    modelStruct.netStruct.FCL = FCLs;
    
    

    std::cout << "Done building model in file : "<< input_file_name << " !" <<std::endl;
    // Initializing the Detection model
    LSTMCNNFCPredictionModel pm(&modelStruct);
    // Training the networks in the model 
    std::cout << "Training model in file : "<< input_file_name << std::endl;
    pm.train();
    
    // path for the target data file 
    std::string expect = (std::string)input_document["expect_file"];
    std::cout << "Model in file expect file: "<< input_file_name << " : " << expect << std::endl;

    // path for the predicted data file
    std::string predict = (std::string)input_document["predict_file"];
    std::cout << "Model in file predict file: "<< input_file_name << " : " << predict << std::endl;
    
    // parameters for model outputs
    double lstmW = (double)input_document["prediction_model"]["model"]["lstmW"];
    double cnnW = (double)input_document["prediction_model"]["model"]["cnnW"];
    int input_window_size = modelStruct.matWidth * modelStruct.matHeight;
    int predictions = (int)input_document["input_size"] - input_window_size;
    
    int simVecSize = 2;
    int marker = 4000;
    int similarityMargin = 4800;
    
    int abs = 1;

    
    // getting predicted time series data points
    std::cout << "Writing output for model in file : "<< input_file_name << std::endl;
    // Used in evaluvatoin
    pm.predict(predictions, expect, predict, lstmW, cnnW, abs);
    
    // getting anomalies identified by the model 
    // Used in evaluvatoin
    pm.predict(predictions, expect, predict, simVecSize, marker, similarityMargin, lstmW, cnnW);
    
    // getting DTW similarity values
    pm.dtwSimilarity(predictions, expect, predict, simVecSize, lstmW, cnnW);
    
    // getting predicted time series data points
    // using normal behavior to identify increase the accuracy of predictions
    logger::log("### DEBUG ###", "test");
    logger::log("### DEBUG ###", modelStruct.dataFile);
    pm.predictNorm(predictions, expect, predict, lstmW, cnnW);
    
    // getting anomalies identified by the model 
    // using normal behavior to identify increase the accuracy of predictions
    // pm.predictNorm(predictions, expect, predict, simVecSize, marker, similarityMargin, lstmW, cnnW);
    
    std::cout << "Done processing model in file : "<< input_file_name << std::endl;
    return 0;
    
}
