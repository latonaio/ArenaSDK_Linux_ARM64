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
#include "ArenaCThreadWindows.h"
#include <stdlib.h>

#if defined _WIN32

void acThreadCreate(LPTHREAD_START_ROUTINE lpThreadStartRoutine, LPVOID lpParam, HANDLE* phThreadId)
{
	*phThreadId = CreateThread(0, 0, lpThreadStartRoutine, lpParam, 0, NULL);
}

void acThreadDestroy(HANDLE phThreadId)
{
	WaitForSingleObject(phThreadId, INFINITE);
	CloseHandle(phThreadId);
}

void acThreadLockInitialize(CRITICAL_SECTION* pMutex)
{
	InitializeCriticalSection(pMutex);
}

void acThreadLockDeinitialize(CRITICAL_SECTION* pMutex)
{
	DeleteCriticalSection(pMutex);
}

void acThreadLock(CRITICAL_SECTION* pMutex)
{
	EnterCriticalSection(pMutex);
}

void acThreadUnlock(CRITICAL_SECTION* pMutex)
{
	LeaveCriticalSection(pMutex);
}

void acThreadConditionVariableInitialize(CONDITION_VARIABLE* pConditionVariable)
{
	InitializeConditionVariable(pConditionVariable);
}

void acThreadConditionVariableDeinitialize(CONDITION_VARIABLE* pConditionVariable)
{
	// no condition variable deinitialize in Windows
}

void acThreadConditionVariableSleep(CONDITION_VARIABLE* pConditionVariable, CRITICAL_SECTION* pMutex)
{
	SleepConditionVariableCS(pConditionVariable, pMutex, INFINITE);
}

void acThreadConditionVariableWake(CONDITION_VARIABLE* pConditionVariable)
{
	WakeConditionVariable(pConditionVariable);
}
#endif
