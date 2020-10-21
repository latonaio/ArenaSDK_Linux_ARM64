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
#include <stdbool.h> // defines boolean type and values

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Trigger: Introduction
//    This example introduces basic trigger configuration and use. In order to
//    configure trigger, enable trigger mode and set the source and selector. The
//    trigger must be armed before it is prepared to execute. Once the trigger is
//    armed, execute the trigger and retrieve an image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

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
// (2) starts stream
// (3) waits until trigger is armed
// (4) triggers image
// (5) gets image
// (6) requeues buffer
// (7) stops stream
AC_ERROR ConfigureTriggerAndAcquireImage(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map and retrieve initial node values that will be changed in
	// order to return their values at the end of the example
	acNodeMap hNodeMap = NULL;
	char triggerModeInitial[MAX_BUF];
	size_t triggerModeBufLen = MAX_BUF;
	char triggerSourceInitial[MAX_BUF];
	size_t triggerSourceBufLen = MAX_BUF;
	char triggerSelectorInitial[MAX_BUF];
	size_t triggerSelectorBufLen = MAX_BUF;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapGetEnumerationValue(hNodeMap, "TriggerSelector", triggerSelectorInitial, &triggerSelectorBufLen) |
		  acNodeMapGetEnumerationValue(hNodeMap, "TriggerMode", triggerModeInitial, &triggerModeBufLen) |
		  acNodeMapGetEnumerationValue(hNodeMap, "TriggerSource", triggerSourceInitial, &triggerSourceBufLen);
	if (err != AC_ERR_SUCCESS)
		printf("\nWarning: failed to retrieve one or more initial node values.\n");

	// Set trigger selector
	//    Set the trigger selector to FrameStart. When triggered, the device will
	//    start acquiring a single frame. This can also be set to
	//    AcquisitionStart or FrameBurstStart.
	printf("%sSet trigger selector to FrameStart\n", TAB1);

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
	printf("%sSet trigger source to Software\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "TriggerSource", "Software");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Start stream
	//    When trigger mode is off and the acquisition mode is set to stream
	//    continuously, starting the stream will have the camera begin acquiring
	//    a steady stream of images. However, with trigger mode enabled, the
	//    device will wait for the trigger before acquiring any.
	printf("%sStart stream\n", TAB1);

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Trigger Armed
	//    Continually check until trigger is armed. Once the trigger is armed, it
	//    is ready to be executed.
	printf("%sWait until trigger is armed\n", TAB2);

	bool8_t triggerArmed = false;

	do
	{
		acNodeMapGetBooleanValue(hNodeMap, "TriggerArmed", &triggerArmed);
	} while (triggerArmed == false);

	// Trigger an image
	//    Trigger an image manually, since trigger mode is enabled. This triggers
	//    the camera to acquire a single image. A buffer is then filled and moved
	//    to the output queue, where it will wait to be retrieved.
	printf("%sTrigger image\n", TAB2);

	err = acNodeMapExecute(hNodeMap, "TriggerSoftware");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Get image
	//    Once an image has been triggered, it can be retrieved. If no image has
	//    been triggered, trying to retrieve an image will hang for the duration
	//    of the timeout and then return an error
	acBuffer hBuffer = NULL;

	err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get image width and height
	size_t imageWidth = 0;
	size_t imageHeight = 0;

	err = acImageGetWidth(hBuffer, &imageWidth);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acImageGetHeight(hBuffer, &imageHeight);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%sGet image (%zux%zu)\n", TAB2, imageWidth, imageHeight);

	// requeue image buffer
	printf("%sRequeue  buffer\n", TAB2);

	err = acDeviceRequeueBuffer(hDevice, hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Stop the stream
	printf("%sStop stream\n", TAB1);

	err = acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return nodes to their initial values
	err = acNodeMapSetEnumerationValue(hNodeMap, "TriggerSource", triggerSourceInitial) |
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
	printf("C_Trigger\n");
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
	err = ConfigureTriggerAndAcquireImage(hDevice);
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
