/**
 * @defgroup    bsl_sal bsl_sal.h
 * @ingroup     bsl
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * @brief       System abstraction layer, the user registers system related functions
 */

#ifndef BSL_SAL_H
#define BSL_SAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup bsl_sal
 *
 * Registrable function structure for memory allocation/release.
 */
typedef struct MemCallback {
    /**
     * @ingroup bsl_sal
     * @brief Allocate a memory block.
     *
     * @par Description:
     * Allocate a memory block.
     *
     * @param size [IN] Size of the allocated memory.
     * @retval: Not NULL, The start address of the allocated memory when memory is allocated successfully.
     * @retval  NULL, Memory allocation failure.
*/
void *(*pfMalloc)(uint32_t size);

    /**
     * @ingroup bsl_sal
     * @brief Reclaim a memory block allocated by pfMalloc.
     *
     * @par Description:
     * Reclaim a block of memory allocated by pfMalloc.
     *
     * @param addr [IN] Start address of the memory allocated by pfMalloc.
     */
    void (*pfFree)(void *addr);
} BSL_SAL_MemCallback;

/**
 * @ingroup bsl_sal
 *
 * Thread lock handle, the corresponding structure is provided by the user during registration.
 */
typedef void *BSL_SAL_ThreadLockHandle;

/**
 * @ingroup bsl_sal
 *
 * Thread handle, the corresponding structure is provided by the user during registration.
 */
typedef void *BSL_SAL_ThreadId;

/**
 * @ingroup bsl_sal
 *
 * mutex
 */
typedef void *BSL_SAL_Mutex;

/**
 * @ingroup bsl_sal
 *
 * Condition handle, the corresponding structure is provided by the user during registration.
 */
typedef void *BSL_SAL_CondVar;

/**
 * @ingroup bsl_sal
 *
 * The user registers the function structure for thread-related operations.
 */
typedef struct ThreadCallback {
    /**
     * @ingroup bsl_sal
     * @brief Create a thread lock.
     *
     * @par Description:
     * Create a thread lock.
     *
     * @param lock [IN/OUT] Lock handle
     * @retval #BSL_SUCCESS, created successfully.
     * @retval #BSL_SAL_ERR_NO_MEMORY, memory space is insufficient and thread lock space cannot be applied for.
     * @retval #BSL_SAL_ERR_UNKNOWN, thread lock initialization failed.
     * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of lock is NULL.
     */
    int32_t (*pfThreadLockNew)(BSL_SAL_ThreadLockHandle *lock);

    /**
    * @ingroup bsl_sal
    * @brief Release the thread lock.
    *
    * @par Description:
    * Release the thread lock. Ensure that the lock can be released when other threads obtain the lock.
    *
    * @param lock [IN] Lock handle
    */
    void (*pfThreadLockFree)(BSL_SAL_ThreadLockHandle lock);

    /**
     * @ingroup bsl_sal
     * @brief Lock the read operation.
     *
     * @par Description:
     * Lock the read operation.
     *
     * @param lock [IN] Lock handle
     * @retval #BSL_SUCCESS, succeeded.
     * @retval #BSL_SAL_ERR_UNKNOWN, operation failed.
     * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of lock is NULL.
     */
    int32_t (*pfThreadReadLock)(BSL_SAL_ThreadLockHandle lock);

    /**
     * @ingroup bsl_sal
     * @brief Lock the write operation.
     *
     * @par Description:
     * Lock the write operation.
     *
     * @param lock [IN] Lock handle
     * @retval #BSL_SUCCESS, succeeded.
     * @retval #BSL_SAL_ERR_UNKNOWN, operation failed.
     * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of lock is NULL.
    */
    int32_t (*pfThreadWriteLock)(BSL_SAL_ThreadLockHandle lock);

    /**
     * @ingroup bsl_sal
     * @brief Unlock
     *
     * @par Description:
     * Unlock
     *
     * @param lock [IN] Lock handle
     * @retval #BSL_SUCCESS, succeeded.
     * @retval #BSL_SAL_ERR_UNKNOWN, operation failed.
     * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of lock is NULL.
     */
    int32_t (*pfThreadUnlock)(BSL_SAL_ThreadLockHandle lock);

    /**
     * @ingroup bsl_sal
     * @brief Obtain the thread ID.
     *
     * @par Description:
     * Obtain the thread ID.
     *
     * @return Thread ID
    */
    uint64_t (*pfThreadGetId)(void);
} BSL_SAL_ThreadCallback;

/**
* @ingroup SAL
* @brief run once: Use the initialization callback.
* @attention This function should not be a cancel, otherwise the default implementation of run
* once seems to have never been called.
*/
typedef void (*BSL_SAL_ThreadInitRoutine)(void);

/**
* @ingroup bsl_sal
* @brief Run the init Func command only once.
*
* @param onceControl [IN] Record the execution status.
* @param initFunc [IN] Initialization function.
*/
typedef int32_t (*BslThreadRunOnce)(uint32_t *onceControl, BSL_SAL_ThreadInitRoutine initFunc);

/**
* @ingroup bsl_sal
* @brief Create a thread.
*
* @attention none
* @param thread [IN/OUT] Thread ID
* @param startFunc [IN] Thread function
* @param arg [IN] Thread function parameters
*/
typedef int32_t (*BslThreadCreate)(BSL_SAL_ThreadId *thread, void *(*startFunc)(void *), void *arg);

/**
* @ingroup bsl_sal
* @brief Close the thread.
*
* @attention none
* @param thread [IN] Thread ID
*/
typedef void (*BslThreadClose)(BSL_SAL_ThreadId thread);

/**
* @ingroup bsl_sal
* @brief Create a condition variable.
*
* @attention none
* @param condVar [IN] Condition variable
*/
typedef int32_t (*BslCreateCondVar)(BSL_SAL_CondVar *condVar);

/**
* @ingroup bsl_sal
* @brief The waiting time ends or the signal is obtained.
*
* @attention None
* @param condVar [IN] Condition variable
*/
typedef int32_t (*BslCondSignal)(BSL_SAL_CondVar condVar);

/**
* @ingroup bsl_sal
* @brief The waiting time ends or the signal is obtained.
* @attention None
* @param condMutex [IN] Mutex
* @param condVar [IN] Condition variable
* @param timeout [IN] Time
*/
typedef int32_t (*BslCondTimedwaitMs)(BSL_SAL_Mutex condMutex, BSL_SAL_CondVar condVar, int32_t timeout);

/**
* @ingroup bsl_sal
* @brief Delete a condition variable.
*
* @attention none
* @param condVar [IN] Condition variable
*/
typedef int32_t (*BslDeleteCondVar)(BSL_SAL_CondVar condVar);

/**
 * @ingroup bsl_sal
 * @brief   Interface for registering memory-related callback functions
 *
 * @par Description:
 * Register the memory application and release functions.
 *
 * @attention None
 * @param cb [IN] Pointer to the memory-related callback function
 * @retval #BSL_SUCCESS, memory application and release functions are successfully registered.
 * @retval #BSL_SAL_ERR_BAD_PARAM 0x02010003. If the cb is null or the cb members have null, caution fill
 * the input parameter cb.
 */
int32_t BSL_SAL_RegMemCallback(BSL_SAL_MemCallback *cb);

