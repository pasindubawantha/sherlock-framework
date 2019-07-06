/**
 * This demonstration shows how use the history buffer
 * with one thread updating the buffer 
 * and another thread reading(copying after each update) the buffer.
 * 
 * */

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

#include "MainLoop.h"
#include "HistoryBuffer.h"
#include "Profiler.h"
#include "DefaultProfiler.h"
// #include "MutexLock.h"
// #include "sherlock.h"

inline bool acquireLock(int *lock){

    int lockval;
    bool returnvalue;
    int res=0;
int op1=20;
int op2=30;
 
// __asm__ ( "movl $10, %eax;"
//                     "movl $20, %ebx;"
//                     "imull %ebx, %eax;"
//     );
    // asm (
    // "0: lwarx %0,0,%2  \n" //load lock and reserve
    // "   cmpwi 0,%0,0   \n" //compare the lock value to 0
    // "   bne 1f         \n" //not 0 then exit function
    // "   ori %0,%0,1    \n" //set the lock to 1
    // "   stwcx. %0,0,%2 \n" //try to acquire the lock
    // "   bne 0b         \n" //reservation lost, try again
    // "   ori  %1,%1,1   \n" //set the return value to true
    // "1:                \n" //didn't get lock, return false
    // : "+r" (lockval), "+r" (returnvalue)
    // : "r"(lock)            //parameter lock is an address
    // : "%cr0" );             //cmpwi, stwcx both clobber cr0
    __asm__(
        "movle eax, 1;"
        "xchg eax, %1;"
        "movle %0, eax;"
        :"+r" (lockval)
        :"r" (lock)
        :"%eax"
    );
   return (lockval == 0);
//    return returnvalue;
}

int main()
{
    std::cout << "[main] Starting" <<std::endl;
    
    // History Buffer
    HistoryBuffer *historyBuffer;
    void *historyBufferSharedMemory = mmap(NULL, sizeof(*historyBuffer), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0); // alocating shared pages
    historyBuffer = new (historyBufferSharedMemory) HistoryBuffer(); //this is the so called "placement new" initialize HistoryBuffer u in shared pages
    
    int bufferSize = 10; // set buffer size to 100 (Short memoty size)
    double *bufferHistory; 
    bufferHistory = (double*) mmap(NULL, (sizeof(*bufferHistory) * bufferSize), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0); 
    historyBuffer->setSize(bufferSize, bufferHistory); // set buffer size to 100 (Short memoty size)
    
    // other thread control variales
    bool *last_line_processed; 
    last_line_processed = (bool*) mmap(NULL, sizeof(*last_line_processed), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0); // alocating shared pages;
    *last_line_processed = false;

    bool *file_failed_to_open;
    file_failed_to_open = (bool*) mmap(NULL, sizeof(*file_failed_to_open), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0); // alocating shared pages;
    *file_failed_to_open = false;

    // MutexLock *mutexLock;
    // void *mutexLockSharedMemory = mmap(NULL, sizeof(*mutexLock), PROT_READ | PROT_WRITE, 
    //                 MAP_SHARED | MAP_ANONYMOUS, -1, 0); // alocating shared pages
    // mutexLock = new (mutexLockSharedMemory) MutexLock(); //this is the so called "placement new" initialize HistoryBuffer u in shared pages
    
    int *lockWord;
    lockWord = (int*) mmap(NULL, sizeof(*lockWord), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0); // alocating shared pages;
    *lockWord = 0;

    int pid = fork(); // creats 2 threads
    
    if(pid == 0){// Child thread - reader (MainLoop)
        std::cout << "[main:child] Stated" << std::endl;
        
        // MainLoop
        MainLoop mainLoop;

        // setting profiler (do not delete pointers)
        Profiler *profiler = new DefaultProfiler("def 1");
        mainLoop.setProfiler(profiler);

        // set historyBuffer
        mainLoop.setHistoryBuffer(historyBuffer);

        int readCount = 0;
        while(!(*last_line_processed || *file_failed_to_open)){

            // if(mutexLock->getLock()){ // this block is mutually locked
            if(acquireLock(lockWord)){ // this block is mutually locked
                if(!historyBuffer->hasTickedPrevious()){
                    // reading values
                    readCount++;
                    double *myhistory = historyBuffer->copyHistoryArray(); // copy history buffer for some use
                    double finalValue;
                    if(historyBuffer->getRealIndex() < historyBuffer->getSize()){
                        finalValue = myhistory[historyBuffer->getRealIndex()-1];
                    } else {
                        finalValue = myhistory[historyBuffer->getSize()-1];
                    }
                    std::cout << "[main:child] read " << (readCount) << " times  [" << historyBuffer->getRealIndex() << "][" << historyBuffer->getSize()-1 << "] {" << finalValue << "}" << std::endl;
                    
                    // MainLoop tick
                    mainLoop.tick();


                    historyBuffer->setTickedPrevious(true); // we have processed previous element now ((last just before the lock))
                }
                // mutexLock->unlock();
                *lockWord = 0;
            }
            
        }
        // historyBuffer->print();

        std::cout << "[main:child] Ended" << std::endl;
    } else if(pid > 0) {// Parent thread - writer writing from file
        std::cout << "[main:parent] Stated" << std::endl;
        
        std::fstream in_file {"../data/test/Twitter_volume_AAPL.50.txt", std::ios::in};
        // std::fstream in_file {"../data/test/Twitter_volume_AAPL.txt", std::ios::in};
        if(in_file.is_open()) {
            std::string line;
            double line_double;
            while(!in_file.eof()){
                // if(mutexLock->getLock()){ // this block is mutually locked
                if(acquireLock(lockWord)){ // this block is mutually locked
                
                    if(historyBuffer->hasTickedPrevious()){
                        std::getline(in_file,line);
                        if(line == "")
                        {   
                            *last_line_processed = true;
                        } else {
                            line_double = std::stod(line);
                            std::cout << "[main:parent] wrote " << line_double << std::endl;
                            historyBuffer->writeSafe(line_double); // wtite to history buffer (last just before the lock)
                        }
                    }
                    // mutexLock->unlock();
                    *lockWord = 0;
                }
            }
            *last_line_processed = true;
        } else {
            std::cout << "[main:parent] Failed to open file !" << std::endl;
            *file_failed_to_open = true;
        }
        in_file.close();
        std::cout << "[main:parent] Ended" << std::endl;
    } else { // error
        std::cout << "[main] Error forking" << std::endl;
    }

    // std::cout << "[main] Ending" << std::endl;
    return 0;
}