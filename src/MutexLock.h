#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

#include <string>
#include <iostream>
#include <memory>

class MutexLock
{
private:
    
protected:

public:
    // Constructors
    MutexLock(unsigned *lockWord);

    // Destructor
    ~MutexLock();
    
    // Copy constructor
    MutexLock(const MutexLock &source);
    
    // Move constructor
    MutexLock(MutexLock &&source);
    
    // methods
    bool acquireLock(unsigned *lock, std::string thread);

    void releaseLock(unsigned *lockWord, std::string thread);
};

#endif // _MUTEXLOCK_H_