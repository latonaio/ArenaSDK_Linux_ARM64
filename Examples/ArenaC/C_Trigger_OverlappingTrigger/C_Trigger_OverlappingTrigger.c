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
#include <string.h>	  // defines strcmp functions
#include <stdbool.h>  // defines boolean types

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Trigger: Overlapping Trigger on Exposure End Events
//    This example demonstrates the use of trigger and exposure end events to
//    minimize the amount of unused exposure time between images. This is done by
//    setting the device to start exposing (or trigger) right when the last
//    exposure has just finished (or exposure end event). After receiving each
//    exposure end event notification, the next trigger is executed to acquire
//    the next image. Once all triggers have executed, the images are retrieved
//    from the output queue. This example shows that there is little time between
//    triggering images, and that the exposure time is close to the time between
//    triggers.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Number of images to grab
#define NUM_IMAGES 10

// Event timeout
#define EVENT_TIMEOUT 3000

// image timeout
#define IMAGE_TIMEOUT 2000

// system timeout
#define SYSTEM_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates basic trigger configuration and use
// (1) sets trigger mode, source, and selector
// (2) sets acquisition mode, event selector, and turns on event notifications
// (3) starts stream
// (4) waits on trigger armed, executes trigger, and waits on exposureEnd event for all images
// (5) grabs each image and requeues image buffer for all images
// (6) stops stream
AC_ERROR OverlapTriggerOnExposureEndEvent(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Initialize events
	//    Events nodes aren't available until the events engine has been
	//    initialized.
	err = acDeviceInitializeEvents(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get node map and retrieve initial node values that will be changed in
	// order to return their values at the end of the example
	acNodeMap hNodeMap = NULL;
	char triggerSelectorInitial[MAX_BUF];
	size_t triggerSelectorBufLen = MAX_BUF;
	char triggerModeInitial[MAX_BUF];
	size_t triggerModeBufLen = MAX_BUF;
	char triggerSourceInitial[MAX_BUF];
	size_t triggerSourceBufLen = MAX_BUF;
	char acquisitionModeInitial[MAX_BUF];
	size_t acquisitionModeBufLen = MAX_BUF;
	char eventSelectorInitial[MAX_BUF];
	size_t eventSelectorBufLen = MAX_BUF;
	char eventNotificationInitial[MAX_BUF];
	size_t eventNotificationBufLen = MAX_BUF;
	char exposureAutoInitial[MAX_BUF];
	size_t exposureAutoBufLen = MAX_BUF;
	char triggerOverlapInitial[MAX_BUF];
	size_t triggerOverlapBufLen = MAX_BUF;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapGetEnumerationValue(hNodeMap, "TriggerSelector", triggerSelectorInitial, &triggerSelectorBufLen) | 
		  acNodeMapGetEnumerationValue(hNodeMap, "TriggerMode", triggerModeInitial, &triggerModeBufLen) |
		  acNodeMapGetEnumerationValue(hNodeMap, "TriggerSource", triggerSourceInitial, &triggerSourceBufLen) |
		  acNodeMapGetEnumerationValue(hNodeMap, "TriggerOverlap", triggerOverlapInitial, &triggerOverlapBufLen) |
		  acNodeMapGetEnumerationValue(hNodeMap, "AcquisitionMode", acquisitionModeInitial, &acquisitionModeBufLen) |
		  acNodeMapGetEnumerationValue(hNodeMap, "EventSelector", eventSelectorInitial, &eventSelectorBufLen) |
		  acNodeMapGetEnumerationValue(hNodeMap, "EventNotification", eventNotificationInitial, &eventNotificationBufLen) |
		  acNodeMapGetEnumerationValue(hNodeMap, "ExposureAuto", exposureAutoInitial, &exposureAutoBufLen);

	if (err != AC_ERR_SUCCESS)
		printf("\nWarning: failed to retrieve one or more initial node values.\n");

	// Set trigger selector
	//    Set the trigger selector to FrameStart. When triggered, the device will
	//    start acquiring a single frame. This can also be set to
	//    AcquisitionStart or FrameBurstStart.
	printf("%sSet trigger selector to \"FrameStart\"\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "TriggerSelector", "FrameStart");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set trigger mode
	//    Enable trigger mode before setting the source and selector and before
	//    starting the stream. Trigger mode cannot be turned on and off while the
	//    device is streaming.
	printf("%sEnable trigger mode\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "TriggerMode", "On");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set trigger source
	//    Set the trigger source to software in order to trigger images without
	//    the use of any additional hardware. Lines of the GPIO can also be used
	//    to trigger.
	printf("%sSet trigger source to \"Software\"\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "TriggerSource", "Software");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set trigger overlap
	//    Trigger overlap defines when a trigger can start accepting a new frame.
	//    Setting trigger to overlap with the previous frame allows the camera to
	//    being exposing the new frame while the camera is still reading out the
	//    sensor data acquired from the previous frame.
	printf("%sSet trigger Overlap to \"PreviousFrame\"\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "TriggerOverlap", "PreviousFrame");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set acquisition mode
	//    When trigger mode is off and the acquisition mode is set to stream
	//    continuously, starting the stream will have the camera begin acquiring
	//    a steady stream of images. However, with trigger mode enabled, the
	//    device will wait for the trigger before acquiring any.
	printf("%sSet acquisition mode to \"Continuous\"\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "AcquisitionMode", "Continuous");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set event selector
	//    We want to trigger and wait to be notified as soon as a certain event
	//    occurs while making the image. Here we choose to be notified at the end
	//    of the exposure of an image.
	printf("%sSet event selector to \"ExposureEnd\"\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "EventSelector", "ExposureEnd");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set event notification
	//    After choosing which event to be notified about, enabling the
	//    EventNotification node will turn on the notification for the event
	//    selected in the EventSelector node.
	printf("%sEnable event notifications for \"ExposureEnd\"\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "EventNotification", "On");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Retrieve exposure time
	//    The exposure time is similar to the time between triggering images.
	//    This is shown by turning off automatic exposure and retrieving the
	//    exposure time.
	double exposureTime = 0;

	err = acNodeMapSetEnumerationValue(hNodeMap, "ExposureAuto", "Off");
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapGetFloatValue(hNodeMap, "ExposureTime", &exposureTime);
	if (err != AC_ERR_SUCCESS)
		return err;

	// convert to nanoseconds
	double exposureTimeNs = exposureTime * 1000;
	printf("%sExposure time: %.0f ns\n", TAB1, exposureTimeNs);

	// start stream
	size_t numberOfImages = NUM_IMAGES;
	printf("%sStart stream with %zu buffers\n", TAB1, numberOfImages);

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Trigger an image
	//    Trigger an image manually, since trigger mode is enabled. This triggers
	//    the camera to acquire a single image. A buffer is then filled and moved
	//    to the output queue, where it will wait to be retrieved.
	for (size_t count = 0; count < NUM_IMAGES; count++)
	{
		printf("\r%sImage %4zu triggered", TAB2, count);
		fflush(stdout);

		// Trigger Armed
		//    Continually check until trigger is armed. Once the trigger is
		//    armed, it is ready to be executed.
		bool8_t isTriggerArmed = false;

		do
		{
			acNodeMapGetBooleanValue(hNodeMap, "TriggerArmed", &isTriggerArmed);
		} while (isTriggerArmed == false);

		err = acNodeMapExecute(hNodeMap, "TriggerSoftware");
		if (err != AC_ERR_SUCCESS)
			return err;

		// Wait on event
		//    Wait on event to process before continuing. The data is created
		//    from the event generation, not from waiting on it.
		err = acDeviceWaitOnEvent(hDevice, EVENT_TIMEOUT);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf(" and ExposureEnd Event notification arrived");
	}

	printf("\n%sDone triggering %zu images\n", TAB1, numberOfImages);

	// retrieve images and timestamps
	uint64_t timestampNs[NUM_IMAGES] = { 0 };

	for (size_t count = 0; count < NUM_IMAGES; count++)
	{
		// grab images
		acBuffer hBuffer = NULL;

		err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acImageGetTimestampNs(hBuffer, &timestampNs[count]);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%sImage %4zu timestamp: %" PRId64 " ns ", TAB2, count, timestampNs[count]);

		if (count > 0)
		{
			// calculate time between triggers
			uint64_t triggerTimestampDifference = timestampNs[count] - timestampNs[count - 1];

			printf("(%" PRId64 " ns since last trigger)\n", triggerTimestampDifference);
		}
		else
			printf("\n");

		// requeue buffer for each image
		err = acDeviceRequeueBuffer(hDevice, hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// stop stream
	printf("%sStop stream\n", TAB1);

	err = acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Deinitialize events
	//    Deinitialize the events engine in order to deallocate memory and stop
	//    processing events. Failing to deinitialize events results in a memory
	//    leak.
	err = acDeviceDeinitializeEvents(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return nodes to their initial values
	err = acNodeMapSetEnumerationValue(hNodeMap, "ExposureAuto", exposureAutoInitial) |
		acNodeMapSetEnumerationValue(hNodeMap, "EventNotification", eventNotificationInitial) |
		acNodeMapSetEnumerationValue(hNodeMap, "EventSelector", eventSelectorInitial) |
		acNodeMapSetEnumerationValue(hNodeMap, "AcquisitionMode", acquisitionModeInitial) |
		acNodeMapSetEnumerationValue(hNodeMap, "TriggerOverlap", triggerOverlapInitial) |
		acNodeMapSetEnumerationValue(hNodeMap, "TriggerSource", triggerSourceInitial) |
		acNodeMapSetEnumerationValue(hNodeMap, "TriggerMode", triggerModeInitial) |
		acNodeMapSetEnumerationValue(hNodeMap, "TriggerSelector", triggerSelectorInitial);

	if (err != AC_ERR_SUCCESS)
		printf("\nWarning: failed to set one or more node values back to its initial value.\n");

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
	printf("C_Trigger_OverlappingTrigger\n");
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
	err = OverlapTriggerOnExposureEndEvent(hDevice);
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