/**
 * @ingroup bsl_sal
 * @brief   Interface for registering thread-related callback functions.
 *
 * @par Description:
 * Register the functions related to thread lock creation, release, lock, unlock, and thread ID obtaining.
 * Can't be called in single thread scenario.
 * Must be called in multiple threads scenario.
 *
 * @attention None
 * @param cb [IN] Thread related callback function pointer
 * @retval #BSL_SUCCESS, The functions related to the thread are successfully registered.
 * @retval #BSL_SAL_ERR_BAD_PARAM 0x02010003. If the cb is null or the cb members have null, fill in the
 *                                            cb pointer with caution.
 */
int32_t BSL_SAL_RegThreadCallback(BSL_SAL_ThreadCallback *cb);

/**
 * @ingroup SAL
 * @brief Allocate memory space.
 *
 * @par Description:
 * Allocate memory space.
 *
 * @attention None
 * @param size [IN] Size of the allocated memory
 * @return If the application is successful, returned the pointer pointing to the memory.
 *         If the application failed, return NULL.
 */
void *BSL_SAL_Malloc(uint32_t size);

/**
 * @ingroup SAL
 * @brief Allocate and clear the memory space.
 *
 * @par Description:
 * Allocate and clear the memory space. The maximum size of UINT32_MAX is allocated.
 *
 * @attention num*size should not have overflow wrap.
 * @param num [IN] Number of allocated memory.
 * @param size [IN] Size of each memory.
 * @return If the application is successful, returned the pointer pointing to the memory.
 *         If the application failed, return NULL.
 */
void *BSL_SAL_Calloc(uint32_t num, uint32_t size);

/**
 * @ingroup SAL
 * @brief   Duplicate the memory space.
 * @param   src Source memory address
 * @param   size Total memory size
 * @return  If the allocation is successful, returned the pointer pointing to the memory.
 *          If the allocation failed, return NULL.
 */
void *BSL_SAL_Dump(const void *src, uint32_t size);

/**
 * @ingroup SAL
 * @brief Release the specified memory.
 *
 * @par Description:
 * Release the specified memory.
 *
 * @attention NONE.
 * @param value [IN] Pointer to the memory space to be released.
 */
void BSL_SAL_Free(void *value);

/**
 * @ingroup SAL
 * @brief Memory expansion
 * @par Description: Memory expansion function.
 * @attention None.
 * @param addr    [IN] Original memory address.
 * @param newSize [IN] Extended memory size.
 * @param oldSize [IN] Memory size before expansion.
 * @retval void*   indicates successful, the extended memory address is returned.
 * @retval NULL    indicates failed, NULL is returned.
 */
void *BSL_SAL_Realloc(void *addr, uint32_t newSize, uint32_t oldSize);

/**
 * @ingroup SAL
 * @brief Set sensitive information to zero.
 *
 * @param ptr [IN] Memory to be zeroed
 * @param size [IN] Length of the memory to be zeroed out
 *
 * @return None
 */
void BSL_SAL_CleanseData(void *ptr, uint32_t size);

/**
 * @ingroup SAL
 * @brief Clear sensitive information and release memory.
 *
 * @param ptr [IN] Pointer to the memory to be released
 * @param size [IN] Length of the memory to be zeroed out
 *
 * @return None
 */
void BSL_SAL_ClearFree(void *ptr, uint32_t size);

#define BSL_SAL_FREE(value_)                        \
    do {                                        \
        if ((value_) != NULL) {                 \
            BSL_SAL_Free((void *)(value_));         \
            (value_) = NULL;                    \
        }                                       \
    } while (0)

#define BSL_SAL_ONCE_INIT 0 // equal to PTHREAD_ONCE_INIT, the pthread symbol is masked.
/**
 * @ingroup SAL
 * @brief Create a thread lock.
 *
 * @par Description:
 * Create a thread lock.
 *
 * @attention none
 * @param lock [IN/OUT] Lock handle
 * @retval #BSL_SUCCESS, created successfully.
 * @retval #BSL_SAL_ERR_NO_MEMORY, memory space is insufficient and failed to apply for process lock space.
 * @retval #BSL_SAL_ERR_UNKNOWN, thread lock initialization failed.
 * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error, the value of lock is NULL.
 */
int32_t BSL_SAL_ThreadLockNew(BSL_SAL_ThreadLockHandle *lock);

/**
 * @ingroup SAL
 * @brief Lock the read operation.
 *
 * @par Description:
 * Lock the read operation.
 *
 * @attention none
 * @param lock [IN] Lock handle
 * @retval #BSL_SUCCESS, succeeded.
 * @retval #BSL_SAL_ERR_UNKNOWN, operation failed.
 * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of lock is NULL.
 */
int32_t BSL_SAL_ThreadReadLock(BSL_SAL_ThreadLockHandle lock);

/**
 * @ingroup SAL
 * @brief Lock the write operation.
 *
 * @par Description:
 * Lock the write operation.
 *
 * @attention none
 * @param lock [IN] Lock handle
 * @retval #BSL_SUCCESS, succeeded.
 * @retval #BSL_SAL_ERR_UNKNOWN, operation failed.
 * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of lock is NULL.
 */
int32_t BSL_SAL_ThreadWriteLock(BSL_SAL_ThreadLockHandle lock);

/**
 * @ingroup SAL
 * @brief Unlock
 *
 * @par Description:
 * Unlock
 *
 * @attention unlock: Locks that have been unlocked are undefined behavior and are not allowed by default.
 * @param lock [IN] Lock handle
 * @retval #BSL_SUCCESS, succeeded.
 * @retval #BSL_SAL_ERR_UNKNOWN operation failed.
 * @retval #BSL_SAL_ERR_BAD_PARAM parameter error. The value of lock is NULL.
 */
int32_t BSL_SAL_ThreadUnlock(BSL_SAL_ThreadLockHandle lock);

/**
 * @ingroup SAL
 * @brief Release the thread lock.
 *
 * @par Description:
 * Release the thread lock.
 *
 * @attention By default, repeated release is prohibited.
 * @param lock [IN] Lock handle.
 */
void BSL_SAL_ThreadLockFree(BSL_SAL_ThreadLockHandle lock);

/**
 * @ingroup SAL
 * @brief Obtain the thread ID.
 *
 * @par Description:
 * Obtain the thread ID.
 *
 * @attention none
 * @retval Thread ID
 */
uint64_t BSL_SAL_ThreadGetId(void);

/**
 * @ingroup SAL
 * @brief Execute only once.
 *
 * @par Description:
 * Run the init Func command only once.
 *
 * @param onceControl [IN] Record the execution status.
 * @param initFunc [IN] Initialization function.
 * @attention The current version does not support registration.
 * @retval #BSL_SUCCESS, succeeded.
 * @retval #BSL_SAL_ERR_BAD_PARAM, input parameter is abnormal.
 * @retval #BSL_SAL_ERR_UNKNOWN, the default run once failed.
 */
int32_t BSL_SAL_ThreadRunOnce(uint32_t *onceControl, BSL_SAL_ThreadInitRoutine initFunc);

/**
 * @ingroup SAL
 * @brief Create a thread.
 *
 * @par Description:
 * Create a thread.
 *
 * @attention none
 * @param thread [IN/OUT] Thread ID
 * @param startFunc [IN] Thread function
 * @param arg [IN] Thread function parameters
 * @retval #BSL_SUCCESS, created successfully.
 * @retval #BSL_SAL_ERR_UNKNOWN, Failed to create a thread.
 * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error.
 */
