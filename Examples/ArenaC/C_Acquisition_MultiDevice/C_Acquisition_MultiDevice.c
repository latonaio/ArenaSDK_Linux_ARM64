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
#include <inttypes.h> // defines macros for printf functions
#include <stdbool.h>  // defines boolean type and values
#include <string.h>   // defins function : strcpy and strcat strcpy_s and strcat_s
#include <stdlib.h>   // defins function : malloc

// Multithreading is implemented differently for Windows and Linux
// systems, so headers, functions and macros are defined according to the
// operating system being used.
#ifdef _WIN32
#include "ArenaCThreadWindows.h"
#define strcpy_portable(destination, size, source) strcpy_s(destination, size, source)
#define strcat_portable(destination, size, source) strcat_s(destination, size, source)
#elif defined linux
#include "ArenaCThreadLinux.h"
// the safe version of this function is not defined in gcc
#define strcpy_portable(destination, size, source) strcpy(destination, source)
#define strcat_portable(destination, size, source) strcat(destination, source)
#endif

#define TAB1 "  "
#define TAB2 "    "

// Acquisition: Multi-Device
//    This example introduces the basics of image acquisition for multiple devices
//    and creating multiple threads. This includes creating all discovered
//    devices, creating a thread for each device to acquire images. The thread
//    then starts acquiring images by grabbing and requeuing buffers, before
//    finally stopping the image stream.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Image timeout
//    Timeout for grabbing images (in milliseconds). If no image is available at
//    the end of the timeout, an exception is thrown. The timeout is the maximum
//    time to wait for an image; however, getting an image will return as soon as
//    an image is available, not waiting the full extent of the timeout.
#define IMAGE_TIMEOUT 2000

// number of images to grab
#define NUM_IMAGES 250

// maximum buffer length
#define MAX_BUF 256

// timeout for detecting camera devices (in milliseconds).
#define SYSTEM_TIMEOUT 100

THREAD_LOCK_VARIABLE(g_printf_mtx);

// =-=-=-=-=-=-=-=-=-
// =-=- HELPER =-=-=-
// =-=-=-=-=-=-=-=-=-

// gets node value
// (1) gets node
// (2) check access mode
// (3) get value
AC_ERROR GetNodeValue(acNodeMap hNodeMap, const char* nodeName, char* pValue, size_t* pLen)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node
	acNode hNode = NULL;
	AC_ACCESS_MODE accessMode = 0;

	err = acNodeMapGetNodeAndAccessMode(hNodeMap, nodeName, &hNode, &accessMode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// check access mode
	if (accessMode != AC_ACCESS_MODE_RO && accessMode != AC_ACCESS_MODE_RW)
		return AC_ERR_ERROR;

	// get value
	err = acValueToString(hNode, pValue, pLen);
	return err;
}

