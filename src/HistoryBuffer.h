/**
 * This buffer is not a usual buffer.
 * This buffer is supposed to be written(one element at a time) once all the reader has read previous element.
 * i.e. Onces the previous element has been ticked by the MainLoop.
 * 
 * */

#ifndef _HISTORYBUFFER_H_
#define _HISTORYBUFFER_H_

#include <string>
#include <iostream>

class HistoryBuffer
{
private:
    
protected:
    double *history;
    int size;
    int current_index_to_write = 0;
    int real_index = 0;
    bool ticked_previous = true;

public:
    // Constructors
    HistoryBuffer();

    // Destructor
    ~HistoryBuffer();
    
    // Copy constructor
    HistoryBuffer(const HistoryBuffer &source);
    
    // Move constructor
    HistoryBuffer(HistoryBuffer &&source);
    
    // methods
    bool hasTickedPrevious() {return this->ticked_previous;};
    void setTickedPrevious(bool ticked) {this->ticked_previous = ticked;};

    int getCuurentIndexToWirte() {return current_index_to_write;};
    int getRealIndex() {return real_index;};
    int getSize() {return size;}
    
    void setSize(int size, double buffer[]);
    bool writeSafe(double value);
    bool writeUnsafe(double value);
    double* copyHistoryArray();
    void print();
    void print(int index);
};

#endif // _HISTORYBUFFER_H_
