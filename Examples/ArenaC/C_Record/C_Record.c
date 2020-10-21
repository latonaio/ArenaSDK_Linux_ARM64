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
#include "SaveCApi.h"
#include <inttypes.h> // defines macros for printf functions
#include <stdbool.h>  // defines boolean type and values
#include <string.h>	  // defines strcmp functions
#include <stdlib.h>

#define TAB1 "  "
#define TAB2 "    "
#define ERASE_LINE "\t\t\t\t\t"

// Record: Introduction
//    This example introduces the basics of video recording using the save
//    library. This includes preparing, creating, and writing images to the
//    video.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Memory Limitations
//    This example appends many images to record a video stream and can quickly
//    exceed memory limits when running on a "Win32" platform target. The 32-bit
//    Windows system allocates a total of 2GB of memory for all data types. It is
//    suggested to run this example on an "x64" platform, which has an 8TB memory
//    limit: 2GB of shared data between the stack and static data, and the
//    remainder for dynamic data. If running on a Win32 platform, this example
//    reduces the height and width for each image being saved to the video file.
#define WIDTH 800  // image width
#define HEIGHT 600 // image height

// Frames per second
//    This example chooses its FPS setting arbitrarily. If the device is
//    outputting images at a similar frame rate, it may seem as though the image
//    is configured for real-time video, but this is only incidental. 250 images
//    and 25 frames per second will produce a 10 second video.
#define FRAMES_PER_SECOND 25.0

// number of images to grab
#define NUM_IMAGES 250

// image timeout
#define IMAGE_TIMEOUT 2000

// timeout for detecting camera devices (in milliseconds).
#define SYSTEM_TIMEOUT 100

// File name
//    The relative path and file name to save to. After running the example, a
//    video should exist at the location specified. The image writer chooses the
//    file format by the image's extension. Aside from MPEG-4 (.mp4), videos can
//    be saved as AVI (.avi), MOV (.mov), and raw (.raw) files.
#define FILE_NAME "Images/C_Record/video.mp4"

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

void usage(char* app)
{
	printf("Usage:\n");
	printf("%s [-w width] [-h height]\n", app);
	printf("Where:\n");
	printf("width:     camera image width to configure. Default is %d.\n", WIDTH);
	printf("height:    camera image height to configure. Default is %d.\n", HEIGHT);
	printf("numImages: number of images to capture for recording. Default is %d.\n", NUM_IMAGES);
	printf("fps:       framerate to use for the recording. Default is %.2f.\n", FRAMES_PER_SECOND);
	printf("\n");
}

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
	//    multiples of the increment can be set.
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

// sets node value
// (1) gets node
// (2) check access mode
// (3) gets value
double SetFloatValue(acNodeMap hNodeMap, const char* nodeName, double value)
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

	// retrieve the range for the node.
	double minimum = 0.0;
	double maximum = 0.0;

	err = acFloatGetMin(hNode, &minimum);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acFloatGetMax(hNode, &maximum);
	if (err != AC_ERR_SUCCESS)
		return err;

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
	err = acFloatSetValue(hNode, value);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return value for output
	return value;
}

