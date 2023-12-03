//-------------------------------------------------------------------
/**
 * @file robust_mutex.hpp
 * @brief Defines a robust mutex class for inter-process synchronization in the LazyMatrix library.
 *
 * This header file defines the RobustMutex class which implements a mutex suitable for 
 * use across multiple processes. This mutex is robust against process crashes and can 
 * handle ownership issues effectively.
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_ROBUST_MUTEX_HPP_
#define INCLUDE_ROBUST_MUTEX_HPP_



//-------------------------------------------------------------------
#include <pthread.h>
#include <atomic>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class RobustMutex
 * @brief A mutex class that is robust and suitable for inter-process synchronization.
 *
 * RobustMutex uses pthread mutex attributes to create a mutex that is robust and 
 * suitable for use across multiple processes. This mutex is particularly useful in 
 * scenarios where mutexes might be orphaned due to abrupt termination of a process.
 */
//-------------------------------------------------------------------
class RobustMutex
{
public:

    /**
     * @brief Constructor for RobustMutex.
     * 
     * Initializes the mutex and sets its state to 'uninitialized'.
     * The actual initialization is deferred to ensure efficiency.
     */
    RobustMutex();

    /**
     * @brief Locks the mutex.
     * 
     * Attempts to lock the mutex. If the mutex was previously locked by a process 
     * that has terminated, it marks the state as consistent and returns EOWNERDEAD.
     * 
     * @return int Returns 0 on success or an error code on failure.
     */
    int lock();

    /**
     * @brief Unlocks the mutex.
     * 
     * Releases the mutex, allowing other processes or threads to acquire it.
     */
    void unlock();

    /**
     * @brief Destructor for RobustMutex.
     * 
     * Destroys the mutex if it was previously initialized, ensuring proper cleanup.
     */
    ~RobustMutex();

private:
    /**
     * @brief Initializes the mutex with necessary attributes.
     * 
     * Sets up the mutex to be robust and sharable across processes.
     */
    void initialize_mutex();

    /**
     * @brief Ensures the mutex is initialized only once.
     * 
     * Uses atomic operations to avoid multiple initializations in a multi-process environment.
     */
    void ensure_initialized();

    pthread_mutex_t mtx; ///< POSIX thread mutex used for synchronization.
    std::atomic<int> initialization_status; ///< Status flag for mutex initialization.

    static const int UNINITIALIZED = 0; ///< Flag indicating mutex is not initialized.
    static const int INITIALIZED = 1;   ///< Flag indicating mutex has been initialized.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Implementation of member functions
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline RobustMutex::RobustMutex()
    : initialization_status(UNINITIALIZED)
{
    ensure_initialized();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline int RobustMutex::lock()
{
    int ret = pthread_mutex_lock(&mtx);
    if (ret == EOWNERDEAD)
    {
        pthread_mutex_consistent(&mtx);
        return EOWNERDEAD;
    }
    return ret;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void RobustMutex::unlock()
{
    pthread_mutex_unlock(&mtx);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline RobustMutex::~RobustMutex()
{
    if (initialization_status.load() == INITIALIZED)
    {
        pthread_mutex_destroy(&mtx);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void RobustMutex::initialize_mutex()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);         
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);      
    pthread_mutex_init(&mtx, &attr);
    pthread_mutexattr_destroy(&attr);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void RobustMutex::ensure_initialized()
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_ROBUST_MUTEX_HPP_