int32_t BSL_SAL_ThreadCreate(BSL_SAL_ThreadId *thread, void *(*startFunc)(void *), void *arg);

/**
 * @ingroup SAL
 * @brief Close the thread.
 *
 * @par Description:
 * Close the thread.
 *
 * @attention none
 * @param thread [IN] Thread ID
 */
void BSL_SAL_ThreadClose(BSL_SAL_ThreadId thread);

/**
 * @ingroup SAL
 * @brief Create a condition variable.
 *
 * @par Description:
 * Create a condition variable.
 *
 * @attention none
 * @param condVar [IN] Condition variable
 * @retval #BSL_SUCCESS, created successfully.
 * @retval #BSL_SAL_ERR_UNKNOWN, failed to create a condition variable.
 * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of condVar is NULL.
 */
int32_t BSL_SAL_CreateCondVar(BSL_SAL_CondVar *condVar);

/**
 * @ingroup SAL
 * @brief The waiting time ends or the signal is obtained.
 *
 * @par Description:
 * The waiting time ends or the signal is obtained.
 *
 * @attention None
 * @param condVar [IN] Condition variable
 * @retval #BSL_SUCCESS, succeeded.
 * @retval #BSL_SAL_ERR_UNKNOWN, function failure
 * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of condVar is NULL.
 */
int32_t BSL_SAL_CondSignal(BSL_SAL_CondVar condVar);

/**
 * @ingroup SAL
 * @brief The waiting time ends or the signal is obtained.
 *
 * @par Description:
 * The waiting time ends or the signal is obtained.
 *
 * @attention None
 * @param condMutex [IN] Mutex
 * @param condVar [IN] Condition variable
 * @param timeout [IN] Time
 * @retval #BSL_SUCCESS, succeeded.
 * @retval #BSL_SAL_ERR_UNKNOWN, fails.
 * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of condMutex or condVar is null.
 */
int32_t BSL_SAL_CondTimedwaitMs(BSL_SAL_Mutex condMutex, BSL_SAL_CondVar condVar, int32_t timeout);

/**
 * @ingroup SAL
 * @brief Delete a condition variable.
 *
 * @par Description:
 * Delete a condition variable.
 *
 * @attention none
 * @param condVar [IN] Condition variable
 * @retval #BSL_SUCCESS, Succeeded in deleting the condition variable.
 * @retval #BSL_SAL_ERR_UNKNOWN, Failed to delete the condition variable.
 * @retval #BSL_SAL_ERR_BAD_PARAM, parameter error. The value of condVar is NULL.
 */
int32_t BSL_SAL_DeleteCondVar(BSL_SAL_CondVar condVar);

typedef void *bsl_sal_file_handle; // Pointer to file handle

/**
 * @ingroup bsl_sal_file
 * @brief Open a file.
 *
 * @par Description:
 * Open the file and ensure that the entered path is standardized.
 *
 * @attention None
 * @param stream [OUT] File handle
 * @param path [IN] File path
 * @param mode [IN] Reading mode
 * @retval #BSL_SUCCESS, succeeded.
 * @retval #BSL_SAL_ERR_FILE_OPEN, failed to be opened.
 * @retval #BSL_NULL_INPUT, parameter error.
 */
int32_t BSL_SAL_FileOpen(bsl_sal_file_handle *stream, const char *path, const char *mode);

/**
 * @ingroup bsl_sal_file
 * @brief Close the file.
 *
 * @par Description:
 * Close the file.
 *
 * @attention none
 * @param stream [IN] File handle
 * @retval NA
 */
void BSL_SAL_FileClose(bsl_sal_file_handle stream);

/**
 * @ingroup bsl_sal_file
 * @brief   Read the file.
 *
 * @par Description:
 * Read the file.
 *
 * @attention none
 * @param stream [IN] File handle
 * @param buffer [IN] Buffer for reading data
 * @param size [IN] The unit of reading.
 * @param num [IN] Number of data records to be read
 * @param len [OUT] Read the data length.
 * @retval #BSL_SUCCESS, succeeded.
 * @retval #BSL_SAL_ERR_UNKNOWN, fails.
 * @retval #BSL_NULL_INPUT, Incorrect parameter
 */
int32_t BSL_SAL_FileRead(bsl_sal_file_handle stream, void *buffer, size_t size, size_t num, size_t *len);

/**
 * @ingroup bsl_sal_file
 * @brief Write a file
 *
 * @par Description:
 * Write File
 *
 * @attention none
 * @param stream [IN] File handle
 * @param buffer [IN] Data to be written
 * @param size [IN] Write the unit
 * @param num [IN] Number of written data
 * @retval #BSL_SUCCESS, succeeded
 * @retval #BSL_SAL_ERR_UNKNOWN, fails
 * @retval #BSL_NULL_INPUT, parameter error
 */
int32_t BSL_SAL_FileWrite(bsl_sal_file_handle stream, const void *buffer, size_t size, size_t num);

/**
 * @ingroup bsl_sal_file
 * @brief Obtain the file length.
 *
 * @par Description:
 * Obtain the file length.
 *
 * @attention none
 * @param path [IN] File path
 * @param len [OUT] File length
 * @retval #BSL_SUCCESS, succeeded
 * @retval #BSL_SAL_ERR_UNKNOWN, fails
 * @retval #BSL_NULL_INPUT, parameter error
 */
int32_t BSL_SAL_FileLength(const char *path, size_t *len);

/**
 * @ingroup bsl_sal_time
 * @brief Basic time data structure definition.
 */
typedef struct {
    uint16_t year;      /**< Year. the value range is [0, 65535]. */
    uint8_t  month;     /**< Month. the value range is [1, 12]. */
    uint8_t  day;       /**< Day, the value range is [1, 31]. */
    uint8_t  hour;      /**< Hour, the value range is [0, 23]. */
    uint8_t  minute;    /**< Minute, the value range is [0, 59]. */
    uint16_t millSec;   /**< Millisecond, the value range is [0, 999]. */
    uint8_t  second;    /**< Second, the value range is [0, 59]. */
    uint16_t microSec;  /**< Microseconds, the value range is [0, 999]. */
} BSL_TIME;

/**
 * @ingroup bsl_sal_time
 * @brief Unix Time structure definition.
 */
typedef int64_t BslUnixTime;

/**
 * @ingroup bsl_sal_time
 * @brief Prototype of the callback function for obtaining the time
 * @par Description:
 * Prototype definition of the callback function for obtaining the time.
 * @attention None.
 * @par Dependency: as follows
 * @li bsl: Development package to which the interface belongs.
 * @li bsl_sal.h: Header file where the API declaration is located.
 * @since AutoTBP V100R021C00
 * @see none.
 */
typedef BslUnixTime (*BslTimeFunc)(void);

/**
 * @ingroup bsl_sal_time
 * @brief Interface for registering the function for obtaining the system time
 * @par Description:
 * You can use this API to register the system time obtaining function.
 * @attention
 * This interface can be registered for multiple times. After the registration is
 * successful, the registration cannot be NULL again.
 * Description of the time range:
 * Users can use the Linux system at most 2038 per year.
 * The lower limit of the time is 1970 - 1 - 1 0: 0: 0.
 * It is recommended that users use this minimum intersection, i.e., the bounds of
 * years are 1970-1-1 0:0:0 ~ 2038-01-19 03:14:08.
 * @param func [IN] Register the function for obtaining the system time
 * @retval void None
 * @par Dependency: as follows
 * @li bsl: development package to which the interface belongs.
 * @li bsl_sal.h: header file where the interface declaration is located.
 * @since AutoTBP V100R021C10
 * @see none
 */