// demonstrates recording a video
// (1) prepares video parameters
// (2) prepares video recorder
// (3) sets video settings
// (4) opens video
// (5) appends images
// (6) closes video
AC_ERROR RecordVideo(acDevice hDevice, acBuffer* hBuffers, unsigned int numImages, double fps)
{
	SC_ERROR err = SC_ERR_SUCCESS;

	// Get video parameters
	printf("%sGet video parameters", TAB1);
	size_t width = 0;
	size_t height = 0;

	err = acImageGetWidth(hBuffers[0], &width);
	if (err != SC_ERR_SUCCESS)
		return err;

	err = acImageGetHeight(hBuffers[0], &height);
	if (err != SC_ERR_SUCCESS)
		return err;

	printf(" (%zux%zu, %f FPS)\n", width, height, fps);

	// Prepare video recorder
	printf("%sPrepare video recorder\n", TAB1);
	saveRecorder hRecorder = NULL;

	err = saveRecorderCreate(
		width,
		height,
		fps,
		&hRecorder);

	if (err != SC_ERR_SUCCESS)
		return err;

	// Set file name pattern
	printf("%sSet file name pattern to %s", TAB1, FILE_NAME);

	err = saveRecorderSetFileNamePattern(hRecorder, FILE_NAME);
	if (err != SC_ERR_SUCCESS)
		return err;

	// Set codec, container, and pixel format
	printf("%sSet codec to H264, container to MPEG-4, and pixel format to BGR8\n", TAB1);

	err = saveRecorderSetH264Mp4BGR8(hRecorder);
	if (err != SC_ERR_SUCCESS)
		return err;

	// Open video
	printf("%sOpen video\n", TAB1);

	printf("\nFFMPEG OUTPUT---------------\n\n");
	err = saveRecorderOpen(hRecorder);
	if (err != SC_ERR_SUCCESS)
		return err;
	printf("\nFFMPEG OUTPUT---------------\n\n");

	// Append images
	printf("%sAppend images\n", TAB2);

	for (unsigned int i = 0; i < numImages; i++)
	{
		if (i % 25 == 0)
			printf(TAB2);
		printf(".");
		if (i % numImages == numImages - 1)
			printf("\n");
		else if (i % 25 == 24)
			printf("\r%s\r", ERASE_LINE);

		// get data from image
		uint8_t* pData = NULL;

		err = acImageGetData(hBuffers[i], &pData);
		if (err != SC_ERR_SUCCESS)
			return err;

		// append data to video
		err = saveRecorderAppendImage(hRecorder, pData);
		if (err != SC_ERR_SUCCESS)
			return err;
	}

	// Close video
	printf("%sClose video\n", TAB1);

	printf("\nFFMPEG OUTPUT---------------\n\n");
	err = saveRecorderClose(hRecorder);
	printf("\nFFMPEG OUTPUT---------------\n");

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

int main(int argc, char** argv)
{
	printf("C_Record\n");
	AC_ERROR err = AC_ERR_SUCCESS;

	// Parse command line args
	int64_t width = WIDTH;
	int64_t height = HEIGHT;
	unsigned int numImages = NUM_IMAGES;
	double fps = FRAMES_PER_SECOND;

	for (int i = 1; i < argc; i++)
	{
		if ((strcmp(argv[i], "-w") == 0) && (i + 1 < argc))
		{
			width = strtol(argv[++i], NULL, 10);
		}
		else if ((strcmp(argv[i], "-h") == 0) && (i + 1 < argc))
		{
			height = strtol(argv[++i], NULL, 10);
		}
		else if ((strcmp(argv[i], "-n") == 0) && (i + 1 < argc))
		{
			numImages = strtol(argv[++i], NULL, 10);
		}
		else if ((strcmp(argv[i], "-fps") == 0) && (i + 1 < argc))
		{
			fps = strtof(argv[++i], NULL);

			if (fps <= 0)
			{
				printf("Framerate must be greater than 0.\n");
				return -1;
			}
		}
		else if (strcmp(argv[i], "--help") == 0)
		{
			usage(argv[0]);
			return 0;
		}
		else
		{
			printf("Invalid argument [%s]\n", argv[i]);
			usage(argv[0]);
			return -1;
		}
	}

	if (height == 0 || width == 0)
	{
		printf("Invalid with or height specified!\n");
		return -1;
	}

	printf("While the recorder is running, images may be buffered to memory.\n");
	printf("To reduce the chance of problems when running on platforms with lower \n"
		   "performance and/or lower amounts of memory, this example will use a \n"
		   "default resolution of %dx%d\n", WIDTH, HEIGHT);
	printf("The default resolution can be overridden with command line arguments.\n"
		   "Use: %s --help for more info.\n", argv[0]);

	printf("\nProceed with example? ('y' to continue) ");

	char continueExample[MAX_BUF];

	char* result = fgets(continueExample, sizeof continueExample, stdin);
	if (result && (0 != strcmp(continueExample, "y\n")))
	{
		printf("\nPress enter to exit example\n");
		getchar();
		return -1;
	}

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
	else if (numImages == 0)
	{
		printf("numImages should be greater than 0\n");
		return 0;
	}

	acDevice hDevice = NULL;
	err = acSystemCreateDevice(hSystem, 0, &hDevice);
	CHECK_RETURN;

	acNodeMap hNodeMap = NULL;
	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	CHECK_RETURN;

	// Store initial settings
	char acquisitionModeInitial[MAX_BUF];
	bool frameRateEnableInitial = true;
	double frameRateInitial = 0.0;
	int64_t imageWidthInitial = 0;
	int64_t imageHeightInitial = 0;

	size_t acquisitionModeInitialLen = MAX_BUF;

	// Store initial camera settings
	err = // Acquisition mode
		acNodeMapGetEnumerationValue(hNodeMap, "AcquisitionMode", acquisitionModeInitial, &acquisitionModeInitialLen) |
		// Framerate enable
		acNodeMapGetBooleanValue(hNodeMap, "AcquisitionFrameRateEnable", &frameRateEnableInitial) |
		// Image width
		acNodeMapGetIntegerValue(hNodeMap, "Width", &imageWidthInitial) |
		// Image height
		acNodeMapGetIntegerValue(hNodeMap, "Heignt", &imageHeightInitial);

	if (frameRateEnableInitial)
	{
		err |= acNodeMapGetFloatValue(hNodeMap, "AcquisitionFrameRate", &frameRateInitial);
	}

	if (err != AC_ERR_SUCCESS)
		printf("\nWarning: failed to retrieve one or more initial node values.\n");


	// Set acquisition mode
	err = acNodeMapSetEnumerationValue(hNodeMap, "AcquisitionMode", "Continuous");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set width and height
	//    Reducing the size of an image reduces the amount of bandwidth
	//    required for each image. The less bandwidth required per image, the
	//    more images can be sent over the same bandwidth.
	width = SetIntValue(hNodeMap, "Width", width);
	height = SetIntValue(hNodeMap, "Height", height);

	// Set framerate
	err = acNodeMapSetBooleanValue(hNodeMap, "AcquisitionFrameRateEnable", true);
	if (err != AC_ERR_SUCCESS)
		return err;

	fps = SetFloatValue(hNodeMap, "AcquisitionFrameRate", fps);

	printf("Using: \nwidth: %" PRId64 "\nheight: %" PRId64 "\nnumImages: %d\nfps: %.2f\n", width, height, numImages, fps);

	acBuffer* hBuffers = (acBuffer*)malloc(numImages * sizeof(acBuffer));

	err = acDeviceStartStream(hDevice);
	CHECK_RETURN;

	printf("Capturing images\n");
	for (unsigned int i = 0; i < numImages; i++)
	{
		if (i % 25 == 0)
			printf(TAB2);
		printf(".");
		if (i % numImages == numImages - 1)
			printf("\n");
		else if (i % 25 == 24)
			printf("\r%s\r", ERASE_LINE);
		acBuffer hBuffer = NULL;
		err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
		CHECK_RETURN;
		err = acImageFactoryConvert(hBuffer, 0x02180015, &hBuffers[i]); // integer value of BGR8 according to PFNC
		CHECK_RETURN;
		err = acDeviceRequeueBuffer(hDevice, hBuffer);
		CHECK_RETURN;
	}
	err = acDeviceStopStream(hDevice);
	CHECK_RETURN;
	// run example
	printf("Commence example\n\n");
	err = RecordVideo(hDevice, hBuffers, numImages, fps);
	CHECK_RETURN;
	printf("\nExample complete\n");

	// clean up example
	for (unsigned int i = 0; i < numImages; i++)
	{
		err = acImageFactoryDestroy(hBuffers[i]);
		CHECK_RETURN;
	}

	free(hBuffers);

	// Restore initial settings

	// Restore width and height
	SetIntValue(hNodeMap, "Width", imageWidthInitial);
	SetIntValue(hNodeMap, "Height", imageHeightInitial);

	// Restore acquisition mode
	err = acNodeMapSetEnumerationValue(hNodeMap, "AcquisitionMode", acquisitionModeInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set framerate
	err = acNodeMapSetBooleanValue(hNodeMap, "AcquisitionFrameRateEnable", frameRateEnableInitial);
	if (err != AC_ERR_SUCCESS)
		return err; 

	if (frameRateEnableInitial)
	{
		SetFloatValue(hNodeMap, "AcquisitionFrameRate", frameRateInitial);
	}

	err = acSystemDestroyDevice(hSystem, hDevice);
	CHECK_RETURN;
	err = acCloseSystem(hSystem);
	CHECK_RETURN;

	printf("Press enter key to complete\n");
	getchar();
	return -1;
}
