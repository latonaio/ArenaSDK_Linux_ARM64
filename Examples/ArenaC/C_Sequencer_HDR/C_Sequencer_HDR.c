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
#include <inttypes.h>
#include <string.h>

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Sequencer HDR
//    This example demonstrates saving a set of images using the sequencer. It
//    will cycle through 3 sequencer states each one using a different exposure
//    time. The images could then be used to generate an HDR image. Much like a
//    state machine, in order to use the sequencer we must initialize each set
//    appropriately. Each set can have its own exposure time and gain, and will
//    contain information such as the sequencer starting position as well as
//    paths to other sets. A set can have multiple paths where each path has its
//    own next set, trigger source and trigger activation. In this example the
//    sequencer has 3 sets where set 0 goes to set 1, set 1 goes to set 2 and set
//    2 goes back to set 0, all being triggered on Frame Start.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Exposure time
//    Time that the sensor will be exposed when grabbing an image (in
//    microseconds). Exposure time depends on the application, but generally the
//    less available the light, the higher the exposure time. Storing multiple
//    values so that each consecutive state can be saved with varying exposure times.
double EXPOSURE_TIME_0 = 25000.0;
double EXPOSURE_TIME_1 = 50000.0;
double EXPOSURE_TIME_2 = 100000.0;

// number of states in sequencer (<= max range of SequencerSetSelector feature)
#define NUM_SETS 3

// name of file to save
#define FILE_NAME "Images/C_Sequencer_HDR<count>.jpg"

// pixel format
#define PIXEL_FORMAT 0x02180015 // BGR8

// system timeout (milliseconds)
#define SYSTEM_TIMEOUT 100

// image timeout (milliseconds)
#define IMAGE_TIMEOUT 2000

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// set sequencer set to desired values
AC_ERROR SetSequencerSet(acNodeMap hNodeMap, int64_t setNumber, double exposureTime, int64_t pathNextSet, const char* triggerSource)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// set Sequencer Set Selector to sequence number
	printf("%sUpdating set %" PRIu64 ":\n", TAB2, setNumber);

	err = acNodeMapSetIntegerValue(hNodeMap, "SequencerSetSelector", setNumber);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapSetEnumerationValue(hNodeMap, "SequencerFeatureSelector", "ExposureTime");
	if (err != AC_ERR_SUCCESS)
		return err;

	// set exposure time to the value we want it to save
	printf("%sExposure time = %.0f\n", TAB2, exposureTime);

	err = acNodeMapSetFloatValue(hNodeMap, "ExposureTime", exposureTime);
	if (err != AC_ERR_SUCCESS)
		return err;

	// select the path we want it to follow from this set to next set, there can
	// be multiple paths so the first path will always be set to 0
	err = acNodeMapSetIntegerValue(hNodeMap, "SequencerPathSelector", 0);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set next state in the sequence, ensure it does not exceed the maximum
	printf("%sPath[0] = %" PRIu64 "\n", TAB3, pathNextSet);

	err = acNodeMapSetIntegerValue(hNodeMap, "SequencerSetNext", pathNextSet);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set Sequencer Trigger Source to Frame Start
	printf("%s Trigger source = %s\n", TAB3, triggerSource);

	err = acNodeMapSetEnumerationValue(hNodeMap, "SequencerTriggerSource", triggerSource);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Save current state
	//    Once all appropriate settings have been configured, make sure to save
	//    the state to the sequence. Notice that these settings will be lost when
	//    the camera is power-cycled.
	printf("%sSave sequence set\n", TAB3);

	err = acNodeMapExecute(hNodeMap, "SequencerSetSave");
	if (err != AC_ERR_SUCCESS)
		return err;

	return err;
}