void BSL_SAL_SysTimeFuncReg(BslTimeFunc func);

/**
 * @brief   Compare Two Dates
 * @param   dateA [IN] The first date
 * @param   dateB [IN] The second date
 * @param   diffSeconds [OUT] Number of seconds between two dates
 * @return  BslTimeCmpResult Comparison result of two dates
 *          BSL_TIME_CMP_ERROR - Error in comparison
 *          BSL_TIME_CMP_EQUAL - The two dates are consistent.
 *          BSL_TIME_DATE_BEFORE - The first date is before the second date.
 *          BSL_TIME_DATE_AFTER - The first date is after the second
 */
int32_t BSL_SAL_DateTimeCompare(const BSL_TIME *dateA, const BSL_TIME *dateB, int64_t *diffSec);

/**
 * @ingroup bsl_sal_time
 * @brief Obtain the system time.
 *
 * @par Description:
 * Obtain the system time.
 *
 * @attention none
 * @param sysTime [out] Time
 * @retval #BSL_SUCCESS, obtained the time successfully.
 * @retval #BSL_SAL_TIME_BAD_PARAM, the value of cb is null.
 * @retval #BSL_INTERNAL_EXCEPTION, an exception occurred when obtaining the time.
 */
int32_t BSL_SAL_SysTimeGet(BSL_TIME *sysTime);

/**
 * @ingroup bsl_sal_time
 * @brief Obtain the Unix time.
 *
 * @par Description:
 * Obtain the Unix time.
 *
 * @attention None
 * @param
 * @retval Return the Unix time.
 */
BslUnixTime BSL_SAL_CurrentSysTimeGet(void);

/**
 * @ingroup bsl_sal_time
 * @brief Convert the date in the BslSysTime format to the UTC time format.
 *
 * @par Description:
 * Convert the date in the BslSysTime format to the UTC time format.
 *
 * @attention None
 * @param dateTime [IN] Date and time
 * @param utcTime [OUT] UTC time
 * @retval #BSL_SUCCESS, time is successfully converted.
 * @retval #BSL_INTERNAL_EXCEPTION, an exception occurred when obtaining the time.
 */
int32_t BSL_SAL_DateToUtcTimeConvert(const BSL_TIME *dateTime, int64_t *utcTime);

/**
 * @ingroup bsl_sal_time
 * @brief Convert the date in the BslUnixTime format to the BslSysTime format.
 *
 * @par Description:
 * Convert the date in the BslUnixTime format to the BslSysTime format.
 *
 * @attention none
 * @param utcTime [IN] UTC time
 * @param sysTime [OUT] BslSysTime Time
 * @retval #BSL_SUCCESS, time is converted successfully
 * @retval #BSL_SAL_TIME_BAD_PARAM, the value of utcTime exceeds the upper limit or the value of sysTime is null.
 */
int32_t BSL_SAL_UtcTimeToDateConvert(int64_t utcTime, BSL_TIME *sysTime);

/**
 * @ingroup bsl_sal_time
 * @brief Compare two dates, accurate to microseconds.
 *
 * @par Description:
 * Compare two dates, accurate to microseconds
 *
 * @attention None
 * @param dateA [IN] Time
 * @param dateB [IN] Time
 * @retval BslTimeCmpResult Comparison result of two dates
 *         BSL_TIME_CMP_ERROR - An error occurred in the comparison.
 *         BSL_TIME_CMP_EQUAL - The two dates are consistent.
 *         BSL_TIME_DATE_BEFORE - The first date is on the second
 *         BSL_TIME_DATE_ AFTER - The first date is after the second
 */
int32_t BSL_SAL_DateTimeCompareByUs(const BSL_TIME *dateA, const BSL_TIME *dateB);

/**
 * @ingroup bsl_sal_time
 * @brief   Sleep the current thread
 *
 * @par Description:
 * Sleep the current thread
 *
 * @attention none
 * @param time [IN] Sleep time
 */
void BSL_SAL_Sleep(uint32_t time);

/**
 * @ingroup bsl_sal_time
 * @brief   Obtain the number of ticks that the system has experienced since startup.
 *
 * @par Description:
 * Obtain the system time.
*
 * @attention none
 * @param
 *
 * @retval Number of ticks
 */
long BSL_SAL_Tick(void);

/**
 * @ingroup bsl_sal_time
 * @brief   Obtain the number of system ticks per second.
 *
 * @par Description:
 * Obtain the system time.
 *
 * @attention none
 * @param
 * @retval Number of ticks per second
 */
long BSL_SAL_TicksPerSec(void);

/**
 * @ingroup BSL_SAL_SockAddr
 *
 * socket address
 */
typedef void *BSL_SAL_SockAddr;

/**
 * @ingroup bsl_sal_net
 * @brief   Socket creation interface
 *
 * @par Description:
 * Socket creation interface.
 *
 * @attention none
 * @param af [IN] Socket specifies the protocol set.
 * @param type [IN] Socket type
 * @param protocol [IN] Protocol type
 * @retval If the creation is successful, a non-negative value is returned.
 * Otherwise, a negative value is returned.
 */
int32_t BSL_SAL_Socket(int32_t af, int32_t type, int32_t protocol);

/**
 * @ingroup bsl_sal_net
 * @brief Close the socket
 *
 * @par Description:
 * Close the socket
 *
 * @attention none
 * @param sockId [IN] Socket file descriptor ID
 * @retval If the operation succeeds, BSL_SUCCESS is returned.
 *         If the operation fails, BSL_SAL_ERR_NET_SOCKCLOSE is returned.
 */
int32_t BSL_SAL_SockClose(int32_t sockId);

/**
 * @ingroup bsl_sal_net
 * @brief   Set the socket
 *
 * @par Description:
 * Set the socket
 *
 * @attention none
 * @param sockId [IN] Socket file descriptor ID
 * @param level [IN] Level of the option to be set.
 * @param name [IN] Options to be set
 * @param val [IN] Value of the option.
 * @param len [IN] val Length
 * @retval If the operation succeeds, BSL_SUCCESS is returned
 *         If the operation fails, BSL_SAL_ERR_NET_SETSOCKOPT is returned.
 */
int32_t BSL_SAL_SetSockopt(int32_t sockId, int32_t level, int32_t name, const void *val, int32_t len);

/**
 * @ingroup bsl_sal_net
 * @brief   Get the socket
 *
 * @par Description:
 * Get the socket
 *
 * @attention none
 * @param sockId [IN] Socket file descriptor ID
 * @param level [IN] Level of the option to be get.
 * @param name [IN] Options to be get
 * @param val [IN] Value of the option.
 * @param len [IN] val Length
 * @retval If the operation succeeds, BSL_SUCCESS is returned
 *         If the operation fails, BSL_SAL_ERR_NET_SETSOCKOPT is returned.
 */
int32_t BSL_SAL_GetSockopt(int32_t sockId, int32_t level, int32_t name, void *val, int32_t *len);

/**
 * @ingroup bsl_sal_net
 * @brief Listening socket
 *
 * @par Description:
 * Listen socket
 *
 * @attention none
 * @param sockId [IN] Socket file descriptor ID
 * @param backlog [IN] Length of the receiving queue
 * @retval If the operation succeeds, BSL_SUCCESS is returned.
 * If the operation fails, BSL_SAL_ERR_NET_LISTEN is returned.
 */
