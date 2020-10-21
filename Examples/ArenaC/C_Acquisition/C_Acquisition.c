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

#define TAB1 "  "
#define TAB2 "    "

// Acquisition: Introduction
//    This example introduces the basics of image acquisition. This includes
//    setting image acquisition and buffer handling modes, setting the device to
//    automatically negotiate packet size, and setting the stream packet resend
//    node before starting the image stream. The example then starts acquiring
//    images by grabbing and requeuing buffers, and retrieving data on images
//    before finally stopping the image stream.

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
#define NUM_IMAGES 25

// maximum buffer length
#define MAX_BUF 256

// timeout for detecting camera devices (in milliseconds).
#define SYSTEM_TIMEOUT 100

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
// (1) sets acquisition mode
// (2) sets buffer handling mode
// (3) enables auto negotiate packet size
// (4) enables packet resend
// (5) starts the stream
// (6) gets a number of images
// (7) prints information from images
// (8) requeues buffers
// (9) stops the stream
AC_ERROR AcquireImages(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get node values that will be changed in order to return their values at
	// the end of the example
	char pAcquisitionModeInitial[MAX_BUF];
	size_t len = MAX_BUF;

	err = GetNodeValue(hNodeMap, "AcquisitionMode", pAcquisitionModeInitial, &len);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set acquisition mode
	//    Set acquisition mode before starting the stream. Starting the stream
	//    requires the acquisition mode to be set beforehand. The acquisition
	//    mode controls the number of images a device acquires once the stream
	//    has been started. Setting the acquisition mode to 'Continuous' keeps
	//    the stream from stopping. This example returns the camera to its
	//    initial acquisition mode near the end of the example.
	printf("%sSet acquisition mode to 'Continuous'\n", TAB1);

	err = SetNodeValue(
		hNodeMap,
		"AcquisitionMode",
		"Continuous");

	if (err != AC_ERR_SUCCESS)
		return err;

	// Set buffer handling mode
	//    Set buffer handling mode before starting the stream. Starting the
	//    stream requires the buffer handling mode to be set beforehand. The
	//    buffer handling mode determines the order and behavior of buffers in
	//    the underlying stream engine. Setting the buffer handling mode to
	//    'NewestOnly' ensures the most recent image is delivered, even if it
	//    means skipping frames.
	printf("%sSet buffer handling mode to 'NewestOnly'\n", TAB1);

	// get stream node map
	acNodeMap hTLStreamNodeMap = NULL;

	err = acDeviceGetTLStreamNodeMap(hDevice, &hTLStreamNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = SetNodeValue(
		hTLStreamNodeMap,
		"StreamBufferHandlingMode",
		"NewestOnly");

	// Enable stream auto negotiate packet size
	//    Setting the stream packet size is done before starting the stream.
	//    Setting the stream to automatically negotiate packet size instructs the
	//    camera to receive the largest packet size that the system will allow.
	//    This generally increases frame rate and results in fewer interrupts per
	//    image, thereby reducing CPU load on the host system. Ethernet settings
	//    may also be manually changed to allow for a larger packet size.
	printf("%sEnable stream to auto negotiate packet size\n", TAB1);

	err = acNodeMapSetBooleanValue(hTLStreamNodeMap, "StreamAutoNegotiatePacketSize", true);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Enable stream packet resend
	//    Enable stream packet resend before starting the stream. Images are sent
	//    from the camera to the host in packets using UDP protocol, which
	//    includes a header image number, packet number, and timestamp
	//    information. If a packet is missed while receiving an image, a packet
	//    resend is requested and this information is used to retrieve and
	//    redeliver the missing packet in the correct order.
	printf("%sEnable stream packet resend\n", TAB1);

	err = acNodeMapSetBooleanValue(hTLStreamNodeMap, "StreamPacketResendEnable", true);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Start stream
	//    Start the stream before grabbing any images. Starting the stream
	//    allocates buffers, which can be passed in as an argument (default: 10),
	//    and begins filling them with data. Starting the stream blocks write
	//    access to many features such as width, height, and pixel format, as
	//    well as acquisition and buffer handling modes, among others. The stream
	//    needs to be stopped later.
	printf("%sStart stream\n", TAB1);

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get images
	printf("%sGetting %d images\n", TAB1, NUM_IMAGES);

	int i = 0;
	for (i = 0; i < NUM_IMAGES; i++)
	{
		// Get image
		//    Retrieve images after the stream has started. If the timeout
		//    expires before an image is retrieved, example will throw. Because
		//    of this, the timeout should be at least a bit larger than the
		//    exposure time.
		printf("%sGet image %d", TAB2, i);
		acBuffer hBuffer = NULL;

		err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;

		// Get image information
		//    Images have information available from them beyond the basics
		//    including information related to data type, image parameters, and
		//    error handling.
		printf(" (");

		// get and display size filled
		size_t sizeFilled = 0;

		err = acBufferGetSizeFilled(hBuffer, &sizeFilled);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%zu bytes; ", sizeFilled);

		// get and display width
		size_t width = 0;

		err = acImageGetWidth(hBuffer, &width);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%zux", width);

		// get and display height
		size_t height = 0;

		err = acImageGetHeight(hBuffer, &height);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%zu; ", height);

		// get and display timestamp
		uint64_t timestampNs = 0;

		err = acImageGetTimestampNs(hBuffer, &timestampNs);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("timestamp (ns): %" PRIu64 ")", timestampNs);

		// Requeue image buffer
		//    Requeue an image buffer when access to it is no longer needed.
		//    Notice that failing to requeue buffers may cause memory to leak and
		//    may also result in the stream engine being starved due to there
		//    being no available buffers.
		printf(" and requeue\n");

		err = acDeviceRequeueBuffer(hDevice, hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// Stop stream
	//    Stop the stream after all images have been requeued. Failing to stop
	//    the stream will leak memory.
	printf("%sStop stream\n", TAB1);

	err = acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return nodes to their initial values
	err = SetNodeValue(
		hNodeMap,
		"AcquisitionMode",
		pAcquisitionModeInitial);

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
	printf("C_Acquisition\n");
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
	err = AcquireImages(hDevice);
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