// start streaming, acquire and save images
AC_ERROR AcquireAndSaveImages(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get width, height and pixel format
	int64_t width = 0;
	int64_t height = 0;
	acNode hPixelFormatNode = NULL;
	acNode hPixelFormatEntryNode = NULL;

	err = acNodeMapGetIntegerValue(hNodeMap, "Width", &width) | acNodeMapGetIntegerValue(hNodeMap, "Height", &height) | acNodeMapGetNode(hNodeMap, "PixelFormat", &hPixelFormatNode) | acEnumerationGetCurrentEntry(hPixelFormatNode, &hPixelFormatEntryNode);
	if (err != AC_ERR_SUCCESS)
	{
		printf("Width, Height, or PixelFormat node could not be found");
		return err;
	}

	// get bits per pixel
	int64_t pixelFormat = 0;
	size_t bitsPerPixel = 0;

	err = acEnumEntryGetIntValue(hPixelFormatEntryNode, &pixelFormat);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acGetBitsPerPixel(pixelFormat, &bitsPerPixel);
	if (err != AC_ERR_SUCCESS)
		return err;

	// prepare image writer from device settings
	saveWriter hWriter = NULL;

	// cast width and height to size_t for writer creation
	size_t saveWidth = (size_t)width;
	size_t saveHeight = (size_t)height;

	err = saveWriterCreate(saveWidth, saveHeight, bitsPerPixel, &hWriter);
	if (err != SC_ERR_SUCCESS)
		return err;

	// set file name pattern
	err = saveWriterSetFileNamePattern(hWriter, FILE_NAME);
	if (err != SC_ERR_SUCCESS)
		return err;

	// start stream
	printf("%sStart streaming\n", TAB2);

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get and save the images
	printf("%sGetting %i images\n", TAB2, NUM_SETS);

	for (int i = 0; i < NUM_SETS; i++)
	{
		// get buffer from device
		acBuffer hBuffer = NULL;

		err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;

		// convert the image to a displayable pixel format.
		printf("%sConverting and saving image %i", TAB3, i);
		acBuffer hConverted = NULL;

		err = acImageFactoryConvert(hBuffer, PIXEL_FORMAT, &hConverted);
		if (err != AC_ERR_SUCCESS)
			return err;

		// get image
		uint8_t* pData = NULL;

		err = acImageGetData(hConverted, &pData);
		if (err != AC_ERR_SUCCESS)
			return err;

		// save image
		err = saveWriterSave(hWriter, pData);
		if (err != SC_ERR_SUCCESS)
			return err;

		// get last file name
		char fileName[MAX_BUF];
		size_t fileLen = MAX_BUF;

		err = saveWriterGetLastFileName(hWriter, fileName, &fileLen);
		if (err != SC_ERR_SUCCESS)
			return err;

		printf(" at %s\n", fileName);

		// destroy converted image
		err = acImageFactoryDestroy(hConverted);
		if (err != AC_ERR_SUCCESS)
			return err;

		// requeue image buffer
		err = acDeviceRequeueBuffer(hDevice, hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// stop stream
	printf("%sStop streaming\n", TAB2);
	err = acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// destroy image writer
	err = saveWriterDestroy(hWriter);
	if (err != SC_ERR_SUCCESS)
		return err;

	return err;
}

AC_ERROR AcquireImagesUsingSequencer(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get node values that will be changed in order to return their values at
	// the end of the example
	acNode hExposureTimeNode = NULL;
	char pExposureAutoInitial[MAX_BUF];
	size_t exposureAutoLen = MAX_BUF;
	char pGainAutoInitial[MAX_BUF];
	size_t gainAutoLen = MAX_BUF;

	err = acNodeMapGetEnumerationValue(hNodeMap, "ExposureAuto", pExposureAutoInitial, &exposureAutoLen) |
		  acNodeMapGetNode(hNodeMap, "ExposureTime", &hExposureTimeNode) | acNodeMapGetEnumerationValue(hNodeMap, "GainAuto", pGainAutoInitial, &gainAutoLen);
	if (err != AC_ERR_SUCCESS)
	{
		printf("ExposureAuto, ExposureTime, or GainAuto node could not be found\n");
		return err;
	}

	// get max and min exposure time to ensure set of exposure times are within this range
	double exposureTimeInitial = 0;
	double exposureTimeMax = 0;
	double exposureTimeMin = 0;

	err = acFloatGetValue(hExposureTimeNode, &exposureTimeInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acFloatGetMax(hExposureTimeNode, &exposureTimeMax);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acFloatGetMin(hExposureTimeNode, &exposureTimeMin);
	if (err != AC_ERR_SUCCESS)
		return err;

	// if largest exposure times is not within the exposure time range, set largest exposure
	// time to max value and set the remaining exposure times to half the value of the
	// state before
	if (EXPOSURE_TIME_2 > exposureTimeMax || EXPOSURE_TIME_2 < exposureTimeMin)
	{
		EXPOSURE_TIME_2 = exposureTimeMax;
		EXPOSURE_TIME_1 = EXPOSURE_TIME_2 / 2;
		EXPOSURE_TIME_0 = EXPOSURE_TIME_1 / 2;
	}

	// Disable automatic exposure and gain
	//    Disable automatic exposure  and gain before setting an exposure time.
	//    Automatic exposure and gain controls whether they are set manually or
	//    automatically by the device. Setting automatic exposure and gain to
	//    'Off' stops the device from automatically updating the exposure time
	//    while streaming.
	printf("%sDisable automatic exposure and gain\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "ExposureAuto", "Off");
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapSetEnumerationValue(hNodeMap, "GainAuto", "Off");
	if (err != AC_ERR_SUCCESS)
		return err;

	// if sequencer mode is on, turn it off
	char pSequencerModeInitial[MAX_BUF];
	size_t SequencerModeLen = MAX_BUF;

	err = acNodeMapGetEnumerationValue(hNodeMap, "SequencerMode", pSequencerModeInitial, &SequencerModeLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	if (strcmp(pSequencerModeInitial, "On") == 0)
	{
		err = acNodeMapSetEnumerationValue(hNodeMap, "SequencerMode", "Off");
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// Put sequencer in configuration mode
	//    Sequencer configuration mode must be on while making changes to the
	//    sequencer sets.
	printf("%sTurn sequencer configuration mode on\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "SequencerConfigurationMode", "On");
	if (err != AC_ERR_SUCCESS)
		return err;

	// set up sequencer sets
	printf("%sSet up sequencer sets\n", TAB1);

	SetSequencerSet(hNodeMap, 0, EXPOSURE_TIME_0, 1, "FrameStart");
	SetSequencerSet(hNodeMap, 1, EXPOSURE_TIME_1, 2, "FrameStart");
	SetSequencerSet(hNodeMap, 2, EXPOSURE_TIME_2, 0, "FrameStart");

	// sets the sequencer starting set to be set 0
	err = acNodeMapSetIntegerValue(hNodeMap, "SequencerSetStart", 0);
	if (err != AC_ERR_SUCCESS)
		return err;

	// turn off configuration mode
	printf("%sTurn sequencer configuration mode off\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "SequencerConfigurationMode", "Off");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Turn on sequencer
	//    When sequencer mode is on and the device is streaming it will follow
	//    the sequencer sets according to their saved settings.
	printf("%sTurn sequencer mode on\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "SequencerMode", "On");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Acquire and Save images
	//    This function will start the stream, acquire an image in each set of
	//    the sequencer using its corresponding settings, save each image and
	//    then stop the stream.
	AcquireAndSaveImages(hDevice);

	// turn off sequencer mode
	printf("%sTurn sequencer mode off\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "SequencerMode", "Off");
	if (err != AC_ERR_SUCCESS)
		return err;

	// return nodes to their initial values
	err = acFloatSetValue(hExposureTimeNode, exposureTimeInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapSetEnumerationValue(hNodeMap, "ExposureAuto", pExposureAutoInitial) |
		  acNodeMapSetEnumerationValue(hNodeMap, "GainAuto", pGainAutoInitial);
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
	printf("C_Sequencer_HDR\n");
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
	err = AcquireImagesUsingSequencer(hDevice);
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
