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
#define TAB4 "                                             "

// Callbacks: Polling
//    This example demonstrates configuring a callback with polling. Polling
//    allows for callbacks to be invoked over time.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Sleep time
//    Windows and Linux operating systems have different sleep functions which
//    cause the calling thread to sleep until a specified amount of time has
//    elapsed. In Windows, Sleep() is specified in milliseconds while in Linux
//    sleep() is specified in seconds.
#if defined _WIN32
#include <Windows.h>
// Time to pass during Sleep (in milliseconds)
#define SLEEP_TIME 1000
#elif defined linux
#include <unistd.h>
// Time to pass during sleep (in seconds)
#define SLEEP_TIME 1
#endif

// Number of times to poll
#define NUM_POLLS 50

// Time to pass between polling (in milliseconds)
#define ELAPSED_TIME 500

// system timeout
#define SYSTEM_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// callback counter
int g_count = 0;

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// callback function
// (1) prints information from the callback
void PrintNodeValue(acNode hNode, void* pUserData)
{

	// Do something on callback
	//    This function is registered with the callback and invoked when the node
	//    map has been polled if the elapsed time is over the threshold of the
	//    node.
	char* pSerialNumberUserData = (char*)pUserData;

	// print device serial number from pUserData
	printf("\r%sDevice Serial Number: %s", TAB4, pSerialNumberUserData);
	fflush(stdout);

	// get device temperature from node
	char pValueBuf[MAX_BUF];
	size_t valueBufLen = MAX_BUF;

	acValueToString(hNode, pValueBuf, &valueBufLen);

	// get device temperature unit
	char pUnitBuf[MAX_BUF];
	size_t unitBufLen = MAX_BUF;

	acFloatGetUnit(hNode, pUnitBuf, &unitBufLen);

	// count number of callbacks
	g_count = g_count + 1;

	printf("\r%s %4i Current device temperature:%s%c%s", TAB3, g_count, pValueBuf, 167, pUnitBuf);
	fflush(stdout);
}

// demonstrates polling
// (1) gets the node map and node to poll
// (2) registers callback
// (3) starts stream
// (4) polls node map at regular intervals, invoking callback
// (5) stops stream
// (6) deregisters callback
AC_ERROR ConfigureCallbackToPollDeviceTemperature(acSystem hSystem, acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Get node map and device temperature node
	//    Nodes are polled through their node maps. This example demonstrates
	//    polling the device temperature node. It has a polling time of 1 second,
	//    which means that its callback will not be invoked within 1 second of
	//    the last time it has been polled.
	printf("%sGet node map and device temperature node\n", TAB1);
	acNodeMap hNodeMap = NULL;
	acNode hDeviceTemperatureNode = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapGetNode(hNodeMap, "DeviceTemperature", &hDeviceTemperatureNode);
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

	err = acCallbackRegister(&hCallback, hDeviceTemperatureNode, PrintNodeValue, &pSerialNumberBuf);
	if (err != AC_ERR_SUCCESS)
		return err;

	// start stream
	printf("%sStart stream\n", TAB1);

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Poll at regular intervals
	//    The polling time of device temperature is 1 second. The callback will
	//    only be invoked if the elapsed time since the last callback is larger
	//    than the polling time. In this example, two counts are shown: a polling
	//    count and a callback count. This is to demonstrate that the callback is
	//    not necessarily invoked every time the node map is polled.
	printf("%sStart polling\n", TAB1);

	for (int i = 0; i < NUM_POLLS; i++)
	{
		printf("\r%s%4i", TAB1, i);
		fflush(stdout);

		// Sleep for elapsed time
#ifdef _WIN32
		Sleep(SLEEP_TIME);
#else
		sleep(SLEEP_TIME);
#endif

		err = acNodeMapPoll(hNodeMap, ELAPSED_TIME);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// stop stream
	printf("\n%sStop stream\n", TAB1);

	err = acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Deregister callback
	//    Failing to deregister a callback results in a memory leak. Once a
	//    callback has been registered, it will no longer be invoked when a node
	//    is invalidated.
	printf("%sDeregister callback\n", TAB1);

	err = acCallbackDeregister(hCallback);
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
	printf("C_Callback_Polling\n");
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
	err = ConfigureCallbackToPollDeviceTemperature(hSystem, hDevice);
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
