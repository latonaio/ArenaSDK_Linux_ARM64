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
#include "ArenaCApi.h"

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "
#define TAB4 "        "

// Callbacks: On Event
//    This example demonstrates configuring a callback with events. Events are a
//    subset of nodes that invoke callbacks through the underlying events engine.
//    The events engine is first initialized to listen for events, then the
//    callback is registered using the timestamp test event node. The event is
//    generated, along with any data generated from the event. The example then
//    waits for the event to process in order to invoke the callback. Registered
//    callbacks must also be deregistered before deinitializing the events engine
//    in order to avoid memory leaks.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Event timeout
#define EVENT_TIMEOUT 2000

// Number of events to generate
#define NUM_EVENTS 5

// system timeout
#define SYSTEM_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// callback function
// (1) prints information from the callback
void PrintNodeValue(acNode hNode, void* pUserData)
{

	// Do something on callback
	printf("%sMessage from callback\n", TAB3);

	// print test event timestamp
	char pValueBuf[MAX_BUF];
	size_t valueBufLen = MAX_BUF;

	acValueToString(hNode, pValueBuf, &valueBufLen);

	printf("%sTest event timestamp: %s\n", TAB4, pValueBuf);

	// print device serial number from pUserData
	char* pSerialNumberUserData = (char*)pUserData;

	printf("%sDevice Serial Number: %s\n", TAB4, pSerialNumberUserData);
}

// demonstrates callbacks with events
// (1) gets event node
// (2) initializes events
// (3) registers callback
// (4) generates event
// (5) waits on event, invoking callback
// (6) deregisters callback
// (7) deinitializes events
AC_ERROR ConfigureCallbackOnEventTestTimestamp(acSystem hSystem, acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Get event node
	//    Events require callbacks and the events engine to operate. This example
	//    uses the timestamp test event node to show the setup and teardown of
	//    events.
	printf("%sGet event node\n", TAB1);
	acNode hEventTestTimestampNode = NULL;

	err = acNodeMapGetNode(hNodeMap, "EventTestTimestamp", &hEventTestTimestampNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Initialize events
	//    Initialize the events engine before registering the callback. Events
	//    nodes aren't available until the events engine has been initialized, so
	//    callback registration should come after events engine initialization.
	printf("%sInitialize events\n", TAB1);

	err = acDeviceInitializeEvents(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get device serial number to retrieve during callback. This could be used
	// to identify devices among multiple cameras.
	size_t index = 0;
	char pSerialNumberBuf[MAX_BUF];
	size_t bufLen = MAX_BUF;

	err = acSystemGetDeviceSerial(hSystem, index, pSerialNumberBuf, &bufLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Register callback
	//    Register the callback before generating events. If an event is
	//    generated but no callbacks are registered, the events engine processes
	//    the event as normal, but nothing happens because no function has been
	//    registered to it.
	printf("%sRegister callback\n", TAB1);
	acCallback hCallback = NULL;

	err = acCallbackRegister(&hCallback, hEventTestTimestampNode, PrintNodeValue, &pSerialNumberBuf);
	if (err != AC_ERR_SUCCESS)
		return err;

	// generate events
	printf("%sGenerate and wait on events\n", TAB1);

	for (int i = 0; i < NUM_EVENTS; i++)
	{
		// Generate event
		//    Generate the event before waiting on it. Generating the event
		//    causes the event to occur, but does not process it or invoke the
		//    registered callback.
		printf("%sGenerate event\n", TAB2);

		err = acNodeMapExecute(hNodeMap, "TestEventGenerate");
		if (err != AC_ERR_SUCCESS)
			return err;

		// Wait on event
		//    Wait on event to process it, invoking the registered callback. The
		//    data is created from the event generation, not from waiting on it.
		printf("%sWait on event\n", TAB2);

		err = acDeviceWaitOnEvent(hDevice, EVENT_TIMEOUT);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// Deregister callback
	//    Failing to deregister a callback results in a memory leak. Once a
	//    callback has been registered, it will no longer be invoked when a node
	//    is invalidated.
	printf("%sDeregister callback\n", TAB1);

	err = acCallbackDeregister(hCallback);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Deinitialize events
	//    Deinitialize the events engine in order to deallocate memory and stop
	//    processing events. Failing to deinitialize events results in a memory
	//    leak.
	printf("%sDeinitialize events\n", TAB1);

	err = acDeviceDeinitializeEvents(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	return err;
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

// error buffer length
#define ERR_BUF 512

#define CHECK_RETURN                                  \
	if (err != AC_ERR_SUCCESS)                        \
	{                                                 \
		char pMessageBuf[ERR_BUF];                    \
		size_t pBufLen = ERR_BUF;                     \
		acGetLastErrorMessage(pMessageBuf, &pBufLen); \
		printf("\nError: %s", pMessageBuf);           \
		printf("\n\nPress enter to complete\n");      \
		getchar();                                    \
		return -1;                                    \
	}

int main()
{
	printf("C_Callback_OnEvent\n");
	AC_ERROR err = AC_ERR_SUCCESS;

	// prepare example
	acSystem hSystem = NULL;
	err = acOpenSystem(&hSystem);
	CHECK_RETURN;
	err = acSystemUpdateDevices(hSystem, SYSTEM_TIMEOUT);
	CHECK_RETURN;
	size_t numDevices = 0;
	err = acSystemGetNumDevices(hSystem, &numDevices);
	CHECK_RETURN;
	if (numDevices == 0)
	{
		printf("\nNo camera connected\nPress enter to complete\n");
		getchar();
		return -1;
	}
	acDevice hDevice = NULL;
	err = acSystemCreateDevice(hSystem, 0, &hDevice);
	CHECK_RETURN;

	// run example
	printf("Commence example\n\n");
	err = ConfigureCallbackOnEventTestTimestamp(hSystem, hDevice);
	CHECK_RETURN;
	printf("\nExample complete\n");

	// clean up example
	err = acSystemDestroyDevice(hSystem, hDevice);
	CHECK_RETURN;
	err = acCloseSystem(hSystem);
	CHECK_RETURN;

	printf("Press enter to complete\n");
	getchar();
	return -1;
}
