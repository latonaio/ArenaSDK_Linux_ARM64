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

#define TAB1 "  "

// Save: Introduction
//    This example introduces the basic save capabilities of the save library. It
//    shows the construction of an image parameters object and an image writer,
//    and saves a single image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// File name
//    The relative path and file name to save to. After running the example, an
//    image should exist at the location specified. The image writer chooses the
//    file format by the image's extension. Aside from PNG (.png), images can be
//    saved as JPEG (.jpg), TIFF (.tiff), BMP (.bmp), and raw (.raw) files. For
//    3D file formats the PLY (.ply) extension can be used.
#define FILE_NAME "Images/C_Save/image.png"

// pixel format
#define PIXEL_FORMAT PFNC_BGR8 // BGR8

// timeout for detecting camera devices (in milliseconds).
#define SYSTEM_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates saving an image
// (1) converts image to a displayable pixel format
// (2) prepares image parameters
// (3) prepares image writer
// (4) saves image
// (5) destroys converted image
AC_ERROR SaveImage(acBuffer hBuffer, const char* filename)
{
	// AC_ERROR and SC_ERROR values are equivalent
	AC_ERROR acErr = AC_ERR_SUCCESS;
	SC_ERROR saveErr = SC_ERR_SUCCESS;

	// Convert image
	//    Convert the image to a displayable pixel format. It is worth keeping in
	//    mind the best pixel and file formats for your application. This example
	//    converts the image so that it is displayable by the operating system.
	printf("%sConvert image to %s\n", TAB1, (PIXEL_FORMAT == PFNC_BGR8 ? "BGR8" : "RGB8"));
	acBuffer hConverted = NULL;

	acErr = acImageFactoryConvert(hBuffer, PIXEL_FORMAT, &hConverted);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// Prepare image parameters
	//    An image's width, height, and bits per pixel are required to save to
	//    disk.
	printf("%sPrepare image parameters\n", TAB1);

	// get width
	size_t width = 0;

	acErr = acImageGetWidth(hConverted, &width);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// get height
	size_t height = 0;

	acErr = acImageGetHeight(hConverted, &height);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// get bits per pixel
	size_t bpp = 0;

	acErr = acImageGetBitsPerPixel(hConverted, &bpp);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// Prepare image writer
	//    The image writer requires the image's parameters, a specified file name
	//    or pattern, and the image data to save. Providing these should result
	//    in a successfully saved file on the disk. Because an image's parameters
	//    and file name pattern may repeat, they can be passed into the image
	//    writer's constructor.
	printf("%sPrepare image writer\n", TAB1);
	saveWriter hWriter = NULL;

	saveErr = saveWriterCreate(width, height, bpp, &hWriter);
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	saveErr = saveWriterSetFileNamePattern(hWriter, filename);
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	// Save image
	//    Get and save the image. Notice that pointers to the beginning of the
	//    image data stay valid for the life cycle of the object, and do not
	//    require memory allocation or object destruction.
	printf("%sSave image\n", TAB1);

	// get image
	uint8_t* pData = NULL;

	acErr = acImageGetData(hConverted, &pData);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// save image
	saveErr = saveWriterSave(hWriter, pData);
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	// destroy image writer
	saveErr = saveWriterDestroy(hWriter);
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	// destroy converted image
	acErr = acImageFactoryDestroy(hConverted);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	return SC_ERR_SUCCESS;
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
	printf("C_Save\n");
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
		return 0;
	}
	acDevice hDevice = NULL;
	err = acSystemCreateDevice(hSystem, 0, &hDevice);
	CHECK_RETURN;
	err = acDeviceStartStream(hDevice);
	CHECK_RETURN;
	acBuffer hBuffer = NULL;
	err = acDeviceGetBuffer(hDevice, 2000, &hBuffer);
	CHECK_RETURN;

	// get bits per pixel
	uint64_t pixelFormat = 0;
	err = acImageGetPixelFormat(hBuffer, &pixelFormat);
	CHECK_RETURN;

	// run example
	printf("Commence example\n\n");
	err = SaveImage(hBuffer, FILE_NAME);
	CHECK_RETURN;
	printf("\nExample complete\n");

	// clean up example
	err = acDeviceRequeueBuffer(hDevice, hBuffer);
	CHECK_RETURN;
	err = acDeviceStopStream(hDevice);
	CHECK_RETURN;
	err = acSystemDestroyDevice(hSystem, hDevice);
	CHECK_RETURN;
	err = acCloseSystem(hSystem);
	CHECK_RETURN;

	printf("Press enter to complete\n");
	getchar();
	return 0;
}
