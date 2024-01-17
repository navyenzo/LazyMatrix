//-------------------------------------------------------------------
/**
 * @file robust_mutex.hpp
 * @brief Defines a robust mutex class for inter-process synchronization.
 *
 * This header file defines the RobustMutex class, which implements a mutex suitable for 
 * use across multiple processes. This mutex is robust against process crashes and can 
 * handle ownership issues effectively. It provides a cross-platform implementation, supporting
 * both Windows and POSIX (Linux) environments.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_ROBUST_MUTEX_HPP_
#define INCLUDE_ROBUST_MUTEX_HPP_



//-------------------------------------------------------------------
#include <atomic>

// Check if we are compiling on Windows
#ifdef _WIN32
    #include <windows.h>
#else
    // POSIX headers
    #include <pthread.h>
#endif
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
 * RobustMutex uses platform-specific implementations to create a mutex that is robust and 
 * suitable for use across multiple processes. It ensures that mutexes are not orphaned due
 * to abrupt termination of a process, providing consistent locking behavior.
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
     * @brief Destructor for RobustMutex.
     * 
     * Destroys the mutex if it was previously initialized, ensuring proper cleanup.
     */
    ~RobustMutex();

    /**
     * @brief Locks the mutex.
     * 
     * Attempts to lock the mutex. In Windows, it waits indefinitely for the lock.
     * In POSIX, if the mutex was previously locked by a process that has terminated, 
     * it marks the state as consistent.
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

#ifdef _WIN32
    HANDLE mtx_handle_; ///< Windows HANDLE for the mutex.
#else
    pthread_mutex_t mtx; ///< POSIX thread mutex used for synchronization.
#endif

    std::atomic<int> initialization_status_; ///< Status flag for mutex initialization.

    static const int UNINITIALIZED = 0; ///< Flag indicating mutex is not initialized.
    static const int INITIALIZED = 1;   ///< Flag indicating mutex has been initialized.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Implementation of member functions
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline RobustMutex::RobustMutex()
    : initialization_status_(UNINITIALIZED)
{
    ensure_initialized();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline RobustMutex::~RobustMutex()
{
    #ifdef _WIN32
        if (mtx_handle_ != NULL)
        {
            CloseHandle(mtx_handle_);
        }
    #else
        if (initialization_status_.load() == INITIALIZED)
        {
            pthread_mutex_destroy(&mtx);
        }
    #endif
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline int RobustMutex::lock()
{
    #ifdef _WIN32
        DWORD wait_result = WaitForSingleObject(mtx_handle_, INFINITE);
        switch (wait_result)
        {
            case WAIT_OBJECT_0:
                return 0; // Success
            default:
                return -1; // Error
        }
    #else
        int ret = pthread_mutex_lock(&mtx);
        if (ret == EOWNERDEAD)
        {
            pthread_mutex_consistent(&mtx);
            return EOWNERDEAD;
        }
        return ret;
    #endif
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void RobustMutex::unlock()
{
    #ifdef _WIN32
        ReleaseMutex(mtx_handle_);
    #else
        pthread_mutex_unlock(&mtx);
    #endif
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void RobustMutex::initialize_mutex()
{
    #ifdef _WIN32
        mtx_handle_ = CreateMutex(NULL, FALSE, NULL);
    #else
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mtx, &attr);
        pthread_mutexattr_destroy(&attr);
    #endif
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void RobustMutex::ensure_initialized()
{
    if (initialization_status_.load() == UNINITIALIZED)
    {
        int expected = UNINITIALIZED;
        if (initialization_status_.compare_exchange_strong(expected, INITIALIZED))
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