int32_t BSL_SAL_SockListen(int32_t sockId, int32_t backlog);

/**
 * @ingroup bsl_sal_net
 * @brief Binding a socket
 *
 * @par Description:
 * Binding Socket
 *
 * @attention None
 * @param sockId [IN] Socket file descriptor ID
 * @param addr [IN] Specify the address.
 * @param len [IN] Address length
 * @retval If the operation succeeds, BSL_SUCCESS is returned.
 * If the operation fails, BSL_SAL_ERR_NET_BIND is returned.
 */
int32_t BSL_SAL_SockBind(int32_t sockId, BSL_SAL_SockAddr addr, size_t len);

/**
 * @ingroup bsl_sal_net
 * @brief Initiate a connection.
 *
 * @par Description:
 * Initiate a connection.
 *
 * @attention none
 * @param sockId [IN] Socket file descriptor ID
 * @param addr [IN] Address to be connected
 * @param len [IN] Address length
 * @retval If the operation succeeds, BSL_SUCCESS is returned
 *         If the operation fails, BSL_SAL_ERR_NET_CONNECT is returned.
 */
int32_t BSL_SAL_SockConnect(int32_t sockId, BSL_SAL_SockAddr addr, size_t len);

/**
 * @ingroup bsl_sal_net
 * @brief   Send a message.
 *
 * @par Description:
 * Send messages
 *
 * @attention none
 * @param sockId [IN] Socket file descriptor ID
 * @param msg [IN] Message sent
 * @param len [IN] Information length
 * @param flags [IN] is generally set to 0.
 * @retval If the operation succeeds, the length of the sent data is returned.
 * If the operation fails, a negative value is returned.
 * If the operation times out or the peer end disables the function, the value 0 is returned.
 */
int32_t BSL_SAL_SockSend(int32_t sockId, const void *msg, size_t len, int32_t flags);

/**
 * @ingroup bsl_sal_net
 * @brief Receive the message.
 *
 * @par Description:
 * Receive information
 *
 * @attention none
 * @param sockfd [IN] Socket file descriptor ID
 * @param buff [IN] Buffer for receiving information
 * @param len [IN] Length of the buffer
 * @param flags [IN] is generally set to 0.
 * @retval If the operation succeeds, the received data length is returned.
 * If the operation fails, a negative value is returned.
 * If the operation times out or the peer end disables the function, the value 0 is returned.
 */
int32_t BSL_SAL_SockRecv(int32_t sockfd, void *buff, size_t len, int32_t flags);

typedef enum {
    BSL_SAL_MEM_MALLOC = 0x0100,                        /* pfMalloc */
    BSL_SAL_MEM_FREE,                                   /* pfFree */

    BSL_SAL_THREAD_LOCK_NEW_CB_FUNC = 0x0200,           /* pfThreadLockNew */
    BSL_SAL_THREAD_LOCK_FREE_CB_FUNC,                   /* pfThreadLockFree */
    BSL_SAL_THREAD_LOCK_READ_LOCK_CB_FUNC,              /* pfThreadReadLock */
    BSL_SAL_THREAD_LOCK_WRITE_LOCK_CB_FUNC,             /* pfThreadWriteLock */
    BSL_SAL_THREAD_LOCK_UNLOCK_CB_FUNC,                 /* pfThreadUnlock */
    BSL_SAL_THREAD_GET_ID_CB_FUNC,                      /* pfThreadGetId */
    BSL_SAL_THREAD_RUN_ONCE_CB_FUNC,                    /* BslThreadRunOnce */
    BSL_SAL_THREAD_CREATE_CB_FUNC,                      /* BslThreadCreate */
    BSL_SAL_THREAD_CLOSE_CB_FUNC,                       /* BslThreadClose */
    BSL_SAL_THREAD_CONDVAR_CREATE_LOCK_CB_FUNC,         /* BslCreateCondVar */
    BSL_SAL_THREAD_CONDVAR_SIGNAL_CB_FUNC,              /* BslCondSignal */
    BSL_SAL_THREAD_CONDVAR_WAIT_CB_FUNC,                /* BslCondTimedwaitMs */
    BSL_SAL_THREAD_CONDVAR_DELETE_CB_FUNC,              /* BslDeleteCondVar */

    BSL_SAL_NET_WRITE_CB_FUNC = 0x0300,                 /* BslSalNetWrite */
    BSL_SAL_NET_READ_CB_FUNC,                           /* BslSalNetRead */
    BSL_SAL_NET_LSEEK_CB_FUNC,                          /* BslSalNetLSeek */
    BSL_SAL_NET_OPEN_CB_FUNC,                           /* BslSalNetOpen */
    BSL_SAL_NET_CLOSE_CB_FUNC,                          /* BslSalNetClose */
    BSL_SAL_NET_SOCK_CB_FUNC,                           /* BslSalSocket */
    BSL_SAL_NET_SOCK_CLOSE_CB_FUNC,                     /* BslSalSockClose */
    BSL_SAL_NET_SET_SOCK_OPT_CB_FUNC,                   /* BslSalSetSockopt */
    BSL_SAL_NET_GET_SOCK_OPT_CB_FUNC,                   /* BslSalGetSockopt */
    BSL_SAL_NET_SOCK_LISTEN_CB_FUNC,                    /* BslSalSockListen */
    BSL_SAL_NET_SOCK_BIND_CB_FUNC,                      /* BslSalSockBind  */
    BSL_SAL_NET_SOCK_CONNECT_CB_FUNC,                   /* BslSalSockConnect */
    BSL_SAL_NET_SOCK_SEND_CB_FUNC,                      /* BslSalSockSend */
    BSL_SAL_NET_SOCK_RECV_CB_FUNC,                      /* BslSalSockRecv */
    BSL_SAL_NET_SELECT_CB_FUNC,                         /* BslSelect */
    BSL_SAL_NET_IOCTL_CB_FUNC,                          /* BslIoctlSocket */
    BSL_SAL_NET_GET_ERRNO_CB_FUNC,                      /* BslGetErrno */
    BSL_SAL_NET_SENDTO_CB_FUNC,                         /* BslSalNetSendTo */
    BSL_SAL_NET_RECVFROM_CB_FUNC,                       /* BslSalNetRecvFrom */
    BSL_SAL_NET_SET_BLOCKMODE_CB_FUNC,                  /* BslSetBlockMode */
    BSL_SAL_NET_SOCKET_WAITE_CB_FUNC,                   /* pfSocketWait */
    BSL_SAL_NET_SOCKET_ERROR_CB_FUNC,                   /* pfSocketError */

    BSL_SAL_TIME_GET_UTC_TIME_CB_FUNC = 0X0400,         /* BslGetUtcTime */
    BSL_SAL_TIME_GET_BSL_TIME_CB_FUNC,                  /* BslGetBslTime */
    BSL_SAL_TIME_UTC_TO_BSL_TIME_CB_FUNC,               /* BslUtcTimeToBslTime */
    BSL_SAL_TIME_SLEEP_CB_FUNC,                         /* BslSleep */
    BSL_SAL_TIME_TICK_CB_FUNC,                          /* BslTick */
    BSL_SAL_TIME_TICK_PER_SEC_CB_FUNC,                  /* BslTicksPerSec */

    BSL_SAL_FILE_OPEN_CB_FUNC = 0X0500,                 /* BslSalFileOpen */
    BSL_SAL_FILE_READ_CB_FUNC,                          /* BslSalFileRead */
    BSL_SAL_FILE_WRITE_CB_FUNC,                         /* BslSalFileWrite */
    BSL_SAL_FILE_CLOSE_CB_FUNC,                         /* BslSalFileClose */
    BSL_SAL_FILE_LENGTH_CB_FUNC,                        /* BslSalFileLength */
    BSL_SAL_FILE_ERROR_CB_FUNC,                         /* BslSalFileError */
    BSL_SAL_FILE_TELL_CB_FUNC,                          /* BslSalFileTell */
    BSL_SAL_FILE_SEEK_CB_FUNC,                          /* BslSalFileSeek */
    BSL_SAL_FILE_GETS_CB_FUNC,                          /* BslSalFGets */
    BSL_SAL_FILE_PUTS_CB_FUNC,                          /* BslSalFPuts */
    BSL_SAL_FILE_FLUSH_CB_FUNC,                         /* BslSalFlush */
    BSL_SAL_FILE_EOF_CB_FUNC,                           /* BslSalFeof */
    BSL_SAL_FILE_SET_ATTR_FUNC,                         /* BslSalFSetAttr */
    BSL_SAL_FILE_GET_ATTR_FUNC,                         /* BslSalFGetAttr */

    BSL_SAL_CONTEXT_MAKE_CB_FUNC = 0x0600,              /* BslMakeContext */
    BSL_SAL_CONTEXT_SWAP_CB_FUNC,                       /* BslSalFGetAttr */
    BSL_SAL_CONTEXT_GET_CB_FUNC,                        /* BslSalFGetAttr */

    BSL_SAL_MAX_FUNC_CB = 0xffff
} BSL_SAL_CB_FUNC_TYPE;

