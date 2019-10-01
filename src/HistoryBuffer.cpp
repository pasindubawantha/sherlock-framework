#include "HistoryBuffer.h"

#include <iostream>
#include <pthread.h>

// Constructors 
HistoryBuffer::HistoryBuffer()
{
    std::cout << "[HistoryBuffer] Constructing"  << std::endl;
}

// Destructor
HistoryBuffer::~HistoryBuffer()
{
    std::cout << "[HistoryBuffer] Destroying"  << std::endl;
    *history = NULL;
}

// Copy constructor
HistoryBuffer::HistoryBuffer(const HistoryBuffer &source)
{
    std::cout << "[HistoryBuffer] Copying"  << std::endl;
}

// Move constructor
HistoryBuffer::HistoryBuffer(HistoryBuffer &&source)// source is taken as an R value
{
    std::cout << "[HistoryBuffer] Moving"  << std::endl;
}

// methods

void HistoryBuffer::setSize(int size, double *buffer)
{
    std::cout << "[HistoryBuffer] Setting size " << size << std::endl;
    this->size = size;
    this->history = buffer;
}

bool HistoryBuffer::writeSafe(double value)
{
    if(!this->ticked_previous) return false;// MainLoop has not ticked previous element yet!

    if(real_index >= size){//Buffer overflow
        for(int i=0; i<size-1; i++){
            history[i] = history[i+1];
        }
        history[size-1] = value;
    } else {
        history[real_index] = value;
    }

    real_index++;

    this->ticked_previous = false;
    return true;
}

bool HistoryBuffer::writeUnsafe(double value)
{
    std::cout << "[HistoryBuffer | writeUnsafwe] warring this write does not ensure previous data is ticked by the MainLoop" << std::endl;

    if(real_index >= size){//Buffer overflow
        for(int i=0; i<size-1; i++){
            history[i] = history[i+1];
        }
        history[size-1] = value;
    } else {
        history[real_index] = value;
    }

    real_index++;
    ticked_previous = false;
    return true;
}

double* HistoryBuffer::copyHistoryArray()
{
    double *historyArray = new double[size];
    for(int i=0; i<size; i++){
        historyArray[i] = history[i];
    }


    return historyArray;
}

void HistoryBuffer::print()
{
    std::cout << "[HistoryBuffer] Printing buffer " << std::endl;
    for(int i=0; i < size; i++){
        std::cout << "[HistoryBuffer | print]  [" << i << "] " << history[i] << std::endl;
    }
}

void HistoryBuffer::print(int index)
{
    std::cout << "[HistoryBuffer | print]  [" << index << "] " << history[index] << std::endl;
}

int HistoryBuffer::getCurrentIndexToRead()
{
    int current_index_to_read;

    if(real_index < size){
        current_index_to_read = real_index - 1;
    } else {
        current_index_to_read = size - 1;
    }
    
    return current_index_to_read;
}