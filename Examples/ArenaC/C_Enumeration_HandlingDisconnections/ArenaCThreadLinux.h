/***************************************************************************************
 ***                                                                                 ***
 ***  Copyright (c) 2019, Lucid Vision Labs, Inc.                                    ***
 ***                                                                                 ***
 ***  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     ***
 ***  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       ***
 ***  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    ***
 ***  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         ***
 ***  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ***
 ***  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  ***
 ***  SOFTWARE.                                                                      ***
 ***                                                                                 ***
 ***************************************************************************************/
#include "ArenaCApi.h"
#if defined linux
#include <unistd.h>	 // defines access to POSIX OS
#include <pthread.h> // defines multithreading

/**
 * @macrofn #define THREAD_FUNCTION_SIGNATURE(__FUNCTION__) void* __FUNCTION__(void* lpParam);
 *
 * @param __FUNCTION__
 *  - Type: none
 *  - [In] parameter
 *  - Function name
 *
 * @definition
 *  - void* __FUNCTION__(void* lpParam)
 *  - Defines the function signature
 *
 * <B> THREAD_FUNCTION_SIGNATURE </B> creates the function signature
 * with the appropriate type to be used for multithreaded functions.
 */
#define THREAD_FUNCTION_SIGNATURE(__FUNCTION__) void* __FUNCTION__(void* lpParam)

/**
 * @macrofn #define THREAD_RETURN(errorCode) pthread_exit(NULL);
 *
 * @param errorCode
 *  - Type: none
 *  - [In] parameter
 *  - Return code
 *
 * @definition
 *  - pthread_exit(NULL);
 *  - Exits the threads
 *
 * <B> THREAD_RETURN </B>  defines the appropriate
 * return statement to exit the thread
 */
#define THREAD_RETURN(errorCode) pthread_exit(NULL);

/**
 * @macrofn #define THREAD_LOCK_VARIABLE_VARIABLE(pMutex) pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 *
 * @param mutex
 *  - Type: none
 *  - [In] parameter
 *  - Mutex variable
 *
 * @definition
 *  - pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 *  - Defines the mutex variable
 *
 * <B>  THREAD_LOCK_VARIABLE </B> creates the mutex variable of the appropriate type
 *     to be used for thread locks
 */
#define THREAD_LOCK_VARIABLE(mutex) pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @macrofn #define THREAD_CONDITION_VARIABLE(conditionVariable) pthread_cond_t conditionVariable = PTHREAD_COND_INITIALIZER;
 *
 * @param conditionVariable
 *  - Type: none
 *  - [In] parameter
 *  - Condition variable
 *
 * @definition
 *  - pthread_cond_t conditionVariable = PTHREAD_COND_INITIALIZER;
 *  - Defines the mutex variable
 *
 * <B> THREAD_CONDITION_VARIABLE </B> creates the mutex variable
 * of the appropriate type to be used for condition signalling
 */
#define THREAD_CONDITION_VARIABLE(conditionVariable) pthread_cond_t conditionVariable = PTHREAD_COND_INITIALIZER;

/**
 * @macrofn #define THREAD_ID(hThreadId) pthread_t hThreadId
 *
 * @param hThread
 *  - Type: none
 *  - [In] parameter
 *  - Thread id
 *
 * @definition
 *  - pthread_t hThread
 *  - Defines the thread id type
 *
 * <B> THREAD_ID </B> creates the thread id of the appropriate type
 */
#define THREAD_ID(hThreadId) pthread_t hThreadId

/**
* @fn void acThreadCreate(void* pThreadStartRoutine(void*), void* lpParam, pthread_t* phThreadId);
*
* @param lpThreadStartRoutine
*  - Type: void*
*  - [In] parameter
*  - Starting thread address
*
* @param lpParam
*  - Type: void*
*  - [In] parameter
*  - Input data
*
* @param phThreadId
*  - Type: pthread_t*
*  - [Out] parameter
*  - Thread id of the created thread
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadCreate </B> creates the new thread and outputs the thread id. phThreadId must be defined 
* using <B> THREAD_ID </B> before creating the new thread. 
*/
void acThreadCreate(void* pThreadStartRoutine(void*), void* lpParam, pthread_t* phThreadId);

