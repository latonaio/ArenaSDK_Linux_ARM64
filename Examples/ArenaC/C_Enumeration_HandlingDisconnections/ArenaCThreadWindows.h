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
#if defined _WIN32
#include <windows.h> // defines winapi functionality for multithreading

/**
 * @macrofn #define THREAD_FUNCTION_SIGNATURE(__FUNCTION__) DWORD WINAPI __FUNCTION__(LPVOID lpParam)
 *
 * @param __FUNCTION__
 *  - Type: none
 *  - [In] parameter
 *  - Function name
 *
 * @definition
 *  - DWORD WINAPI __FUNCTION__(LPVOID lpParam)
 *  - Defines the function signature
 *
 * <B> THREAD_FUNCTION_SIGNATURE </B> creates the function signature
 * with the appropriate type to be used for multithreaded functions.
 */
#define THREAD_FUNCTION_SIGNATURE(__FUNCTION__) DWORD WINAPI __FUNCTION__(LPVOID lpParam)

/**
 * @macrofn THREAD_RETURN(errorCode) return errorCode;
 *
 * @param errorCode
 *  - Type: none
 *  - [In] parameter
 *  - return code
 *
 * @definition
 *  - return errorCode;
 *  - Exits the threads
 *
 * <B> THREAD_RETURN </B>  defines the appropriate
 * return statement to exit the thread
 */
#define THREAD_RETURN(errorCode) return errorCode;

/**
 * @macrofn #defineTHREAD_LOCK_VARIABLE_VARIABLE(pMutex) CRITICAL_SECTION mutex;
 *
 * @param mutex
 *  - Type: none
 *  - [In] parameter
 *  - Mutex variable
 *
 * @definition
 *  - CRITICAL_SECTION mutex;
 *  - Defines the mutex variable
 *
 * <B>  THREAD_LOCK_VARIABLE </B> creates the mutex variable of the appropriate type
 *     to be used for thread locks
 */
#define THREAD_LOCK_VARIABLE(mutex) CRITICAL_SECTION mutex;

/**
 * @macrofn #define THREAD_CONDITION_VARIABLE(pConditionVariable) CONDITION_VARIABLE conditionVariable;
 *
 * @param conditionVariable
 *  - Type: none
 *  - [In] parameter
 *  - Condition variable
 *
 * @definition
 *  - CONDITION_VARIABLE conditionVariable;
 *  - Defines the mutex variable
 *
 * <B> THREAD_CONDITION_VARIABLE </B> creates the mutex variable of the appropriate 
 * type to be used for condition signalling
 */
#define THREAD_CONDITION_VARIABLE(conditionVariable) CONDITION_VARIABLE conditionVariable;

/**
 * @macrofn #define THREAD_ID(hThreadId) HANDLE hThreadId
 *
 * @param hThreadId
 *  - Type: none
 *  - [In] parameter
 *  - Thread id
 *
 * @definition
 *  - HANDLE hThread
 *  - Defines the thread id type
 *
 * <B> THREAD_ID </B> creates the thread id of the appropriate type.
 */
#define THREAD_ID(hThreadId) HANDLE hThreadId

/**
* @fn void acThreadCreate(LPTHREAD_START_ROUTINE lpThreadStartRoutine, LPVOID lpParam);
*
* @param lpThreadStartRoutine
*  - Type: LPTHREAD_START_ROUTINE
*  - [In] parameter
*  - Starting thread address
*
* @param lpParam
*  - Type: LPVOID
*  - [In] parameter
*  - Accepts null
*  - Input data
*
* @param phThreadId
*  - Type: HANDLE*
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
void acThreadCreate(LPTHREAD_START_ROUTINE lpThreadStartRoutine, LPVOID lpParam, HANDLE* phThreadId);

/**
* @fn void acThreadDestroy(HANDLE phThreadId);
*
* @param phThreadId
*  - Type: HANDLE
*  - [In] parameter
*  - thread handle
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadDestroy </B> waits for thread to finish and joins thread.
*/
void acThreadDestroy(HANDLE phThreadId);

/**
* @fn void acThreadLockInitialize(CRITICAL_SECTION* pMutex);
*
* @param mutex
*  - Type: CRITICAL_SECTION*
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
void acThreadLockInitialize(CRITICAL_SECTION* pMutex);

/**
* @fn void acThreadLockDeinitialize(CRITICAL_SECTION* pMutex);
*
* @param mutex
*  - Type: CRITICAL_SECTION*
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
void acThreadLockDeinitialize(CRITICAL_SECTION* pMutex);

/**
* @fn void acThreadLock(CRITICAL_SECTION* pMutex);
*
* @param mutex
*  - Type: CRITICAL_SECTION*
*  - [In] parameter
*  - Mutex variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadLock </B> locks thread. The lock must be initialized before locking the thread.
*/
void acThreadLock(CRITICAL_SECTION* pMutex);

/**
* @fn void acThreadUnlock(CRITICAL_SECTION* pMutex);
*
* @param mutex
*  - Type: CRITICAL_SECTION*
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
void acThreadUnlock(CRITICAL_SECTION* pMutex);

/**
* @fn void acThreadConditionVariableInitialize(CONDITION_VARIABLE* pConditionVariable);
*
* @param hThread
*  - Type: HANDLE
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
void acThreadConditionVariableInitialize(CONDITION_VARIABLE* pConditionVariable);

/**
* @fn void acThreadConditionVariableDeinitialize(CONDITION_VARIABLE* pConditionVariable);
*
* @param hThread
*  - Type: HANDLE
*  - [In] parameter
*  - Condition variable
*
* @return
*  - Type: AC_ERROR
*  - Error code for the function
*  - Returns AC_ERR_SUCCESS (0) on success
*
* <B> acThreadConditionVariableDeinitialize </B> destroys a previously initialized condition variable.
*/
void acThreadConditionVariableDeinitialize(CONDITION_VARIABLE* pConditionVariable);

/**
* @fn void acThreadConditionVariableSleep(CONDITION_VARIABLE* conditionVariable, CRITICAL_SECTION* pMutex);
*
* @param conditionVariable
*  - Type: CONDITION_VARIABLE*
*  - [In] parameter
*  - Condition variable
*
* @param mutex
*  - Type: CRITICAL_SECTION*
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
void acThreadConditionVariableSleep(CONDITION_VARIABLE* pConditionVariable, CRITICAL_SECTION* pMutex);

/**
* @fn void acThreadConditionVariableWake(CONDITION_VARIABLE* pConditionVariable);
*
* @param conditionVariable
*  - Type: CONDITION_VARIABLE*
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
void acThreadConditionVariableWake(CONDITION_VARIABLE* pConditionVariable);
#endif
