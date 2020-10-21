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

#include "stdafx.h"
#include "ArenaCThreadLinux.h"
#if defined linux

void acThreadCreate(void* pThreadStartRoutine(void*), void* lpParam, pthread_t* phThreadId)
{

	pthread_create(&*phThreadId, NULL, pThreadStartRoutine, lpParam);
}

void acThreadDestroy(pthread_t phThreadId)
{
	pthread_join(phThreadId, NULL);
}

void acThreadLockInitialize(pthread_mutex_t* pMutex)
{
	pthread_mutex_init(pMutex, NULL);
}

void acThreadLockDeinitialize(pthread_mutex_t* pMutex)
{
	pthread_mutex_destroy(pMutex);
}

void acThreadLock(pthread_mutex_t* pMutex)
{
	pthread_mutex_lock(pMutex);
}

void acThreadUnlock(pthread_mutex_t* pMutex)
{
	pthread_mutex_unlock(pMutex);
}

void acThreadConditionVariableInitialize(pthread_cond_t* pConditionVariable)
{
	pthread_cond_init(pConditionVariable, NULL);
}

void acThreadConditionVariableDeinitialize(pthread_cond_t* pConditionVariable)
{
	pthread_cond_destroy(pConditionVariable);
}

void acThreadConditionVariableSleep(pthread_cond_t* pConditionVariable, pthread_mutex_t* pMutex)
{
	pthread_cond_wait(pConditionVariable, pMutex);
}

void acThreadConditionVariableWake(pthread_cond_t* pConditionVariable)
{
	pthread_cond_signal(pConditionVariable);
}
#endif
