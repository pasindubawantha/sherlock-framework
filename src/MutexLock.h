#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

#include <string>
#include <iostream>

class MutexLock
{
private:
    
protected:
    bool locked = false;
public:
    // Constructors
    MutexLock();

    // Destructor
    ~MutexLock();
    
    // Copy constructor
    MutexLock(const MutexLock &source);
    
    // Move constructor
    MutexLock(MutexLock &&source);
    
    // methods
    bool getLock();
    void unlock();
};

#endif // _MutexLock_H_