/**
* @fn void acThreadDestroy(pthread_t phThreadId);
*
* @param phThreadId
*  - Type: pthread_t
*  - [In] parameter
*  - Thread id
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadDestroy </B> waits for thread to finish and joins thread
*/
void acThreadDestroy(pthread_t phThreadId);

/**
* @fn void acThreadLockInitialize(pthread_mutex_t* pMutex);
*
* @param mutex
*  - Type: pthread_mutex_t*
*  - [In] parameter
*  - Mutex variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadLockInitialize </B> initializes the thread lock. The thread lock variable
* must be defined using <B> THREAD_LOCK_VARIABLE </B> before initializing. 
*/
void acThreadLockInitialize(pthread_mutex_t* pMutex);

/**
* @fn void acThreadLockDeinitialize(pthread_mutex_t* pMutex);
*
* @param mutex
*  - Type: pthread_mutex_t*
*  - [In] parameter
*  - Mutex variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadLockDeinitialize </B> destroys the previously initialized thread lock variable.
*/
void acThreadLockDeinitialize(pthread_mutex_t* pMutex);

/**
* @fn void acThreadLock(pthread_mutex_t* pMutex);
*
* @param pMutex
*  - Type: pthread_mutex_t*
*  - [In] parameter
*  - Mutex lock variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadLock </B> locks thread. The lock must be initialized before locking the thread.
*/
void acThreadLock(pthread_mutex_t* pMutex);

/**
* @fn void acThreadUnlock(pthread_mutex_t* pMutex);
*
* @param pMutex
*  - Type: pthread_mutex_t*
*  - [In] parameter
*  - Mutex variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadUnlock </B> unlocks the thread.
*/
void acThreadUnlock(pthread_mutex_t* pMutex);

/**
* @fn void acThreadConditionVariableInitialize(pthread_cond_t* pConditionVariable);
*
* @param pConditionVariable
*  - Type: pthread_cond_t
*  - [In] parameter
*  - Condition variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadConditionVariableInitialize </B> initializes the condition variable. The condition variable
* name must be defined using <B> THREAD_CONDITION_VARIABLE </B> before initializing.
*/
void acThreadConditionVariableInitialize(pthread_cond_t* pConditionVariable);

/**
* @fn void acThreadConditionVariableDeinitialize(pthread_cond_t* pConditionVariable);
*
* @param pConditionVariable
*  - Type: pthread_cond_t
*  - [In] parameter
*  - Condition Variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadConditionVariableDeinitialize </B> destroys a previously initialized condition variable. 
*/
void acThreadConditionVariableDeinitialize(pthread_cond_t* pConditionVariable);

/**
* @fn void acThreadConditionVariableSleep(pthread_cond_t* pConditionVariable, pthread_mutex_t* pMutex);
*
* @param pConditionVariable
*  - Type: pthread_cond_t*
*  - [In] parameter
*  - Condition variable
*
* @param pMutex
*  - Type: pthread_mutex_t*
*  - [In] parameter
*  - Mutex variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadConditionVariableSleep </B> waits for the thread to be signalled awake.
*/
void acThreadConditionVariableSleep(pthread_cond_t* pConditionVariable, pthread_mutex_t* pMutex);

/**
* @fn void acThreadConditionVariableWake(pthread_cond_t* pConditionVariable);
*
* @param pConditionVariable
*  - Type: pthread_cond_t*
*  - [In] parameter
*  - Condition variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadConditionVariableWake </B> signals thread to wake.
*/
void acThreadConditionVariableWake(pthread_cond_t* pConditionVariable);
#endif
