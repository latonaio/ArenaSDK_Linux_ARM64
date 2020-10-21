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
#include <inttypes.h>
#include <string.h>

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Helios: Min/Max Depth
//    This example captures a 3D image and interprets the ABCY data into
//    their appropriate x, y and z coordinates and intensities. It converts
//    this data into millimeters and then displays this data for points with
//    both the largest and smallest values of z.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// file name
#define FILE_NAME "Images/C_Helios_MinMaxDepth.ply"

// pixel format
#define PIXEL_FORMAT "Coord3D_ABCY16s"

// image timeout
#define IMAGE_TIMEOUT 2000
#define DEVICE_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// store x, y, z data in mm and intesity for a given point
struct PointData
{
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t intensity;
};

// demonstrates acquiring 3D data for a specific point
// (1) gets image
// (2) interprets ABCY data to get x, y, z and intensity
// (3) stores data for point with min and max z values
// (4) displays 3D data for min and max points
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

	// Set pixel format
	//    This example demonstrates data interpretation for both a signed or unsigned
	//    pixel format. Default PIXEL_FORMAT here is set to Coord3D_ABCY16s but this
	//    can be modified to be an unsigned pixel format by changing it to Coord3D_ABCY16.
	printf("%sSet %s to pixel format\n", TAB1, PIXEL_FORMAT);

	err = acNodeMapSetStringValue(hNodeMap, "PixelFormat", PIXEL_FORMAT);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set operating mode distance
	printf("%sSet 3D operating mode to Distance1500mm\n", TAB1);

	err = acNodeMapSetStringValue(hNodeMap, "Scan3dOperatingMode", "Distance1500mm");
	if (err != AC_ERR_SUCCESS)
		return err;

	// get the coordinate scale in order to convert x, y and z values to mm as well
	// as the offset for x and y to correctly adjust values when in an unsigned pixel format
	printf("%sGet xyz coordinate scales and offsets\n\n", TAB1);
	double scaleX, scaleY, scaleZ = 0;

	double offsetA = 0.0;
	double offsetB = 0.0;

	err = acNodeMapSetStringValue(hNodeMap, "Scan3dCoordinateSelector", "CoordinateA") | 
		  acNodeMapGetFloatValue(hNodeMap, "Scan3dCoordinateScale", &scaleX) | 
		  acNodeMapGetFloatValue(hNodeMap, "Scan3dCoordinateOffset", &offsetA) | 
		  acNodeMapSetStringValue(hNodeMap, "Scan3dCoordinateSelector", "CoordinateB") | 
		  acNodeMapGetFloatValue(hNodeMap, "Scan3dCoordinateScale", &scaleY) | 
		  acNodeMapGetFloatValue(hNodeMap, "Scan3dCoordinateOffset", &offsetB) |
		  acNodeMapSetStringValue(hNodeMap, "Scan3dCoordinateSelector", "CoordinateC") | 
		  acNodeMapGetFloatValue(hNodeMap, "Scan3dCoordinateScale", &scaleZ);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get image
	printf("%sAcquire image\n", TAB2);
	acBuffer hBuffer = NULL;

	err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	size_t width, height = 0;
	size_t srcBpp = 0;
	uint8_t* pInput = NULL;

	err = acImageGetWidth(hBuffer, &width) |
		  acImageGetHeight(hBuffer, &height) |
		  acImageGetBitsPerPixel(hBuffer, &srcBpp) |
		  acImageGetData(hBuffer, &pInput);
	if (err != AC_ERR_SUCCESS)
		return err;

	size_t size = width * height;
	size_t srcPixelSize = srcBpp / 8;
	const uint8_t* pIn = pInput;

	// minDepth z value is set to 32767 to guarantee closer points exist
	// as this is the largest value possible
	typedef struct PointData PointData;
	PointData minDepth = { 0, 0, 32767, 0 };
	PointData maxDepth = { 0, 0, 0, 0 };

	printf("%sFind points with min and max z values\n", TAB2);

	// using strcmp to avoid conversion issue
	int compareResult_ABCY16s = strcmp(PIXEL_FORMAT, "Coord3D_ABCY16s"); // if they are equal compareResult_ABCY16s = 0
	int compareResult_ABCY16 = strcmp(PIXEL_FORMAT, "Coord3D_ABCY16"); // if they are equal compareResult_ABCY16 = 0

	bool isSignedPixelFormat = false;

	// if PIXEL_FORMAT is equal to Coord3D_ABCY16s
	if (compareResult_ABCY16s == 0)
	{
		isSignedPixelFormat = true;

		for (size_t i = 0; i < size; i++)
		{
			// Extract point data to signed 16 bit integer
			//    The first channel is the x coordinate, second channel is the y coordinate,
			//    the third channel is the z coordinate and the fourth channel is intensity.
			//    We offset pIn by 2 for each channel because pIn is an 8 bit integer and we
			//    want to read it as a 16 bit integer.
			int16_t x = *(const int16_t*)(pIn);
			int16_t y = *(const int16_t*)((pIn + 2));
			int16_t z = *(const int16_t*)((pIn + 4));
			int16_t intensity = *(const int16_t*)((pIn + 6));

			// convert x, y and z values to mm using their coordinate scales
			x = (int16_t)((double)((x)*scaleX));
			y = (int16_t)((double)((y)*scaleY));
			z = (int16_t)((double)((z)*scaleZ));

			if (z < minDepth.z && z > 0)
			{
				minDepth.x = x;
				minDepth.y = y;
				minDepth.z = z;
				minDepth.intensity = intensity;
			}
			else if (z > maxDepth.z)
			{
				maxDepth.x = x;
				maxDepth.y = y;
				maxDepth.z = z;
				maxDepth.intensity = intensity;
			}

			pIn += srcPixelSize;
		}

		// display data
		printf("%sMinimum depth point found with z distance of %d mm and intensity %d at coordinates (%d mm, %d mm)\n", TAB3, minDepth.z, minDepth.intensity, minDepth.x, minDepth.y);
		printf("%sMaximum depth point found with z distance of %d mm and intensity %d at coordinates (%d mm, %d mm)\n", TAB3, maxDepth.z, maxDepth.intensity, maxDepth.x, maxDepth.y);
	}
	// if PIXEL_FORMAT is equal to Coord3D_ABCY16
	else if (compareResult_ABCY16 == 0)
	{
		for (size_t i = 0; i < size; i++)
		{
			// Extract point data to signed 16 bit integer
			//    The first channel is the x coordinate, second channel is the y coordinate,
			//    the third channel is the z coordinate and the fourth channel is intensity.
			//    We offset pIn by 2 for each channel because pIn is an 8 bit integer and we
			//    want to read it as a 16 bit integer.
			uint16_t x = *(const uint16_t*)((pIn));
			uint16_t y = *(const uint16_t*)((pIn + 2));
			uint16_t z = *(const uint16_t*)((pIn + 4));
			uint16_t intensity = *(const uint16_t*)((pIn + 6));

			// if z is less than max value, as invalid values get filtered to 65535
			if (z < 65535)
			{
				// Convert x, y and z to millimeters
				//    Using each coordinates' appropriate scales, convert x, y and z values to mm.
				//    For the x and y coordinates in an unsigned pixel format, we must then add the
				//    offset to our converted values in order to get the correct position in millimeters.
				x = (uint16_t)((double)x * scaleX + offsetA);
				y = (uint16_t)((double)y * scaleY + offsetB);
				z = (uint16_t)((double)(z)*scaleZ);

				if (z < minDepth.z && z > 0)
				{
					minDepth.x = x;
					minDepth.y = y;
					minDepth.z = z;
					minDepth.intensity = intensity;
				}
				else if (z > maxDepth.z)
				{
					maxDepth.x = x;
					maxDepth.y = y;
					maxDepth.z = z;
					maxDepth.intensity = intensity;
				}
			}

			pIn += srcPixelSize;
		}

		// display data
		printf("%sMinimum depth point found with z distance of %d mm and intensity %d at coordinates (%d mm, %d mm)\n", TAB3, minDepth.z, minDepth.intensity, minDepth.x, minDepth.y);
		printf("%sMaximum depth point found with z distance of %d mm and intensity %d at coordinates (%d mm, %d mm)\n", TAB3, maxDepth.z, maxDepth.intensity, maxDepth.x, maxDepth.y);
	}

	else
	{
		printf("%sThis example requires the camera to be in either 3D image format Coord3D_ABCY16 or Coord3D_ABCY16s\n\n", TAB1);
	}
	// cast width and height to size_t for writer creation
	saveWriter hWriter = NULL;

	size_t saveWidth = (size_t)width;
	size_t saveHeight = (size_t)height;

	err = saveWriterCreate(saveWidth, saveHeight, srcBpp, &hWriter);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set file name pattern
	err = saveWriterSetFileNamePattern(hWriter, FILE_NAME);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get image
	uint8_t* pData = NULL;

	err = acImageGetData(hBuffer, &pData);
	if (err != AC_ERR_SUCCESS)
		return err;

	// parameters for saveWriterSetPlyAndConfigExtended
	savePlyParams plyParams =
	{
		true,                     // filterPoints default
		isSignedPixelFormat,         
		(float)scaleX,            // using scaleX as scale since all scales = 0.25f. Casted to float since saveWriterSetPlyAndConfigExtended will expect it as float
		(float)offsetA,           // offsetA casted to float since saveWriterSetPlyAndConfigExtended will expect it as float
		(float)offsetB,           // offsetB casted to float since saveWriterSetPlyAndConfigExtended will expect it as float
		0.0f                      // offsetC default
	};

	err = saveWriterSetPlyAndConfigExtended(hWriter, plyParams);
	if (err != AC_ERR_SUCCESS)
		return err;

	// save image
	err = saveWriterSave(hWriter, pData);
	if (err != AC_ERR_SUCCESS)
		return err;

	char fileName[MAX_BUF];
	size_t fileLen = MAX_BUF;

	err = saveWriterGetLastFileName(hWriter, fileName, &fileLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%sSave image at %s\n\n", TAB2, fileName);

	err = acDeviceRequeueBuffer(hDevice, hBuffer) |
		  acDeviceStopStream(hDevice) |
		  saveWriterDestroy(hWriter);
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
	printf("C_Helios_MinMaxDepth\n");
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
