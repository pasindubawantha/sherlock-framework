/* 
 * File:   Preprocessor.cpp
 * Author: heshan
 * 
 * Created on April 20, 2018, 8:00 PM
 */

#include "DataProcessor.h"

#include <cmath>
#include <iostream>

DataProcessor::DataProcessor() {
    out_magnitude = 0.0;
}

DataProcessor::DataProcessor(const DataProcessor& orig) { }

DataProcessor::~DataProcessor() { }

std::vector<double> DataProcessor::process(std::vector<double> vec, int vecType) {

    //std::cout<<"processing...\n";
    double min_train = DBL_MAX;
    // get magnitude of vector by pythogros therom
    double magnitude = 0.0;
    for(std::vector<double>::iterator it = vec.begin(); it != vec.end(); ++it) {
        magnitude += std::pow(*it,2);
    } 
    magnitude = std::pow(magnitude,0.5);
    
    // if target vector
    if (vecType == 1) out_magnitude = magnitude;
    
    // if not target vector devide elements by magnitude
    if (magnitude != 0) {
        for(std::vector<double>::iterator it = vec.begin(); it != vec.end(); ++it) {
            *it /= magnitude;
        }
    }
    
    return vec;
}

std::vector<double> DataProcessor::postprocess(std::vector<double> vec) {

//    std::cout<<"post processing...\n";
    
    for(std::vector<double>::iterator it = vec.begin(); it != vec.end(); ++it) {
        *it *= out_magnitude;
    }
    return vec;
}

double DataProcessor::postProcess(double val) {

//    std::cout<<"post processing...\n";
//    std::cout<<"\n"<<out_magnitude<<"******"<<"\n";
    return val*(out_magnitude);
}

int DataProcessor::printVector(std::vector<double> vec){
    
    for(std::vector<double>::iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it<<", ";
    }
    std::cout<<std::endl;
    return 0;
}
