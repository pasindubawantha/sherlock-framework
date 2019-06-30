/**
 * This demonstration shows how use the history buffer
 * with one thread updating the buffer 
 * and another thread reading(copying after each update) the buffer.
 * 
 * */

#include <iostream>
#include <thread>
#include <mutex> 
#include <memory>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/mman.h>

#include "MainLoop.h"
#include "HistoryBuffer.h"
#include "Profiler.h"
#include "DefaultProfiler.h"
#include "MutexLock.h"


int main()
{
    std::cout << "[main] Starting" <<std::endl;
    
    // History Buffer
    HistoryBuffer *historyBuffer;
    void *historyBufferSharedMemory = mmap(NULL, sizeof(*historyBuffer), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0); // alocating shared pages
    historyBuffer = new (historyBufferSharedMemory) HistoryBuffer(); //this is the so called "placement new" initialize HistoryBuffer u in shared pages
    
    int bufferSize = 100; // set buffer size to 100 (Short memoty size)
    double *bufferHistory; 
    bufferHistory = (double*) mmap(NULL, (sizeof(*bufferHistory) * bufferSize), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0); 
    historyBuffer->setSize(bufferSize, bufferHistory); // set buffer size to 100 (Short memoty size)

    int realSize = 220; // set real input stream to have 220 doubles (for demo)

    // std::mutex *mtx; // mutual exclusion flag to be used 
    // void *mtxSharedMemory = mmap(NULL, sizeof(*mtx), PROT_READ | PROT_WRITE, 
    //                 MAP_SHARED | MAP_ANONYMOUS, -1, 0); // alocating shared pages
    // mtx = new (mtxSharedMemory) std::mutex();

    // std::unique_lock<std::mutex> *lock;
    // void *lockSharedMemory = mmap(NULL, sizeof(*lock), PROT_READ | PROT_WRITE, 
    //                 MAP_SHARED | MAP_ANONYMOUS, -1, 0); // alocating shared pages
    // lock = new (lockSharedMemory) std::unique_lock<std::mutex>(*mtx);
    // lock->unlock();

    MutexLock *mutexLock;
    void *mutexLockSharedMemory = mmap(NULL, sizeof(*mutexLock), PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0); // alocating shared pages
    mutexLock = new (mutexLockSharedMemory) MutexLock(); //this is the so called "placement new" initialize HistoryBuffer u in shared pages
    

    int pid = fork(); // creats 2 threads
    
    if(pid == 0){// Child thread
        std::cout << "[main:child] Stated" << std::endl;
        
        int readCount = 0;
        while(readCount < realSize){
            if(mutexLock->getLock()){ // this block is mutually locked
                // std::lock_guard<std::mutex> lock(*mtx);
                if(!historyBuffer->hasTickedPrevious()){
                    
                    readCount++;
                    double *myhistory = historyBuffer->copyHistoryArray(); // copy history buffer for some use
                    std::cout << "[main:child] read " << (readCount) << " times" << std::endl;
                    historyBuffer->setTickedPrevious(true); // we have processed previous element now ((last just before the lock))
                    // sleep(1); // sleep child to show thread safty
                }
                mutexLock->unlock();
            }
            
        }

        std::cout << "[main:child] Ended" << std::endl;
    } else if(pid > 0) {// Parent thread
        std::cout << "[main:parent] Stated" << std::endl;
        
        int writeCount = 0;
        while(writeCount < realSize){
            if(mutexLock->getLock()){ // this block is mutually locked
                // std::lock_guard<std::mutex> lock(*mtx);
                if(historyBuffer->hasTickedPrevious()){
                    // sleep(1); // sleep parent to show thread safty
                    writeCount++;
                    std::cout << "[main:parent] wrote " << (writeCount) << std::endl;
                    historyBuffer->writeSafe(writeCount); // wtite to history buffer (last just before the lock)
                }
                mutexLock->unlock();
            }
            
        }

        std::cout << "[main:parent] Ended" << std::endl;
    } else { // error
        std::cout << "[main] Error forking" << std::endl;
    }

    std::cout << "[main] Ending" << std::endl;
    return 0;
}