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

#define _USE_MATH_DEFINES
#include <math.h>

#define TAB1 "  "
#define TAB2 "    "

// Polarization, Color DoLP AoLP
//    This example introduces the basics of working with polarization pixel
//    formats. Specifically, this examples retrieves a 2-channel DoLP, AoLP
//    image, PolarizedDolpAolp_Mono8, and converts it to a BGR image for saving,
//    treating the AoLP data as hue, and the DoLP data as saturation.  This
//    example shows only how to convert the 8-bit-per-channel form of the pixel
//    format. The 12p-bit-per-channel form, while similar, will not produce a
//    correct result.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// name of file to save
#define FILE_NAME "Images/C_Polarization_ColorDolpAolp.jpg"

// pixel format
#define PIXEL_FORMAT 0x02180015 // BGR8
#define PIXEL_FORMAT_NAME "BGR8"

#define LUCID_PolarizedDolpAolp_BayerRG8 0x8210029F

// image timeout (milliseconds)
#define IMAGE_TIMEOUT 2000

// system timeout (milliseconds)
#define SYSTEM_TIMEOUT 100

// max buffer size
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

void SplitTiles(const uint8_t* pSrc, size_t srcDoubleStep, size_t srcStride, size_t srcW, size_t srcH, uint8_t* pDst, size_t dstStep, size_t dstHalfStride)
{
	for (size_t i = 0; i < srcH; i += 2)
	{
		for (size_t j = 0; j < srcW; j += 2)
		{
			*pDst = *pSrc;

			*(pDst + 1) = *(pSrc + 1);

			pSrc += srcDoubleStep;

			pDst += dstStep;
		}

		pSrc += srcStride;

		pDst += dstHalfStride;
	}
}

