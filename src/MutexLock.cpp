#include "MutexLock.h"

#include <iostream>
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
    // if(this->locked) return false;
    return (locked) ? false : (locked = true);

}

void MutexLock::unlock()
{
    locked = false;
}
