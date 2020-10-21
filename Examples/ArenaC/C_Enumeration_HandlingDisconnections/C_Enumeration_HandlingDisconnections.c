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
#include <stdbool.h>  // defines boolean type and values
#include <inttypes.h> // defines macros for printf functions

// Multithreading is implemented differently for Windows and Linux
// systems, so headers, functions and macros are defined according to the
// operating system being used.
#ifdef _WIN32
#include "ArenaCThreadWindows.h"
#elif defined linux
#include "ArenaCThreadLinux.h"
#endif

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "
#define TAB4 "        "

// Enumeration: Handling Disconnections
//    This example demonstrates a multi-threaded approach to handling device
//    First, the acquisition thread is responsible for acquiring images when the
//    disconnections. It spawns two threads, each with a different
//    responsibility. device is connected. Second, the enumeration thread handles
//    disconnections by reconnecting the device and notifying the acquisition
//    thread.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// image timeout
#define IMAGE_TIMEOUT 2000

// update timeout
#define SYSTEM_TIMEOUT 100

// maximum number of images
#define MAX_IMAGES 500

// maximum buffer length
#define MAX_BUF 256

// Global settings
// This example uses global scope for variables and information shared
// between threads. This is not a best practice but is done this way for the sake
// of simplicity.

THREAD_LOCK_VARIABLE(g_deviceMutex)

THREAD_CONDITION_VARIABLE(g_deviceConnected)

// arenaC
acDevice g_pDevice = NULL;

// other
static bool g_isRunning = false;

// =-=-=-=-=-=-=-=-=-
// =-=- HELPER =-=-=-
// =-=-=-=-=-=-=-=-=-

// return error
// (1) exit current thread
// (2) notify other thread to exit
// (3) return error
AC_ERROR exitThreads(AC_ERROR err)
{
	g_isRunning = false;
	acThreadConditionVariableWake(&g_deviceConnected);
	return err;
}

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// reconnects a device when disconnected
// (1) on disconnection, activate
// (2) search for device
// (3) reconnect device
// (4) notify all on reconnection
// (5) notify all on exit

THREAD_FUNCTION_SIGNATURE(EnumerationThread)
{
	AC_ERROR err = AC_ERR_SUCCESS;
	acSystem hSystem = (acSystem*)lpParam;

	// grab subnet upon entering thread
	uint32_t subnetMask = 0;

	err = acSystemGetDeviceSubnetMask(hSystem, 0, &subnetMask);
	if (err != AC_ERR_SUCCESS)
		exitThreads(err);

	while (g_isRunning)
	{

		// Search for device
		//    When the device has been disconnected, this thread waits for it
		//    to reconnect, constantly updating and scanning the device list
		//    for the lost device.
		err = acSystemUpdateDevices(hSystem, SYSTEM_TIMEOUT);
		if (err == AC_ERR_SUCCESS)
		{
			err = acSystemCreateDevice(hSystem, 0, &g_pDevice);
			if (err == AC_ERR_SUCCESS)
			{
				// Recreate device and notify other thread
				//    Once the device has been found, recreate it and notify
				//    the acquisition thread that it can stop waiting and
				//    continue acquiring images.
				printf("%sDevice reconnected\n", TAB4);

				// Ensure appropriate network settings
				//    Check that the device is on the same subnet after
				//    reconnecting the camera. If the camera/adapter are on a
				//    non 169.254.*.* subnet but not using a persistent IP or
				//    DHCP, the camera will automatically be assigned an LLA
				//    and pick a 169.254.*.* IP/subnet after reconnecting,
				//    causing the example to exit. There are several ways to
				//    fix this issue: (1) by setting a static IP to the
				//    ethernet port, (2) forcing an IP address whenever the
				//    device is updated (see C_ForceIP), (3) running
				//    ArenaConfig to configure the adapter settings, or (4)
				//    setting up a persistent IP on the camera using
				//    IPConfigUtility
				uint32_t subnetMaskReconnect = 0;

				err = acSystemGetDeviceSubnetMask(hSystem, 0, &subnetMaskReconnect);
				if (err != AC_ERR_SUCCESS)
					exitThreads(err);

				if (subnetMask != subnetMaskReconnect)
				{

					printf("\n%sError: Subnet has changed upon reconnecting\n", TAB2);
					printf("%sSubnet at example start:   %" PRIu32 "\n", TAB3, subnetMask);
					printf("%sSubnet after reconnection: %" PRIu32 "\n", TAB3, subnetMaskReconnect);
					printf("\n%sPress enter to exit example\n", TAB1);

					// exit thread and notify other thread to exit
					exitThreads(err);
				}

				err = acDeviceStartStream(g_pDevice);
				if (err != AC_ERR_SUCCESS)
					exitThreads(err);

				acThreadConditionVariableWake(&g_deviceConnected);
			}
		}
	}

	// Notify other thread on exit
	//    If the device is disconnected at the time of exit, the other thread
	//    will be waiting for reconnection. Sending this notification allows the
	//    other thread to stop waiting and continue to exit.
	printf("%sNotify other thread on exit\n", TAB3);

	acThreadConditionVariableWake(&g_deviceConnected);

	THREAD_RETURN(err);
}

