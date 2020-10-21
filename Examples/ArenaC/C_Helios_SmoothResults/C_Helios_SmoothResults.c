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

// Helios: Smooth Results
//    This example introduces setting different nodes specific to grabbing and saving
//    a 3D image with an emphasis on smooth results.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

#define TAB1 "  "
#define TAB2 "    "

// system timeout
#define SYSTEM_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// image timeout
#define IMAGE_TIMEOUT 2000

// pixel format
#define PIXEL_FORMAT PFNC_BGR8 // BGR8

// file name
#define FILE_NAME "Images/C_Helios_SmoothResults.ply"

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates saving an image
// (1) sets all relevant nodes targeted towards getting smooth results
// (2) gets image
// (3) prepares image parameters and writer
// (4) saves ply image
AC_ERROR AcquireImageAndInterpretData(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get nodemap
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// validate if Scan3dCoordinateSelector node exists. If not - probaly not Helios camera used running the example
	char checkpScan3dCoordinateSelector[MAX_BUF];
	size_t checkpScan3dCoordinateSelectorBufLen = MAX_BUF;
	err = acNodeMapGetEnumerationValue(hNodeMap, "Scan3dCoordinateSelector", checkpScan3dCoordinateSelector, &checkpScan3dCoordinateSelectorBufLen);
	if (err != AC_ERR_SUCCESS) {
		printf("%sScan3dCoordinateSelector node is not found. Please make sure that Helios device is used for the example.\n\n", TAB1);
		return 0;
	}

	// validate if Scan3dCoordinateOffset node exists. If not - probaly Helios has an old firmware
	double checkOffset = 0.0;
	err = acNodeMapGetFloatValue(hNodeMap, "Scan3dCoordinateOffset", &checkOffset);
	if (err != AC_ERR_SUCCESS) {
		printf("%sScan3dCoordinateOffset node is not found. Please update Helios firmware.\n\n", TAB1);
		return 0;
	}


	// get node values that will be changed in order to return their values at
	// the end of the example
	char pPixelFormatInitial[MAX_BUF];
	size_t pPixelFormatBufLen = MAX_BUF;
	char pScan3dModeInitial[MAX_BUF];
	size_t pScan3dModeBufLen = MAX_BUF;
	char pExposureTimeInitial[MAX_BUF];
	size_t pExposureTimeBufLen = MAX_BUF;
	char pConversionGainInitial[MAX_BUF];
	size_t pConversionGainBufLen = MAX_BUF;
	int64_t imageAccumulationInitial = 0;
	bool spatialFilterInitial = false;
	bool confidenceThresholdInitial = false;

	err = acNodeMapGetStringValue(hNodeMap, "PixelFormat", pPixelFormatInitial, &pPixelFormatBufLen) |
		  acNodeMapGetStringValue(hNodeMap, "Scan3dOperatingMode", pScan3dModeInitial, &pScan3dModeBufLen) |
		  acNodeMapGetStringValue(hNodeMap, "ExposureTimeSelector", pExposureTimeInitial, &pExposureTimeBufLen) | 
		  acNodeMapGetStringValue(hNodeMap, "ConversionGain", pConversionGainInitial, &pConversionGainBufLen) |
		  acNodeMapGetIntegerValue(hNodeMap, "Scan3dImageAccumulation", &imageAccumulationInitial) |
		  acNodeMapGetBooleanValue(hNodeMap, "Scan3dSpatialFilterEnable", &spatialFilterInitial) |
		  acNodeMapGetBooleanValue(hNodeMap, "Scan3dConfidenceThresholdEnable", &confidenceThresholdInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set pixel format
	printf("%sSet Coord3D_ABCY16s to pixel format\n", TAB1);

	err = acNodeMapSetStringValue(hNodeMap, "PixelFormat", "Coord3D_ABCY16s");
	if (err != AC_ERR_SUCCESS)
		return err;

	// set operating mode distance
	printf("%sSet 3D operating mode to Distance1500mm\n", TAB1);

	err = acNodeMapSetStringValue(hNodeMap, "Scan3dOperatingMode", "Distance1500mm");
	if (err != AC_ERR_SUCCESS)
		return err;

	// set exposure time
	printf("%sSet time selector to Exp1000Us\n", TAB1);

	err = acNodeMapSetStringValue(hNodeMap, "ExposureTimeSelector", "Exp1000Us");
	if (err != AC_ERR_SUCCESS)
		return err;

	// set gain
	printf("%sSet gain to low\n", TAB1);

	err = acNodeMapSetStringValue(hNodeMap, "ConversionGain", "Low");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Set image accumulation
	printf("%sSet image accumulation to 4\n", TAB1);

	err = acNodeMapSetIntegerValue(hNodeMap, "Scan3dImageAccumulation", 4);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Enable spatial filter
	printf("%sEnable spatial filter\n", TAB1);

	err = acNodeMapSetBooleanValue(hNodeMap, "Scan3dSpatialFilterEnable", true);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Enable confidence threshold
	printf("%sEnable confidence threshold\n\n", TAB1);

	err = acNodeMapSetBooleanValue(hNodeMap, "Scan3dConfidenceThresholdEnable", true);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get image
	acBuffer hBuffer = NULL;

	err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Prepare image parameters
	//    An image's width, height, and bits per pixel are required to
	//    save to disk. Its size and stride (i.e. pitch) can be
	//    calculated from those 3 inputs. Notice that an image's size and
	//    stride use bytes as a unit while the bits per pixel uses bits.
	printf("%sPrepare image parameters\n", TAB2);

	size_t width = 0;
	size_t height = 0;
	size_t bpp = 0;

	err = acImageGetWidth(hBuffer, &width);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acImageGetHeight(hBuffer, &height);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acImageGetBitsPerPixel(hBuffer, &bpp);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Prepare image writer
	//    The image writer requires 3 arguments to save an image: the
	//    image's parameters, a specified file name or pattern, and the
	//    image data to save. Providing these should result in a
	//    successfully saved file on the disk. Because an image's
	//    parameters and file name pattern may repeat, they can be passed
	//    into the image writer's constructor.
	printf("%sPrepare image writer\n", TAB2);

	saveWriter hWriter = NULL;

	err = saveWriterCreate(width, height, bpp, &hWriter);
	if (err != AC_ERR_SUCCESS)
		return err;	

	err = saveWriterSetFileNamePattern(hWriter, FILE_NAME);
	if (err != AC_ERR_SUCCESS)
		return err;

	// parameters for saveWriterSetPlyAndConfigExtended
	savePlyParams plyParams = {
		true,  // filterPoints default
		true,  // isSignedPixelFormat = true; the example use Coord3D_ABCY16s
		0.25f, // scale default
		0.0f,  // offsetA default
		0.0f,  // offsetB default
		0.0f   // offsetC default
	};

	// set the output file format of the image writer to .ply
	err = saveWriterSetPlyAndConfigExtended(hWriter, plyParams);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%sSave image at %s\n\n", TAB2, FILE_NAME);

	// get image
	uint8_t* pData = NULL;

	err = acImageGetData(hBuffer, &pData);
	if (err != AC_ERR_SUCCESS)
		return err;

	// save image
	err = saveWriterSave(hWriter, pData);
	if (err != SC_ERR_SUCCESS)
		return err;

	// destroy image writer
	err = saveWriterDestroy(hWriter);
	if (err != SC_ERR_SUCCESS)
		return err;

	// clean up example
	err = acDeviceRequeueBuffer(hDevice, hBuffer) | acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return nodes to their initial values
	err = acNodeMapSetStringValue(hNodeMap, "PixelFormat", pPixelFormatInitial) |
		  acNodeMapSetStringValue(hNodeMap, "Scan3dOperatingMode", pScan3dModeInitial) |
		  acNodeMapSetStringValue(hNodeMap, "ExposureTimeSelector", pExposureTimeInitial) |
		  acNodeMapSetStringValue(hNodeMap, "ConversionGain", pConversionGainInitial) |
		  acNodeMapSetIntegerValue(hNodeMap, "Scan3dImageAccumulation", imageAccumulationInitial) |
		  acNodeMapSetBooleanValue(hNodeMap, "Scan3dSpatialFilterEnable", spatialFilterInitial) |
		  acNodeMapSetBooleanValue(hNodeMap, "Scan3dConfidenceThresholdEnable", confidenceThresholdInitial);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sNodes were set back to initial values\n", TAB1);

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
	printf("C_Helios_SmoothResults\n");
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

	
	printf("Commence example\n\n");

	// run example
	err = AcquireImageAndInterpretData(hDevice);	

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
