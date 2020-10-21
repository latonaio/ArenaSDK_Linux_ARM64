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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAB1 "  "
#define TAB2 "    "

// Helios: Heat Map
//    This example demonstrates saving an RGB heatmap of a 3D image. It captures
//    a 3D image, interprets the ABCY data to retrieve the distance value for each
//    pixel and then converts this data into a BGR and an RGB buffer. The BGR buffer
//    is used to create a jpg heatmap image and the RGB buffer is used to color the
//    ply image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// file name
#define PLY_FILE_NAME "Images/C_Helios_HeatMap.ply"
#define JPG_FILE_NAME "Images/C_Helios_HeatMap.jpg"

// pixel format
#define PIXEL_FORMAT PFNC_BGR8 // BGR8

// image timeout
#define IMAGE_TIMEOUT 2000

// device timeout
#define DEVICE_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-


// demonstrates saving a bgr heatmap image
// (1) gets image
// (2) interprets ABCY data to get z coordinate
// (3) creates a buffer for BGR and RGB colorings using z data
// (4) creates jpg heatmap image using BGR buffer
// (5) colors ply image using RGB buffer
// (6) saves jpg and ply image
AC_ERROR AcquireImageAndInterpretData(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

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
	
	err = acNodeMapGetStringValue(hNodeMap, "PixelFormat", pPixelFormatInitial, &pPixelFormatBufLen) | 
		  acNodeMapGetStringValue(hNodeMap, "Scan3dOperatingMode", pScan3dModeInitial, &pScan3dModeBufLen);
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

	// get the z coordinate scale in order to convert z values to mm
	printf("%sGet z coordinate scale\n\n", TAB1);

	err = acNodeMapSetStringValue(hNodeMap, "Scan3dCoordinateSelector", "CoordinateC");
	if (err != AC_ERR_SUCCESS)
		return err;

	double scale = 0.0;
	err = acNodeMapGetFloatValue(hNodeMap, "Scan3dCoordinateScale", &scale);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// retrieve image
	printf("%sAcquire image\n", TAB2);
	acBuffer hBuffer = NULL;

	err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	// prepare info from input buffer
	size_t width = 0;
	size_t height = 0;
	size_t srcBpp = 0;
	uint64_t srcPixelFormat = 0;
	uint8_t* pInput = NULL;

	err = acImageGetWidth(hBuffer, &width) |
		  acImageGetHeight(hBuffer, &height) |
		  acImageGetBitsPerPixel(hBuffer, &srcBpp) |
		  acImageGetPixelFormat(hBuffer, &srcPixelFormat) |
		  acImageGetData(hBuffer, &pInput);
	if (err != AC_ERR_SUCCESS)
		return err;

	size_t size = width * height;
	size_t srcPixelSize = srcBpp / 8;

	// prepare memory output buffer
	size_t dstBpp = 0;

	err = acGetBitsPerPixel(PIXEL_FORMAT, &dstBpp);
	if (err != AC_ERR_SUCCESS)
		return err;

	size_t dstPixelSize = dstBpp / 8;
	size_t dstDataSize = width * height * dstBpp / 8;
	uint8_t* pOutput = (uint8_t*)malloc(dstDataSize);
	memset(pOutput, 0, dstDataSize);

	// Prepare coloring buffer for ply image
	//    Saving ply with color takes RGB coloring compared to the BGR coloring
	//    the jpg image uses, therefore we need a separate buffer for this data.
	uint8_t* pColoring = (uint8_t*)malloc(dstDataSize);
	memset(pColoring, 0, dstDataSize);
	uint8_t* pColor = pColoring;

	// manually convert to BGR image

	const uint8_t* pIn = pInput;
	uint8_t* pOut = pOutput;

	const double RGBmin = 0;
	const double RGBmax = 255;

	const double redColorBorder = 0;
	const double yellowColorBorder = 375;
	const double greenColorBorder = 750;
	const double cyanColorBorder = 1125;
	const double blueColorBorder = 1500;

	// iterate through each pixel and assign a color to it according to a distance
	for (size_t i = 0; i < size; i++)
	{
		// Isolate the z data
		//    The first channel is the x coordinate, second channel is the y coordinate,
		//    the third channel is the z coordinate (which is what we will use to determine
		//    the colouring) and the fourth channel is confidence.
		uint16_t z = *(uint16_t*)((pIn + 4));

		// Convert z to millimeters
		//    The z data converts at a specified ratio to mm, so by multiplying it by the
		//    Scan3dCoordinateScale for CoordinateC, we are able to convert it to mm and
		//    can then compare it to the maximum distance of 1500mm.
		z = (int16_t)((double)(z)*scale);

		double coordinateColorBlue = 0.0;
		double coordinateColorGreen = 0.0;
		double coordinateColorRed = 0.0;

		// colors between red and yellow
		if ((z >= redColorBorder) && (z <= yellowColorBorder))
		{
			double yellowColorPercentage = z / yellowColorBorder;

			coordinateColorBlue = RGBmin;
			coordinateColorGreen = RGBmax * yellowColorPercentage;
			coordinateColorRed = RGBmax;
		}

		// colors between yellow and green
		else if ((z > yellowColorBorder) && (z <= greenColorBorder))
		{
			double greenColorPercentage = (z - yellowColorBorder) / yellowColorBorder;

			coordinateColorBlue = RGBmin;
			coordinateColorGreen = RGBmax;
			coordinateColorRed = RGBmax - RGBmax * greenColorPercentage;
		}

		// colors between green and cyan
		else if ((z > greenColorBorder) && (z <= cyanColorBorder))
		{
			double cyanColorPercentage = (z - greenColorBorder) / yellowColorBorder;

			coordinateColorBlue = RGBmax * cyanColorPercentage;
			coordinateColorGreen = RGBmax;
			coordinateColorRed = RGBmin;
		}

		// colors between cyan and blue
		else if ((z > cyanColorBorder) && (z <= blueColorBorder))
		{
			double blueColorPercentage = (z - cyanColorBorder) / yellowColorBorder;

			coordinateColorBlue = RGBmax;
			coordinateColorGreen = RGBmax - RGBmax * blueColorPercentage;
			coordinateColorRed = RGBmin;

		}
		else
		{
			coordinateColorBlue = RGBmin;
			coordinateColorGreen = RGBmin;
			coordinateColorRed = RGBmin;
		}

		// set pixel format values and move to next pixel
		*pOut = (uint8_t)(coordinateColorBlue);
		*(pOut + 1) = (uint8_t)(coordinateColorGreen);
		*(pOut + 2) = (uint8_t)(coordinateColorRed);

		pIn += srcPixelSize;
		pOut += dstPixelSize;

		// set RGB pixel coloring for ply
		*pColor = (uint8_t)(coordinateColorRed);
		*(pColor + 1) = (uint8_t)(coordinateColorGreen);
		*(pColor + 2) = (uint8_t)(coordinateColorBlue);
		pColor += dstPixelSize;
	}

	printf("%sCreate BGR heatmap using z data from 3D image\n", TAB2);

	// cast width and height to size_t for writer creation
	size_t saveWidth = (size_t)width;
	size_t saveHeight = (size_t)height;
	saveWriter hJpegWriter = NULL;
	err = saveWriterCreate(saveWidth, saveHeight, dstBpp, &hJpegWriter);
	if (err != AC_ERR_SUCCESS)
		return err;

	// save image as jpg
	printf("%sSave heatmap image as jpg to ", TAB2);

	// create image from buffer and save
	acBuffer hJpegCreate = NULL;
	uint8_t* hJpegImageData = NULL;
	const char* pJpegFileNamePattern = JPG_FILE_NAME;

	err = acImageFactoryCreate(pOutput, dstDataSize, saveWidth, saveHeight, PIXEL_FORMAT, &hJpegCreate);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acImageGetData(hJpegCreate, &hJpegImageData);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = saveWriterSetFileNamePattern(hJpegWriter, pJpegFileNamePattern);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = saveWriterSave(hJpegWriter, hJpegImageData);
	if (err != AC_ERR_SUCCESS)
		return err;

	char jpegFileName[MAX_BUF];
	size_t jpegFileLen = MAX_BUF;

	err = saveWriterGetLastFileName(hJpegWriter, jpegFileName, &jpegFileLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%s\n", jpegFileName);

	// save image as ply
	printf("%sSave 3D image as ply to ", TAB2);
	saveWriter hPlyWriter = NULL;

	err = saveWriterCreate(saveWidth, saveHeight, srcBpp, &hPlyWriter);
	if (err != AC_ERR_SUCCESS)
		return err;
	uint8_t* hPlyImageData = NULL;
	const char* pPlyFileNamePattern = PLY_FILE_NAME;

	err = acImageGetData(hBuffer, &hPlyImageData);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = saveWriterSetFileNamePattern(hPlyWriter, pPlyFileNamePattern);
	if (err != AC_ERR_SUCCESS)
		return err;

	// parameters for saveWriterSetPlyAndConfigExtended
	savePlyParams params = 
	{
		true,			// filterPoints default
		true,			// isSignedPixelFormat = true; the example use Coord3D_ABCY16s
		(float)scale,	// scale casted to float since saveWriterSetPlyAndConfigExtended will expect scale as float
		0.0f,			// offsetA default
		0.0f,			// offsetB default
		0.0f			// offsetC default
	};

	err = saveWriterSetPlyAndConfigExtended(hPlyWriter, params);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = saveWriterSaveWithColor(hPlyWriter, hPlyImageData, pColoring, true);
	if (err != AC_ERR_SUCCESS)
		return err;

	char plyFileName[MAX_BUF];
	size_t plyFileLen = MAX_BUF;

	err = saveWriterGetLastFileName(hPlyWriter, plyFileName, &plyFileLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%s\n\n", plyFileName);

	// clean up
	free(pOutput);

	err = acImageFactoryDestroy(hJpegCreate) |
		  acDeviceRequeueBuffer(hDevice, hBuffer) |
		  acDeviceStopStream(hDevice) |
		  saveWriterDestroy(hJpegWriter) | saveWriterDestroy(hPlyWriter);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return nodes to their initial values
	err = acNodeMapSetStringValue(hNodeMap, "Scan3dOperatingMode", pScan3dModeInitial) |
		  acNodeMapSetStringValue(hNodeMap, "PixelFormat", pPixelFormatInitial);
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
	printf("C_Helios_HeatMap\n");
	AC_ERROR err = AC_ERR_SUCCESS;

	// prepare example
	acSystem hSystem = NULL;
	err = acOpenSystem(&hSystem);
	CHECK_RETURN;
	err = acSystemUpdateDevices(hSystem, DEVICE_TIMEOUT);
	CHECK_RETURN;
	size_t numDevices = 0;
	err = acSystemGetNumDevices(hSystem, &numDevices);
	CHECK_RETURN;
	if (numDevices == 0)
	{
		printf("\nNo camera connected\nPress enter to complete\n");
		getchar();
		return 0;
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
	return 0;
}