/**
 * @ingroup bsl_sal
 *
 * Definition of the net callback interface.
 */

/**
 * @ingroup bsl_sal
 * @brief Write data to file descriptor.
 *
 * @par Description: Attempt to write len bytes from the buffer to the file associated with the open file descriptor.
 * @param fd [IN] File descriptor.
 * @param buffer [IN] The write data buffer.
 * @param len [IN] The len which want to write.
 * @param err [OUT] The IO errno.
 * @return Return the number of bytes actually written to the file.
 * Otherwise, -1 shall be returned and errno set to indicate the error.
 */
typedef int32_t (*BslSalNetWrite)(int32_t fd, const void *buf, uint32_t len, int32_t *err);

/**
 * @ingroup bsl_sal
 * @brief Read data from file descriptor.
 *
 * @par Description: Attempt to read len bytes from the the file associated with the open file descriptor to the buffer.
 * @param fd [IN] File descriptor.
 * @param buffer [OUT] The read data buffer.
 * @param len [IN] The len which want to read.
 * @param err [OUT] The IO errno.
 * @return Return the number of bytes actually read from the file.
 * Otherwise, -1 shall be returned and errno set to indicate the error.
 */
typedef int32_t (*BslSalNetRead)(int32_t fd, void *buf, uint32_t len, int32_t *err);

/**
 * @ingroup bsl_sal
 * @brief Seek interface.
 *
 * @par Description: Offsets the file read/write position to a certain position.
 * @param fd [IN] File descriptor.
 * @param offset [IN] The offset from the start position.
 * @param origin [IN] The start position. One of SEEK_SET, SEEK_CUR and SEEK_END.
 * @return If succeed, return the new read/write position. Otherwise, return -1.
 */
typedef int64_t (*BslSalNetLSeek)(int32_t fd, int64_t offset, uint32_t origin);

/**
 * @ingroup bsl_sal
 * @brief Close file descriptor.
 *
 * @par Description: Close file descriptor.
 * @param path [IN] File path.
 * @param flag [IN] open mode.
 * @return Error code in bsl_errno.h.
 */
typedef int32_t (*BslSalNetOpen)(const char *path, int32_t flag);

/**
 * @ingroup bsl_sal
 * @brief Close file descriptor.
 *
 * @par Description: Close file descriptor.
 * @param fd [IN] File descriptor.
 * @return Error code in bsl_errno.h.
 */
typedef int32_t (*BslSalNetClose)(int32_t fd);

/**
 * @ingroup bsl_sal
 * @brief Socket creation interface.
 *
 * @return If the creation is successful, a non-negative value is returned.
 * Otherwise, a negative value is returned.
 */
typedef int32_t (*BslSalSocket)(int32_t af, int32_t type, int32_t protocol);

/**
 * @ingroup bsl_sal
 * @brief Socket close interface.
 *
 * @return  If the operation succeeds, BSL_SUCCESS is returned.
 *          If the operation fails, BSL_SAL_ERR_NET_SOCKCLOSE is returned.
 */
typedef int32_t (*BslSalSockClose)(int32_t sockId);

/**
 * @ingroup bsl_sal
 * @brief Set the socket.
 *
 * @return If the operation succeeds, BSL_SUCCESS is returned
 *         If the operation fails, BSL_SAL_ERR_NET_SETSOCKOPT is returned.
 */
typedef int32_t (*BslSalSetSockopt)(int32_t sockId, int32_t level, int32_t name, const void *val, int32_t len);

/**
 * @ingroup bsl_sal
 * @brief Get socket.
 *
 * @return If the operation succeeds, BSL_SUCCESS is returned.
 *         If the operation fails, BSL_SAL_ERR_NET_GETSOCKOPT is returned.
 */
typedef int32_t (*BslSalGetSockopt)(int32_t sockId, int32_t level, int32_t name, void *val, int32_t *len);

/**
 * @ingroup bsl_sal
 * @brief Listen socket.
 *
 * @return If the operation succeeds, BSL_SUCCESS is returned.
 *         If the operation fails, BSL_SAL_ERR_NET_LISTEN is returned.
 */
typedef int32_t (*BslSalSockListen)(int32_t sockId, int32_t backlog);

/**
 * @ingroup bsl_sal
 * @brief Binding a socket.
 *
 * @return If the operation succeeds, BSL_SUCCESS is returned.
 *         If the operation fails, BSL_SAL_ERR_NET_BIND is returned.
 */
typedef int32_t (*BslSalSockBind)(int32_t sockId, BSL_SAL_SockAddr addr, size_t len);

/**
 * @ingroup bsl_sal
 * @brief Initiate a connection.
 *
 * @return If the operation succeeds, BSL_SUCCESS is returned
 *         If the operation fails, BSL_SAL_ERR_NET_CONNECT is returned.
 */
typedef int32_t (*BslSalSockConnect)(int32_t sockId, BSL_SAL_SockAddr addr, size_t len);

/**
 * @ingroup bsl_sal
 * @brief Send a message.
 *
 * @return If the operation succeeds, the length of the sent data is returned.
 *         If the operation fails, a negative value is returned.
 *         If the operation times out or the peer end disables the function, the value 0 is returned.
 */
typedef int32_t (*BslSalSockSend)(int32_t sockId, const void *msg, size_t len, int32_t flags);

/**
 * @ingroup bsl_sal
 * @brief Receive the message
 *
 * @return If the operation succeeds, the received data length is returned.
 *         If the operation fails, a negative value is returned.
 *         If the operation times out or the peer end disables the function, the value 0 is returned.
 */