// demonstrates acquisition
// (1) acquires image
// (2) splits bayer tile data into into 2x2 grid
// (3) manually converts to HSV
// (4) treats AoLP as hue
// (5) treats DoLP as saturation
// (6) converts HSV to displayable pixel format
// (7) saves to disk
AC_ERROR ConvertDoLPAoLPToPixelFormat(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get node values that will be changed in order to return their values at
	// the end of the example
	char pixelFormatInitial[MAX_BUF];
	size_t pixelFormatBufLen = MAX_BUF;

	err = acNodeMapGetEnumerationValue(hNodeMap, "PixelFormat", pixelFormatInitial, &pixelFormatBufLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Change to DoLP AoLP pixel format
	//    DoLP AoLP pixel formats are a 2-channel image. The first channel holds
	//    DoLP (Degree of Linear Polarization) data while the second holds AoLP
	//    (Angle of Linear Polarization) data.
	printf("%sSet PolarizedDolpAolp_BayerRG8 to pixel format\n", TAB1);

	err = acNodeMapSetEnumerationValue(hNodeMap, "PixelFormat", "PolarizedDolpAolp_BayerRG8");
	if (err != AC_ERR_SUCCESS)
		return err;

	// retrieve image
	printf("%sAcquire image\n", TAB1);
	acBuffer hBuffer = NULL;

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	// src info
	size_t srcWidth = 0;
	size_t srcHeight = 0;
	size_t srcBitsPerPixel = 0;
	uint64_t srcPF = 0;
	uint8_t* pSrc = NULL;

	err = acImageGetPixelFormat(hBuffer, &srcPF) |
		  acImageGetWidth(hBuffer, &srcWidth) |
		  acImageGetHeight(hBuffer, &srcHeight) |
		  acImageGetBitsPerPixel(hBuffer, &srcBitsPerPixel) |
		  acImageGetData(hBuffer, &pSrc);
	if (err != AC_ERR_SUCCESS)
		return err;

	size_t srcBytesPerPixel = srcBitsPerPixel / 8;
	size_t srcDoubleBytesPerPixel = srcBytesPerPixel * 2;
	size_t srcStride = srcWidth * srcBitsPerPixel / 8;

	// dst info
	uint64_t dstPF = srcPF;
	size_t dstWidth = srcWidth;
	size_t dstHeight = srcHeight;
	size_t dstBitsPerPixel = 0;
	err = acImageGetBitsPerPixel(hBuffer, &dstBitsPerPixel);
	if (err != AC_ERR_SUCCESS)
		return err;
	size_t dstBytesPerPixel = dstBitsPerPixel / 8;
	size_t dstStride = dstWidth * dstBitsPerPixel / 8;
	size_t dstHalfStride = dstStride / 2;
	size_t dstDataSize = dstWidth * dstHeight * dstBitsPerPixel / 8;
	size_t dstHalfDataSize = dstDataSize / 2;

	uint8_t* pDst = (uint8_t*)malloc(dstDataSize);
	memset(pDst, 0, dstDataSize);

	// reference to starting position of source image to read from
	uint8_t* pSrcTopLeft = pSrc;
	uint8_t* pSrcTopRight = pSrc + srcBytesPerPixel;
	uint8_t* pSrcBottomLeft = pSrc + srcStride;
	uint8_t* pSrcBottomRight = pSrc + srcStride + srcBytesPerPixel;

	// reference to starting position of each quadrant of destination 2x2 grid
	uint8_t* pDstTopLeft = pDst;
	uint8_t* pDstTopRight = pDst + dstHalfStride;
	uint8_t* pDstBottomLeft = pDst + dstHalfDataSize;
	uint8_t* pDstBottomRight = pDst + dstHalfDataSize + dstHalfStride;

	// split bayer tile data into 2x2 grid
	printf("%sSplitting bayer tile data into into 2x2 grid\n", TAB1);

	SplitTiles(pSrcTopLeft, srcDoubleBytesPerPixel, srcStride, srcWidth, srcHeight, pDstTopLeft, dstBytesPerPixel, dstHalfStride);
	SplitTiles(pSrcTopRight, srcDoubleBytesPerPixel, srcStride, srcWidth, srcHeight, pDstTopRight, dstBytesPerPixel, dstHalfStride);
	SplitTiles(pSrcBottomLeft, srcDoubleBytesPerPixel, srcStride, srcWidth, srcHeight, pDstBottomLeft, dstBytesPerPixel, dstHalfStride);
	SplitTiles(pSrcBottomRight, srcDoubleBytesPerPixel, srcStride, srcWidth, srcHeight, pDstBottomRight, dstBytesPerPixel, dstHalfStride);

	saveWriter hSaveWriter = NULL;

	err = saveWriterCreate(dstWidth, dstHeight, dstBitsPerPixel, &hSaveWriter);
	if (err != AC_ERR_SUCCESS)
		return err;

	// create image from buffer and save
	acBuffer pCreate = NULL;

	err = acImageFactoryCreate(pDst, dstDataSize, dstWidth, dstHeight, dstPF, &pCreate);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Manually convert to HSV image
	//     Treat the AoLP data as hue and the DoLP data as saturation, and manually
	//     convert from HSV to the desired pixel format: algorithm available on Wikipedia:
	//     https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV
	size_t width = 0;
	size_t height = 0;
	size_t inBitsPerPixel = 0;
	uint64_t inPixelFormat = 0;
	uint8_t* pInput = NULL;

	err = acImageGetWidth(pCreate, &width) |
		  acImageGetHeight(pCreate, &height) |
		  acImageGetBitsPerPixel(pCreate, &inBitsPerPixel) |
		  acImageGetPixelFormat(pCreate, &inPixelFormat) |
		  acImageGetData(pCreate, &pInput);
	if (err != AC_ERR_SUCCESS)
		return err;

	size_t size = width * height;
	size_t inPixelSize = srcBitsPerPixel / 8;

	// prepare memory output buffer
	size_t outBitsPerPixel = 0;

	err = acGetBitsPerPixel(PIXEL_FORMAT, &outBitsPerPixel);
	if (err != AC_ERR_SUCCESS)
		return err;

	size_t outPixelSize = outBitsPerPixel / 8;
	size_t outDataSize = width * height * outBitsPerPixel / 8;
	uint8_t* pOutput = (uint8_t*)malloc(outDataSize);
	memset(pOutput, 0, outDataSize);

	// Convert 2-channel 8-bits-per-channel DoLP AoLP
	//    The first channel of each pixel holds the DoLP data and has a maximum
	//    value of 255; the second channel of each pixel holds the AoLP data and
	//    has a maximum value of 201.
	printf("%sUsing AoLP as hue and DoLP as saturation, convert from HSV to %s\n", TAB1, PIXEL_FORMAT_NAME);

	if (inPixelFormat == LUCID_PolarizedDolpAolp_BayerRG8)
	{
		const uint8_t* pIn = pInput;
		uint8_t* pOut = pOutput;
		for (size_t i = 0; i < size; i++)
		{
			// Separate the AoLP and DoLP channels
			//    The first channel is the DoLP (degree of linear polarization)
			//    channel. The second channel is the AoLP (angle of linear
			//    polarization) channel.
			double dolpValue = (double)(*pIn);
			double aolpValue = (double)(*(pIn + 1));

			// Map to hue, saturation, and value
			//    For the formula to work, double the AoLP for the hue and map
			//    the DoLP values between 0 and 1 for saturation.  Maximize value
			//    to keep things bright.
			double hue = (aolpValue)*2.0;
			if (hue > 255.0)
				hue = 255.0;
			double saturation = dolpValue / 255.0;
			double value = 255.0;

			// Calculate chroma, hue', and x (second largest component)
			//    These intermediary values help in the conversion of HSV to BGR.
			double c = value * saturation;
			double h = hue / 60.0;
			double x = c * (1.0 - fabs(fmod(h, 2.0) - 1.0));

			// Calculate blue, green, and red
			//    Blue, green, and red can be calculated from hue, saturation,
			//    and value, and the intermediary values chroma, hue', and x.
			double blue = 0.0;
			double green = 0.0;
			double red = 0.0;

			// colors between red and yellow
			if ((h >= 0) && (h <= 1))
			{
				blue = value - c;
				green = x + value - c;
				red = value;
			}

			// colors between yellow and green
			else if ((h >= 1) && (h <= 2))
			{
				blue = value - c;
				green = value;
				red = x + value - c;
			}

			// colors between green and cyan
			else if ((h >= 2) && (h <= 3))
			{
				blue = x + value - c;
				green = value;
				red = value - c;
			}

			// colors between cyan and blue
			else if ((h >= 3) && (h <= 4))
			{
				blue = value;
				green = x + value - c;
				red = value - c;
			}

			// colors between blue and magenta
			else if ((h >= 4) && (h <= 5))
			{
				blue = value;
				green = value - c;
				red = x + value - c;
			}

			// colors between magenta and red
			else if ((h >= 5) && (h <= 6))
			{
				blue = x + value - c;
				green = value - c;
				red = value;
			}

			// set pixel format values and move to next pixel
			*pOut = (uint8_t)blue;
			*(pOut + 1) = (uint8_t)green;
			*(pOut + 2) = (uint8_t)red;

			pIn += inPixelSize;
			pOut += outPixelSize;
		}
	}
	else
	{
		printf("\n\nThis example requires PolarizedDolpAolp_BayerRG8 pixel format");
		return err;
	}

	// create image from buffer and save
	printf("%sSave image to ", TAB1);

	// cast width and height to size_t for writer creation
	size_t saveWidth = (size_t)width;
	size_t saveHeight = (size_t)height;
	saveWriter hWriter = NULL;
	err = saveWriterCreate(saveWidth, saveHeight, outBitsPerPixel, &hWriter);
	if (err != AC_ERR_SUCCESS)
		return err;

	// create image from buffer and save
	acBuffer hCreate = NULL;
	uint8_t* hImageData = NULL;
	const char* pFileNamePattern = FILE_NAME;

	err = acImageFactoryCreate(pOutput, outDataSize, saveWidth, saveHeight, PIXEL_FORMAT, &hCreate);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acImageGetData(hCreate, &hImageData);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = saveWriterSetFileNamePattern(hWriter, pFileNamePattern);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = saveWriterSave(hWriter, hImageData);
	if (err != AC_ERR_SUCCESS)
		return err;

	char fileName[MAX_BUF];
	size_t fileLen = MAX_BUF;

	err = saveWriterGetLastFileName(hWriter, fileName, &fileLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%s\n", fileName);

	// clean up
	free(pOutput);

	err = acImageFactoryDestroy(hCreate) |
		  acImageFactoryDestroy(pCreate) |
		  acDeviceRequeueBuffer(hDevice, hBuffer) |
		  acDeviceStopStream(hDevice) |
		  saveWriterDestroy(hWriter);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return nodes to their initial values
	err = acNodeMapSetEnumerationValue(hNodeMap, "PixelFormat", pixelFormatInitial);
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
	printf("C_Polarization_ColorDolpAolp\n");
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
	err = ConvertDoLPAoLPToPixelFormat(hDevice);
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
