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
#include <string.h>	  // defines strcmp functions

#define TAB1 "  "
#define TAB2 "    "

// Acquisition: Rapid Acquisition
//    This example demonstrates configuring device settings in order to reduce
//    bandwidth and increase framerate. This includes reducing the region of
//    interest, reducing bits per pixel, increasing packet size, reducing
//    exposure time, and setting a large number of buffers.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Width and height
#define WIDTH 100
#define HEIGHT 100

// Pixel format
#define PIXEL_FORMAT "Mono8"

// Number of images to grab
#define NUM_IMAGES 500

// Maximum packet size
//    In order to grab images at the maximum packet size, the ethernet adapter
//    must be configured appropriately: 'Jumbo packet' must be set to its
//    maximum, 'UDP checksum offload' must be set to 'Rx & Tx Enabled', and
//    'Received Buffers' must be set to its maximum.
#define MAX_PACKET_SIZE false

// Image timeout
#define IMAGE_TIMEOUT 2000

// System timeout
#define SYSTEM_TIMEOUT 100

// Maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- HELPER =-=-=-
// =-=-=-=-=-=-=-=-=-

// sets node value
// (1) gets node
// (2) check access mode
// (3) gets value
int64_t SetIntValue(acNodeMap hNodeMap, const char* nodeName, int64_t value)
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

	// retrieve the range and increment for the node.
	int64_t minimum = 0;
	int64_t maximum = 0;
	int64_t increment = 0;

	err = acIntegerGetMin(hNode, &minimum);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acIntegerGetMax(hNode, &maximum);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acIntegerGetInc(hNode, &increment);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Ensure increment
	//    If a node has an increment (all integer nodes & some float nodes), only
	//    multiples of the increment can be set. Ensure this by dividing and then
	//    multiplying by the increment. If a value is between two increments,
	//    this will push it to the lower value. Most minimum values are divisible
	//    by the increment. If not, this must also be considered in the
	//    calculation.
	value = (((value - minimum) / increment) * increment) + minimum;

	// Check min/max values
	//    Values must not be less than the minimum or exceed the maximum value of
	//    a node. If a value does so, simply push it within range.
	if (value < minimum)
	{
		value = minimum;
	}
	if (value > maximum)
	{
		value = maximum;
	}

	// set integer value
	err = acIntegerSetValue(hNode, value);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return value for output
	return value;
}

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates configuration for high frame rates
// (1) lowers image size
// (2) maximizes packet size
// (3) minimizes exposure time
// (4) sets high number of buffers
// (5) requeues buffers
AC_ERROR AcquireImagesRapidly(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// retrieve exposure time node to get and set value
	acNode hExposureTimeNode = NULL;

	err = acNodeMapGetNode(hNodeMap, "ExposureTime", &hExposureTimeNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get node values that will be changed in order to return their values at
	// the end of the example
	double exposureTimeInitial = 0;
	char pExposureAutoInitial[MAX_BUF];
	size_t exposureAutoLen = MAX_BUF;
	char pPixelFormatInitial[MAX_BUF];
	size_t pixelFormatLen = MAX_BUF;
	int64_t widthInitial = 0;
	int64_t heightInitial = 0;
	acNode hDeviceStreamChannelPacketSizeNode = NULL;
	int64_t deviceStreamChannelPacketSizeInitial = 0;

	err = acNodeMapGetEnumerationValue(hNodeMap, "ExposureAuto", pExposureAutoInitial, &exposureAutoLen) |
		  acFloatGetValue(hExposureTimeNode, &exposureTimeInitial) |
		  acNodeMapGetEnumerationValue(hNodeMap, "PixelFormat", pPixelFormatInitial, &pixelFormatLen) |
		  acNodeMapGetIntegerValue(hNodeMap, "Width", &widthInitial) |
		  acNodeMapGetIntegerValue(hNodeMap, "Height", &heightInitial);
	if (err != AC_ERR_SUCCESS)
		printf("Error: failed to retrieve one or more initial node values.\n");

	// retrieve DeviceStreamChannelPacketSize value if max packet size defined as
	// true
	if (MAX_PACKET_SIZE)
	{
		// get node
		err = acNodeMapGetNode(hNodeMap, "DeviceStreamChannelPacketSize", &hDeviceStreamChannelPacketSizeNode);
		if (err != AC_ERR_SUCCESS)
			return err;

		// get initial node value
		err = acIntegerGetValue(hDeviceStreamChannelPacketSizeNode, &deviceStreamChannelPacketSizeInitial);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// Set low width and height
	//    Reducing the size of an image reduces the amount of bandwidth required
	//    for each image. The less bandwidth required per image, the more images
	//    can be sent over the same bandwidth.
	printf("%sSet low width and height", TAB1);
	int64_t imageWidth = SetIntValue(hNodeMap, "Width", WIDTH);
	int64_t imageHeight = SetIntValue(hNodeMap, "Height", HEIGHT);

	printf(" (%" PRId64 "x%" PRId64 ")\n", imageWidth, imageHeight);

	// Set small pixel format
	//    Similar to reducing the ROI, reducing the number of bits per pixel also
	//    reduces the bandwidth required for each image. The smallest pixel
	//    formats are 8-bit bayer and 8-bit mono (i.e. BayerRG8 and Mono8).
	printf("%sSet small pixel format (%s)\n", TAB1, PIXEL_FORMAT);

	err = acNodeMapSetEnumerationValue(hNodeMap, "PixelFormat", PIXEL_FORMAT);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set maximum stream channel packet size
	//    Maximizing packet size increases frame rate by reducing the amount of
	//    overhead required between images. This includes both extra
	//    header/trailer data per packet as well as extra time from intra-packet
	//    spacing (the time between packets). In order to grab images at the
	//    maximum packet size, the ethernet adapter must be configured
	//    appropriately: 'Jumbo packet' must be set to its maximum, 'UDP checksum
	//    offload' must be set to 'Rx & Tx Enabled', and 'Received Buffers' must
	//    be set to its maximum.
	if (MAX_PACKET_SIZE)
	{
		// get maximum value
		int64_t maximumChannelPacketSize = 0;

		err = acIntegerGetMax(hDeviceStreamChannelPacketSizeNode, &maximumChannelPacketSize);
		if (err != AC_ERR_SUCCESS)
			return err;

		// get unit
		char pChannelPacketSizeUnitBuf[MAX_BUF];
		size_t channelPacketSizeBufLen = MAX_BUF;

		err = acIntegerGetUnit(hDeviceStreamChannelPacketSizeNode, pChannelPacketSizeUnitBuf, &channelPacketSizeBufLen);
		if (err != AC_ERR_SUCCESS)
			return err;

		// set node to maximum size
		printf("%sSet maximum device stream channel packet size (%" PRIu64 " %s)\n", TAB1, maximumChannelPacketSize, pChannelPacketSizeUnitBuf);

		err = acIntegerSetValue(hDeviceStreamChannelPacketSizeNode, maximumChannelPacketSize);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// Turn off automatic exposure and set low exposure time
	//    Decreasing exposure time can increase frame rate by reducing the amount
	//    of time it takes to grab an image. Reducing the exposure time past
	//    certain thresholds can cause problems related to not having enough
	//    light. In certain situations this can be mitigated by increasing gain
	//    and/or environmental light.
	err = acNodeMapSetEnumerationValue(hNodeMap, "ExposureAuto", "Off");
	if (err != AC_ERR_SUCCESS)
		return err;

	// get minimum exposure time
	double minimumExposureTime = 0;

	err = acFloatGetMin(hExposureTimeNode, &minimumExposureTime);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get exposure time unit
	char pExposureUnitBuf[MAX_BUF];
	size_t exposureBufLen = MAX_BUF;

	err = acFloatGetUnit(hExposureTimeNode, pExposureUnitBuf, &exposureBufLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set to minimum value
	printf("%sSet minimum exposure time (%.0f %s)\n", TAB1, minimumExposureTime, pExposureUnitBuf);

	err = acFloatSetValue(hExposureTimeNode, minimumExposureTime);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Start stream
	//    Start the stream before grabbing any images. Starting the stream
	//    allocates buffers, which can be passed in as an argument (default: 10),
	//    and begins filling them with data. Starting the stream blocks write
	//    access to many features such as width, height, and pixel format, as
	//    well as acquisition and buffer handling modes, among others. The stream
	//    needs to be stopped later.
	printf("%sStart stream with %i buffers\n", TAB1, NUM_IMAGES);

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get images
	for (int i = 1; i <= NUM_IMAGES; i++)
	{
		printf("\r%sGet image %d", TAB2, i);
		fflush(stdout);

		acBuffer hBuffer = NULL;

		// get buffer for each image
		err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;

		// requeue buffer for each image
		err = acDeviceRequeueBuffer(hDevice, hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// stop stream
	printf("\n%sStop stream\n", TAB1);

	err = acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return all nodes to their initial values
	if (0 == strcmp(pExposureAutoInitial, "Off"))
	{
		err = acFloatSetValue(hExposureTimeNode, exposureTimeInitial);
		if (err != AC_ERR_SUCCESS)
			return err;
	}
	err = acNodeMapSetEnumerationValue(hNodeMap, "ExposureAuto", pExposureAutoInitial) |
		  acNodeMapSetEnumerationValue(hNodeMap, "PixelFormat", pPixelFormatInitial) |
		  acNodeMapSetIntegerValue(hNodeMap, "Width", widthInitial) |
		  acNodeMapSetIntegerValue(hNodeMap, "Height", heightInitial);
	if (err != AC_ERR_SUCCESS)
		printf("Error: failed to set one or more node values back to its initial value.\n");

	if (MAX_PACKET_SIZE)
	{
		err = acIntegerSetValue(hDeviceStreamChannelPacketSizeNode, deviceStreamChannelPacketSizeInitial);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

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
	printf("C_Acquisition_RapidAcquisition\n");
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
	err = AcquireImagesRapidly(hDevice);
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