typedef int32_t (*BslSalSockRecv)(int32_t sockfd, void *buff, size_t len, int32_t flags);

/**
 * @ingroup bsl_sal
 * @brief Same as linux function "sendto"
 */
typedef int32_t (*BslSalNetSendTo)(int32_t sock, const void *buf, size_t len, int32_t flags, void *address,
    int32_t addrLen, int32_t *err);

/**
 * @ingroup bsl_sal
 * @brief Same as linux function "recvfrom"
 */
typedef int32_t (*BslSalNetRecvFrom)(int32_t sock, void *buf, size_t len, int32_t flags, void *address,
    int32_t *addrLen, int32_t *err);

/**
 * @ingroup bsl_sal
 * @brief Same as linux function "select"
 */
typedef int32_t (*BslSelect)(int32_t nfds, void *readfds, void *writefds, void *exceptfds, void *timeout);

/**
 * @ingroup bsl_sal
 * @brief Same as linux function "ioctl"
 */
typedef int32_t (*BslIoctlSocket)(int32_t sockId, long cmd, unsigned long *arg);

/**
 * @ingroup bsl_sal
 * @brief return "errno"
 */
typedef int32_t (*BslGetErrno)(void);

/**
 * @ingroup bsl_sal
 * @brief Set block mode.
 *
* @return If the operation succeeds, BSL_SUCCESS is returned.
*         If the file open fails, BSL_SAL_ERR_NET_NOBLOCK is returned.
 */
typedef int32_t (*BslSetBlockMode)(int32_t fd, int32_t isBlock);

/**
 * @ingroup bsl_sal
 * @brief Wait for the socket to read and write data within the period specified by maxTime.
 *
 */
typedef int32_t (*BslSocketWait)(int32_t fd, int32_t forRead, int64_t maxTime);

/**
 * @ingroup bsl_sal
 * @brief Get the error code of the socket.
 *
 */
typedef int32_t (*BslSocketError)(int32_t fd);

/**
 * @ingroup bsl_sal
 *
 * Definition of the time callback interface.
 */

/**
 * @ingroup bsl_sal
 * @brief Obtains the current UTC time
 *
 * @return Return the utc time.
 */
typedef BslUnixTime (*BslGetUtcTime)(void);

/**
 * @ingroup bsl_sal
 * @brief Obtains the current system time. The time type is BSL_TIME.
 *
 * @param bslTime [IN/OUT] Pointer to the BSL_TIME structure.
 * @return Error code in bsl_errno.h.
 */
typedef int32_t (*BslGetBslTime)(BSL_TIME *bslTime);

/**
 * @ingroup bsl_sal
 * @brief Convert the utc time to BSL_TIME.
 *
 * @param utcTime [IN] UTC time.
 * @param bslTime [IN/OUT] Pointer to the BSL_TIME structure.
 * @return Error code in bsl_errno.h.
 */
typedef int32_t (*BslUtcTimeToBslTime)(int64_t utcTime, BSL_TIME *sysTime);

/**
 * @ingroup bsl_sal
 * @brief Sets the program to sleep for a specified time, in seconds.
 *
 * @param time [IN] Sleep time.
 */
typedef void (*BslSleep)(uint32_t time);

/**
 * @ingroup bsl_sal
 * @brief Obtain the number of ticks that the system has experienced since startup.
 *
 * @return Return tick for success and -1 for failure.
 */
typedef long (*BslTick)(void);

/**
 * @ingroup bsl_sal
 * @brief Obtain the number of system ticks per second.
 *
 * @return Return number of ticks per secone for success and -1 for failure.
 */
typedef long (*BslTicksPerSec)(void);

/**
 * @ingroup bsl_sal
 *
 * Definition of the file callback interface.
 */

/**
* @ingroup bsl_sal
* @brief Open the file.
*
* @return If the operation succeeds, BSL_SUCCESS is returned.
*         If the file open fails, BSL_SAL_ERR_FILE_OPEN is returned.
*         If parameter error, BSL_NULL_INPUT is returned.
*/
typedef int32_t (*BslSalFileOpen)(bsl_sal_file_handle *stream, const char *path, const char *mode);

/**
* @ingroup bsl_sal
* @brief Read the file.
*
* @return  If the file is successfully read, BSL_SUCCESS is returned.
*          If the file read fails, BSL_SAL_ERR_FILE_READ is returned.
*          If parameter error, BSL_NULL_INPUT is returned.
*/
typedef int32_t (*BslSalFileRead)(bsl_sal_file_handle stream, void *buffer, size_t size, size_t num, size_t *len);

/**
* @ingroup bsl_sal
* @brief Write the file
*
* @return  If the file is successfully write, BSL_SUCCESS is returned.
*          If the file read fails, BSL_SAL_ERR_FILE_WRITE is returned.
*          If parameter error, BSL_NULL_INPUT is returned.
*/
typedef int32_t (*BslSalFileWrite)(bsl_sal_file_handle stream, const void *buffer, size_t size, size_t num);

/**
* @ingroup bsl_sal
* @brief Close the file.
*
* @return NA
*/
typedef void (*BslSalFileClose)(bsl_sal_file_handle stream);

/**
* @ingroup bsl_sal
* @brief Obtain the file length.
*
* @return If the file length is obtained successfully, BSL_SUCCESS is returned.
*         If the file read fails, BSL_SAL_ERR_FILE_LENGTH is returned.
*         If parameter error, BSL_NULL_INPUT is returned.
*/
typedef int32_t (*BslSalFileLength)(const char *path, size_t *len);

/**
* @ingroup bsl_sal
* @brief Test the error indicator for the given stream.
*
* @return If the error indicator associated with the stream was set or the stream is null, false is returned.
*         Otherwise, true is returned.
*/
typedef bool (*BslSalFileError)(bsl_sal_file_handle stream);

/**
* @ingroup bsl_sal
* @brief Get the current file position in stream.
*
* @return  If the file position is obtained successfully, BSL_SUCCESS is returned.
*          If fail to get the file position, BSL_SAL_ERR_FILE_TELL is returned.
*          If parameter error, BSL_NULL_INPUT is returned.
*/
typedef int32_t (*BslSalFileTell)(bsl_sal_file_handle stream, long *pos);

/**
* @ingroup bsl_sal
* @brief Change the current file position associated with stream to a new location within the file.
*
* @return  If successful, BSL_SUCCESS is returned.
*          If fail to change the file position, BSL_SAL_ERR_FILE_SEEK is returned.
*          If parameter error, BSL_NULL_INPUT is returned.
*/
typedef int32_t (*BslSalFileSeek)(bsl_sal_file_handle stream, long offset, int32_t origin);

/**
* @ingroup bsl_sal
* @brief Read a line from the stream and store it into the buffer.
*
* @return  If successful, the same read buffer is returned.
*          Otherwise, return NULL.
*/
typedef char *(*BslSalFGets)(bsl_sal_file_handle stream, char *buf, int32_t readLen);

/**
* @ingroup bsl_sal
* @brief  Write a string to the specified stream.
*
* @return If successful, return true. Otherwise, return false.
*/
typedef bool (*BslSalFPuts)(bsl_sal_file_handle stream, const char *buf);

/**
* @ingroup bsl_sal
* @brief Flush cache buffer associated with the specified output stream.
*
* @return If successful, return true. Otherwise, return false.
*/
typedef bool (*BslSalFlush)(bsl_sal_file_handle stream);

