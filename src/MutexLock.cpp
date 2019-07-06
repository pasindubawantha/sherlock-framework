#include "MutexLock.h"

#include <iostream>
#include <stdio.h>
#include <pthread.h>

// Constructors 
MutexLock::MutexLock()
{
    std::cout << "[MutexLock] Constructing"  << std::endl;
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

// methods
bool MutexLock::getLock()
{
 
//     this->lock;
//     bool returnvalue = false;
//     int lockval;
//     asm (
//     "0: lwarx %0,0,%2  \n" //load lock and reserve
//     "   cmpwi 0,%0,0   \n" //compare the lock value to 0
//     "   bne 1f         \n" //not 0 then exit function
//     "   ori %0,%0,1    \n" //set the lock to 1
//     "   stwcx. %0,0,%2 \n" //try to acquire the lock
//     "   bne 0b         \n" //reservation lost, try again
//     "   ori  %1,%1,1   \n" //set the return value to true
//     "1:                \n" //didn't get lock, return false
//     : "+r" (lockval), "+r" (returnvalue)
//     : "r"(this->lock)            //parameter lock is an address
//     : "cr0" );             //cmpwi, stwcx both clobber cr0
//    return returnvalue;

    __asm  // Inline assembly is written this way in c
    {
        mov eax, 1   // EAX is a 32 bit register in which we are assigning 1
        xchg eax, this->lock // Exchange eax and lock_var atomically
        mov this->lock_sate, eax // merely saving for printing purpose
    };

    if(this->lock_sate == 0){
        return true;
    }
    return false;
    // if(this->locked) return false;
    // return (locked) ? false : (locked = true);

}

void MutexLock::unlock()
{
    __asm
    {
        mov eax, 0
        xchg eax, this->lock_sate  // This could have been a single assignment lock_var = 0
        mov this->lock_sate, eax // But we are merely using xchg again to see the previous value
    }
    // locked = false;

}
