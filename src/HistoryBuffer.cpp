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

    if(real_index > size){//Buffer overflow
        current_index_to_write = current_index_to_write % size;
    }

    history[current_index_to_write] = value;
    current_index_to_write++;
    real_index++;

    this->ticked_previous = false;
    return true;
}

bool HistoryBuffer::writeUnsafe(double value)
{
    std::cout << "[HistoryBuffer | writeUnsafwe] warring this write does not ensure previous data is ticked by the MainLoop" << std::endl;

    if(real_index > size){//Buffer overflow
        current_index_to_write = current_index_to_write % size;
    }

    history[current_index_to_write] = value;
    current_index_to_write++;
    real_index++;

    ticked_previous = false;
    return true;
}

double* HistoryBuffer::copyHistoryArray()
{
    double *historyArray = new double[size];
    if(real_index < size){
        for(int i=0; i<size; i++){
            historyArray[i] = history[ (current_index_to_write -1 ) ];
        }
    } else {
        for(int i=0; i<size; i++){
            historyArray[i] = history[ (current_index_to_write+i) % size ];
        }
    }


    return historyArray;
}

void HistoryBuffer::print()
{
    std::cout << "[HistoryBuffer] Printing buffer " << std::endl;
    for(int i=0; i < this->size; i++){
        std::cout << "[HistoryBuffer | print]  [" << i << "] " << history[i] << std::endl;
    }
}

void HistoryBuffer::print(int index)
{
    std::cout << "[HistoryBuffer | print]  [" << index << "] " << history[index] << std::endl;
}