/**
* @ingroup bsl_sal
* @brief Indicate whether the end-of-file flag is set for the given stream.
*
* @return If successful, return BSL_SUCCESS. Otherwise, see bsl_errno.h.
*/
typedef int32_t (*BslSalFeof)(bsl_sal_file_handle stream);

/**
* @ingroup bsl_sal
* @brief Set the attributes associated with the terminal referred to by the open stream.
*
* @return  If successful, BSL_SUCCESS is returned.
*          If fail to set, BSL_SAL_ERR_FILE_SET_ATTR is returned.
*          If parameter error, BSL_NULL_INPUT is returned.
*/
typedef int32_t (*BslSalFSetAttr)(bsl_sal_file_handle stream, int cmd, const void *arg);

/**
* @ingroup bsl_sal
* @brief  Get the attributes associated with the terminal referred to by the open stream.
*
* @return  If successful, BSL_SUCCESS is returned.
*          If fail to get, BSL_SAL_ERR_FILE_GET_ATTR is returned.
*          If parameter error, BSL_NULL_INPUT is returned
*/
typedef int32_t (*BslSalFGetAttr)(bsl_sal_file_handle stream, void *arg);

/**
 * @ingroup SAL
 * @brief Registers or deregisters the system callback function.
 *
 * @param funcType [IN] Callback function type.
 * @param funcCb [IN] Pointer to the callback function.
 * @return BSL_SUCCESS is successfully executed.
 *         BSL_SAL_ERR_BAD_PARAM The parameter is incorrect.
 *         Otherwise, error code in bsl_errno.h.
 */
int32_t BAL_SAL_CallBack_Ctrl(BSL_SAL_CB_FUNC_TYPE funcType, void *funcCb);

/**
 * @ingroup bsl_sal_net
 * @brief   Check the socket descriptor.
 *
 * @par Description:
 * Check the socket descriptor.
 *
 * @attention None
 * @param nfds [IN] Total number of file descriptors that are listened on
 * @param readfds [IN] Readable file descriptor (optional)
 * @param writefds [IN] Descriptor of a writable file. This parameter is optional.
 * @param exceptfds [IN] Exception file descriptor (optional)
 * @param timeout [IN] Set the timeout interval.
 * @retval: if the operation succeeds, Number of ready descriptors returned;
 * if the operation fails, a negative value is returned;
 * if the operation times out, 0 is returned
 */
int32_t BSL_SAL_Select(int32_t nfds, void *readfds, void *writefds, void *exceptfds, void *timeout);

/**
 * @ingroup bsl_sal_net
 * @brief   Device control interface function
 *
 * @par Description:
 * Device control interface function
 *
 * @attention None
 * @param sockId [IN] Socket file descriptor ID
 * @param cmd [IN] Interaction protocol
 * @param arg [IN] Parameter
 * @retval If the operation succeeds, BSL_SUCCESS is returned.
 * If the operation fails, BSL_SAL_ERR_NET_IOCTL is returned.
 */
int32_t BSL_SAL_Ioctlsocket(int32_t sockId, long cmd, unsigned long *arg);

/**
 * @ingroup bsl_sal_net
 * @brief   Obtain the last error corresponding to the socket.
 *
 * @par Description:
 * Obtain the last error corresponding to the socket.
 *
 * @attention none
 * @retval Return the corresponding error.
 */
int32_t BSL_SAL_SockGetLastSocketError(void);

/**
 * @ingroup bsl_sal_net
 * @brief Set the blocking mode.
 *
 * @par Description:
 * Set the blocking mode.
 *
 * @attention none
 * @param fd [IN] Socket file descriptor ID
 * @param isBlock [IN] Indicates whether the block mode is used.
 * @retval If the operation succeeds, BSL_SUCCESS is returned;
 *         If the operation fails, BSL_SAL_ERR_NET_NOBLOCK is returned.
 */
int32_t BSL_SAL_SetBlockMode(int32_t fd, int32_t isBlock);

/**
 * @ingroup bsl_sal_net
 * @brief   Wait for the socket to read and write data within the period specified by maxTime.
 *
 * @par Description:
 * Wait for Socket read/write within maxTime.
 *
 * @attention None
 * @param fd [IN] Socket file descriptor ID
 * @param forRead [IN] 0: waiting for write; 1: waiting for read
 * @param maxTime [IN] Maximum waiting time
 * @retval If the operation is successful, the select result is returned;
 * If the operation fails, the select result is returned.
 * For details, see bsl_errno.h.
 */
int32_t BSL_SAL_SocketWait(int32_t fd, int32_t forRead, int64_t maxTime);

/**
 * @ingroup bsl_sal_net
 * @brief   Call getsockopt to obtain the socket error code.
 *
 * @par Description:
 * Call getsockopt to obtain the error code of the socket.
 *
 * @attention None
 * @param fd [IN] Socket file descriptor ID
 * @retval If the operation succeeds, BSL_SUCCESS is returned;
 *         If the operation fails, the last error code is returned.
 */
int32_t BSL_SAL_SocketError(int32_t fd);

/**
 * @ingroup SAL
 * @brief String comparison
 *
 * @par Description:
 * String comparison
 *
 * @attention
 * @param str1 [IN] First string to be compared.
 * @param str2 [IN] Second string to be compared.
 * @return If the parameter is abnormal, BSL_NULL_INPUT is returned.
 * @return If the strings are the same, 0 is returned;
 *         Otherwise, the difference between different characters is returned.
 */
int32_t BSL_SAL_StrcaseCmp(const char *str1, const char *str2);

/**
 * @ingroup SAL
 * @brief Search for the corresponding character position in a string.
 *
 * @par Description:
 * Search for the corresponding character position in a string.
 *
 * @attention
 * @param str [IN] String
 * @param character [IN] Character to be searched for
 * @param count [IN] Range to be found
 * @return If a character is found, the position of the character is returned;
 *         Otherwise, NULL is returned.
 */
void *BSL_SAL_Memchr(const char *str, int32_t character, size_t count);

/**
 * @ingroup SAL
 * @brief Convert string to number
 *
 * @par Description:
 * Convert string to number
 *
 * @attention
 * @param str [IN] String to be converted.
 * @return If the conversion is successful, the corresponding number is returned;
 *         Otherwise, the value 0 is returned.
 */
int32_t BSL_SAL_Atoi(const char *str);

/**
 * @ingroup SAL
 * @brief Obtain the length of a given string.
 *
 * @par Description:
 * Obtain the length of a given string.
 *
 * @attention
 * @param string [IN] String to obtain the length.
 * @param count [IN] Maximum length
 * @return If the parameter is abnormal, 0 is returned.
 * @return If the length of a string is greater than the maximum length,
 *         count is returned. Otherwise, the actual length of the string is returned.
 */
uint32_t BSL_SAL_Strnlen(const char *string, uint32_t count);


/**
 * @ingroup bsl_sal_async
 * @brief   Equivalent to linux function "makecontext"
 */
typedef void (*BslMakeContext)(void *ucp, void (*func)(void), int argc, const void *args);

/**
 * @ingroup bsl_sal_async
 * @brief   Equivalent to linux function "swapcontext"
 */
typedef int (*BslSwapContext)(void *oucp, const void *ucp);

/**
 * @ingroup bsl_sal_async
 * @brief   Equivalent to linux function "getcontext"
 */
typedef int (*BslGetContext)(void *ucp);

#ifdef __cplusplus
}
#endif

#endif