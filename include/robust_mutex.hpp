#ifndef INCLUDE_ROBUST_MUTEX_HPP_
#define INCLUDE_ROBUST_MUTEX_HPP_



//-------------------------------------------------------------------
#include <pthread.h>
#include <atomic>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// RobustMutex -- An inter-process mutex that can be used across
//                multiple processes and multiple threads
//-------------------------------------------------------------------
class RobustMutex
{
public:

    // Constructor initializes the atomic status flag and ensures the mutex is initialized
    RobustMutex()
        : initialization_status(UNINITIALIZED)
    {
        ensure_initialized();
    }

    // Locks the mutex. If the mutex was locked by a process that died, 
    // it returns EOWNERDEAD to let the user handle the inconsistency.
    int lock()
    {
        int ret = pthread_mutex_lock(&mtx);
        if (ret == EOWNERDEAD)
        {
            pthread_mutex_consistent(&mtx);
            return EOWNERDEAD;
        }
        return ret;
    }

    // Unlocks the mutex.
    void unlock()
    {
        pthread_mutex_unlock(&mtx);
    }

    // Destructor destroys the mutex if it was initialized.
    ~RobustMutex()
    {
        if (initialization_status.load() == INITIALIZED)
        {
            pthread_mutex_destroy(&mtx);
        }
    }



private:

    // Initializes the robust mutex attributes and the mutex itself.
    void initialize_mutex()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);         
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);      
        pthread_mutex_init(&mtx, &attr);
        pthread_mutexattr_destroy(&attr);
    }

    // Ensures the mutex is initialized. Only the first call actually 
    // initializes the mutex, subsequent calls are no-ops.
    void ensure_initialized()
    {
        if (initialization_status.load() == UNINITIALIZED)
        {
            int expected = UNINITIALIZED;
            if (initialization_status.compare_exchange_strong(expected, INITIALIZED))
            {
                initialize_mutex();
            }
        }
    }

    // The actual pthread mutex
    pthread_mutex_t mtx;

    // Atomic status flag to ensure mutex is initialized only once
    std::atomic<int> initialization_status;            

    // Status flag indicating the mutex is not initialized
    static const int UNINITIALIZED = 0;

    // Status flag indicating the mutex has been initialized
    static const int INITIALIZED = 1;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_ROBUST_MUTEX_HPP_