// sets node value
// (1) gets node
// (2) check access mode
// (3) gets value
AC_ERROR SetNodeValue(acNodeMap hNodeMap, const char* nodeName, const char* pValue)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node
	acNode hNode = NULL;
	AC_ACCESS_MODE accessMode = 0;

	err = acNodeMapGetNodeAndAccessMode(hNodeMap, nodeName, &hNode, &accessMode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// check access mode
	if (accessMode != AC_ACCESS_MODE_WO && accessMode != AC_ACCESS_MODE_RW)
		return AC_ERR_ERROR;

	// get value
	err = acValueFromString(hNode, pValue);
	return err;
}

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates acquisition
// (1) starts the stream
// (2) gets a number of images
// (3) requeues buffers
// (4) stops the stream
THREAD_FUNCTION_SIGNATURE(AcquireImages)
{
	AC_ERROR err = AC_ERR_SUCCESS;
	acDevice hDevice = (acDevice*)lpParam;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);

	// get node values that will be changed in order to return their values at
	// the end of the example
	char pAcquisitionModeInitial[MAX_BUF];
	size_t len = MAX_BUF;

	err = GetNodeValue(hNodeMap, "AcquisitionMode", pAcquisitionModeInitial, &len);
	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);

	// Set acquisition mode
	//    Set acquisition mode before starting the stream. Starting the stream
	//    requires the acquisition mode to be set beforehand. The acquisition
	//    mode controls the number of images a device acquires once the stream
	//    has been started. Setting the acquisition mode to 'Continuous' keeps
	//    the stream from stopping.
	err = SetNodeValue(
		hNodeMap,
		"AcquisitionMode",
		"Continuous");

	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);
	
	// get TLStreamNodeMap
	acNodeMap hTLStreamNodeMap = NULL;
	err = acDeviceGetTLStreamNodeMap(hDevice, &hTLStreamNodeMap);

	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);

	// Enable stream auto negotiate packet size
	//    Setting the stream packet size is done before starting the stream.
	//    Setting the stream to automatically negotiate packet size instructs the
	//    camera to receive the largest packet size that the system will allow.
	//    This generally increases frame rate and results in fewer interrupts per
	//    image, thereby reducing CPU load on the host system. Ethernet settings
	//    may also be manually changed to allow for a larger packet size.
	err = acNodeMapSetBooleanValue(hTLStreamNodeMap, "StreamAutoNegotiatePacketSize", true);
	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);

	// Enable stream packet resend
	//    Enable stream packet resend before starting the stream. Images are sent
	//    from the camera to the host in packets using UDP protocol, which
	//    includes a header image number, packet number, and timestamp
	//    information. If a packet is missed while receiving an image, a packet
	//    resend is requested and this information is used to retrieve and
	//    redeliver the missing packet in the correct order.
	err = acNodeMapSetBooleanValue(hTLStreamNodeMap, "StreamPacketResendEnable", true);
	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);

	// get device model for thread info
	char pDeviceModel[MAX_BUF];
	size_t pDeviceModelLen = MAX_BUF;
	err = acNodeMapGetStringValue(hNodeMap, "DeviceModelName", pDeviceModel, &pDeviceModelLen);
	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);

	// get device serial for thead info
	char pDeviceSerial[MAX_BUF];
	size_t pDeviceSerialLen = MAX_BUF;
	err = acNodeMapGetStringValue(hNodeMap, "DeviceSerialNumber", pDeviceSerial, &pDeviceSerialLen);
	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);

	// combine device model and serial as thread Info
	char threadInfo[MAX_BUF];
	strcpy_portable(threadInfo, MAX_BUF, pDeviceModel);
	strcat_portable(threadInfo, MAX_BUF, pDeviceSerial);
	strcat_portable(threadInfo, MAX_BUF, " | ");

	// Start stream
	//    Start the stream before grabbing any images. Starting the stream
	//    allocates buffers, which can be passed in as an argument (default: 10),
	//    and begins filling them with data. Starting the stream blocks write
	//    access to many features such as width, height, and pixel format, as
	//    well as acquisition and buffer handling modes, among others. The stream
	//    needs to be stopped later.
	acThreadLock(&g_printf_mtx);
	printf("%s%sStart stream\n", threadInfo, TAB1);
	acThreadUnlock(&g_printf_mtx);

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);

	// get images
	acThreadLock(&g_printf_mtx);
	printf("%s%sGetting %d images\n", threadInfo, TAB1, NUM_IMAGES);
	acThreadUnlock(&g_printf_mtx);

	for (int i = 0; i < NUM_IMAGES; i++)
	{
		// Get image
		//    Retrieve images after the stream has started. If the timeout
		//    expires before an image is retrieved, example will throw. Because
		//    of this, the timeout should be at least a bit larger than the
		//    exposure time.
		acThreadLock(&g_printf_mtx);
		printf("%s%sGetting image %d\n", threadInfo, TAB2, i);
		acThreadUnlock(&g_printf_mtx);
		acBuffer hBuffer = NULL;

		err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
		if (err != AC_ERR_SUCCESS)
			THREAD_RETURN(err);

		// Requeue image buffer
		//    Requeue an image buffer when access to it is no longer needed.
		//    Notice that failing to requeue buffers may cause memory to leak and
		//    may also result in the stream engine being starved due to there
		//    being no available buffers.

		err = acDeviceRequeueBuffer(hDevice, hBuffer);
		if (err != AC_ERR_SUCCESS)
			THREAD_RETURN(err);

		acThreadLock(&g_printf_mtx);
		printf("%s%sImage %d received and requeue\n", threadInfo, TAB2, i);
		acThreadUnlock(&g_printf_mtx);
	}

	// Stop stream
	//    Stop the stream after all images have been requeued. Failing to stop
	//    the stream will leak memory.
	acThreadLock(&g_printf_mtx);
	printf("%s%sStop stream\n", threadInfo, TAB1);
	acThreadUnlock(&g_printf_mtx);

	err = acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		THREAD_RETURN(err);

	THREAD_RETURN(err);
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
//acSystemDestroyDevice(hSystem, devicesArray[index]);
#define CHECK_RETURN_AND_FREE_ON_FAILURE                        \
	if (err != AC_ERR_SUCCESS)                                  \
	{                                                           \
		char pMessageBuf[ERR_BUF];                              \
		size_t pBufLen = ERR_BUF;                               \
		acGetLastErrorMessage(pMessageBuf, &pBufLen);           \
		if (devicesArray)                                       \
		{                                                       \
			free(devicesArray);                                 \
		}                                                       \
		if (threadsArray)                                       \
		{                                                       \
			for (size_t index = 0; index < numDevices; index++) \
			{                                                   \
				acThreadDestroy(threadsArray[index]);           \
			}                                                   \
			free(threadsArray);                                 \
		}                                                       \
		printf("\nError: %s", pMessageBuf);                     \
		printf("\n\nPress enter to complete\n");                \
		getchar();                                              \
		return -1;                                              \
	}

