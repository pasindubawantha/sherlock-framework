#include "MutexLock.h"

#include <iostream>


// Constructors 
MutexLock::MutexLock(unsigned *lockWord)
{
    std::cout << "[MutexLock] Constructing"  << std::endl;
    *lockWord = 0;

}

// Destructor
MutexLock::~MutexLock()
{
    std::cout << "[MutexLock] Destroying"  << std::endl;
}

// Copy constructor
MutexLock::MutexLock(const MutexLock &source)
{
    std::cout << "[MutexLock] Copying"  << std::endl;
}

// Move constructor
MutexLock::MutexLock(MutexLock &&source)// source is taken as an R value
{
    std::cout << "[MutexLock] Moving"  << std::endl;
}

bool MutexLock::acquireLock(unsigned *lock, std::string thread){
    unsigned lockval = 0;
    // std::cout << "[Mutex:" << thread << "] Lock aquiring" << std::endl;
    __asm__ __volatile__(
        "movl $1, %%eax;"
        "xchg %%eax, %1;"
        "movl %%eax, %0;"
        :"=r" (lockval) /* %0: Out */
        :"m" (*lock) /* %1: In */
        :"%eax"/* Trash registers, registers used by my code */
    );

    // std::cout << "[Mutex:" << thread << "] Lock aquired" << std::endl;
    return (lockval == 0);
}

void MutexLock::releaseLock(unsigned *lockWord, std::string thread){
    *lockWord = 0;
     // std::cout << "[Mutex:" << thread << "] Lock released" << std::endl;
}