// acquires images while device is connected
// (1) starts stream
// (2) retrieves images
// (3) catches disconnections, destroying device appropriately
// (4) waits for reconnection
// (5) restarts stream, continuing to retrieve images
// (6) stops stream
THREAD_FUNCTION_SIGNATURE(AcquisitionThread)
{
	AC_ERROR err = AC_ERR_SUCCESS;
	acSystem hSystem = (acSystem*)lpParam;

	// start stream
	int numImages = 0;
	err = acDeviceStartStream(g_pDevice);
	if (err != AC_ERR_SUCCESS)
		exitThreads(err);

	// Get images while connected
	//    While the device is connected and streaming, grab images. This
	//    example just counts the images and requeues their buffers; however,
	//    using the image factory to copy or convert the images for display
	//    or processing might be a useful addition.
	while (g_isRunning && numImages < MAX_IMAGES)
	{
		printf("\r%sGet image %i", TAB3, numImages);
		fflush(stdout);
		acBuffer hBuffer = NULL;

		err = acDeviceGetBuffer(g_pDevice, IMAGE_TIMEOUT, &hBuffer);
		if (err != AC_ERR_SUCCESS)
		{
			// Catch disconnection
			//    Disconnections will most likely show themselves as
			//    read/write timeouts. This is caused as the host attempts to
			//    signal the device, but the device doesn't respond, timing
			//    out.
			printf("\n%sDevice disconnected\n", TAB4);

			err = acSystemDestroyDevice(hSystem, g_pDevice);
			if (err != AC_ERR_SUCCESS)
				exitThreads(err);

			// Lock access across threads
			//    Use a lock to protect access to shared resources, ensuring
			//    that simultaneous writes/reads across threads don't clobber
			//    one another.
			acThreadLock(&g_deviceMutex);
			g_pDevice = NULL;

			// check that exit condition has not been called while inside the
			// loop
			if (g_isRunning)
			{
				// Wait for reconnection if necessary
				//    When the device has been disconnected, the acquisition
				//    thread waits on the enumeration thread to reconnect the
				//    device. This is done through a conditional variable.
				//    This conditional variable has also been set up to stop
				//    waiting if the application has been terminated.
				acThreadConditionVariableSleep(&g_deviceConnected, &g_deviceMutex);
			}

			acThreadUnlock(&g_deviceMutex);
		}
		else
		{
			// increment image count and requeue buffer
			numImages = numImages + 1;

			err = acDeviceRequeueBuffer(g_pDevice, hBuffer);
			if (err != AC_ERR_SUCCESS)
				exitThreads(err);

			if (numImages >= MAX_IMAGES)
			{
				printf("\n%sAcquisition completed, press enter to continue\n", TAB3);
				g_isRunning = false;
			}
		}
	};

	// stop stream
	if (g_pDevice)
	{
		err = acDeviceStopStream(g_pDevice);
		if (err != AC_ERR_SUCCESS)
			exitThreads(err);
	}

	THREAD_RETURN(err);
}

// run example
// (1) create acquisition and enumeration threads
// (2) stop acquisition on key press
// (3) wait for threads to complete and close handles
AC_ERROR RunExample(acSystem hSystem)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// initialize lock and condition variable
	acThreadLockInitialize(&g_deviceMutex);
	acThreadConditionVariableInitialize(&g_deviceConnected);

	// Start acquisition and enumeration threads and wait for key
	//    Spawn a thread for acquisition and a thread for enumeration. Keeping
	//    this kind of work off the main thread allows for multiple things
	//    happening simultaneously in the case of more complex applications.
	printf("%sStart acquisition and enumeration threads\n", TAB1);
	g_isRunning = true;

	THREAD_ID(hChildAcquisitionThread);
	THREAD_ID(hChildEnumerationThread);

	acThreadCreate(AcquisitionThread, hSystem, &hChildAcquisitionThread);
	acThreadCreate(EnumerationThread, hSystem, &hChildEnumerationThread);

	// Stop acquisition on key press
	//    Because the main thread pushed all the work into the worker threads, it
	//    is free to wait for user input to end the application.
	printf("%sPress enter to stop acquisition\n", TAB2);
	getchar();

	// Wait for threads to complete and close handles
	//    When user input is received, the application must send a signal to the
	//    worker threads that this has happened. This is accomplished with a
	//    global boolean. The application should then wait for each thread to
	//    complete before continuing its shutdown.
	g_isRunning = false;

	acThreadDestroy(hChildEnumerationThread);
	acThreadDestroy(hChildAcquisitionThread);

	// deinitialize condition variable and thread lock and destroying all threads
	acThreadLockDeinitialize(&g_deviceMutex);
	acThreadConditionVariableDeinitialize(&g_deviceConnected);

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
	printf("C_Enumeration_HandlingDisconnections\n");
	printf("Please manually disconnect and reconnect device as device acquires images\n");
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
		printf("Waiting for a device...\n");
		while (true)
		{
			err = acSystemUpdateDevices(hSystem, SYSTEM_TIMEOUT);
			err = acSystemGetNumDevices(hSystem, &numDevices);
			if (numDevices > 0)
				break;
		}
	}

	err = acSystemCreateDevice(hSystem, 0, &g_pDevice);
	CHECK_RETURN;

	// run example
	printf("Commence example\n\n");
	err = RunExample(hSystem);
	CHECK_RETURN;
	printf("\nExample complete\n");

	// clean up example
	if (g_pDevice)
	{
		err = acSystemDestroyDevice(hSystem, g_pDevice);
		CHECK_RETURN;
	}
	err = acCloseSystem(hSystem);
	CHECK_RETURN;

	printf("Press enter to complete\n");
	getchar();
	return -1;
}