int main()
{
	printf("C_Acquisition_MultiDevice\n");
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
	if (numDevices < 2)
	{
		printf("\nThis example is recommended to run with more than one device to demonstrate the multithreading clearly\n");
	}

	// dynamically allocated arrays
	acDevice* devicesArray = NULL;
	THREAD_ID* threadsArray = NULL;
	// array of discovered devices
	devicesArray = (acDevice*)malloc(numDevices * sizeof(acDevice*));
	if (!devicesArray)
	{
		err = AC_ERR_OUT_OF_MEMORY;
		CHECK_RETURN_AND_FREE_ON_FAILURE;
	}
	// create devices and add them to the array
	for (size_t index = 0; index < numDevices; index++)
	{
		acDevice hDevice = NULL;
		err = acSystemCreateDevice(hSystem, index, &hDevice);
		CHECK_RETURN_AND_FREE_ON_FAILURE;
		devicesArray[index] = hDevice;
	}

	// run example
	printf("Commence example\n\n");

	// initialize printf mutex
	acThreadLockInitialize(&g_printf_mtx);

	// array of threads
	threadsArray = (THREAD_ID*)malloc(numDevices * sizeof(THREAD_ID*));
	if (!threadsArray)
	{
		err = AC_ERR_OUT_OF_MEMORY;
		CHECK_RETURN_AND_FREE_ON_FAILURE;
	}
	// start acquiring images for each device on a separate thread
	for (size_t index = 0; index < numDevices; index++)
	{
		// create a thread handle
		THREAD_ID hThreadHandle;

		// start a thread
		acThreadCreate(AcquireImages, devicesArray[index], &hThreadHandle);

		// add to array to destory later
		threadsArray[index] = hThreadHandle;
	}

	// join / destory thread using handle
	for (size_t index = 0; index < numDevices; index++)
	{
		// wait for threads to complete and close threads
		acThreadDestroy(threadsArray[index]);
	}

	// deinitialize / destroy the printf mutex
	acThreadLockDeinitialize(&g_printf_mtx);

	printf("\nExample complete\n");

	// clean up example
	for (size_t index = 0; index < numDevices; index++)
	{
		err = acSystemDestroyDevice(hSystem, devicesArray[index]);
		CHECK_RETURN_AND_FREE_ON_FAILURE;
	}

	// free allocated arrays
	free(threadsArray);
	threadsArray = NULL;
	free(devicesArray);
	devicesArray = NULL;

	err = acCloseSystem(hSystem);
	CHECK_RETURN;

	printf("Press enter to complete\n");
	getchar();
	return -1;
}
