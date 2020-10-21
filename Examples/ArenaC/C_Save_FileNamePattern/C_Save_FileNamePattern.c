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
#define TAB2 "    "

// Save: File Name Pattern
//    This example demonstrates saving a set of images according to a file name
//    pattern, which uses the <count> and <timestamp> tags to differentiate
//    between saved images. The essential point of the example includes setting
//    the image writer up with a file name pattern to save each image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// File name pattern
//    File name patterns can use tags to easily customize your file names.
//    Customizable tags can be added to a file name pattern and later set on the
//    fly. Two tags, <count> and <datetime> have been built in to the save
//    library. As seen below, <datetime> can take an argument to specify output.
//    <count> also accepts arguments (local, path, and global) to specify what
//    exactly is being counted. Aside from PNG (.png), images can be saved as
//    JPEG (.jpg), TIFF (.tiff), BMP (.bmp), and raw (.raw) files.
#define FILE_NAME "Images/C_Save_FileNamePattern/<vendor>_<model>_<serial>_image<count>-<datetime:yyMMdd_hhmmss_fff>.bmp"

// pixel format
#define PIXEL_FORMAT 0x02180015 // BGR8

// system timeout (milliseconds)
#define SYSTEM_TIMEOUT 100

// image timeout (milliseconds)
#define IMAGE_TIMEOUT 2000

// number of images to acquire and save
#define NUM_IMAGES 25

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates saving images using a pattern
// (1) prepares image parameters
// (2) prepares image writer with a file name pattern, including <count> and <timestamp> tags
// (3) grabs images and saves each image without supplying new file names
// (4) requeues buffers
// (5) destroys image writer
AC_ERROR AcquireAndSaveImages(acDevice hDevice)
{
	// AC_ERROR and SC_ERROR values are equivalent
	AC_ERROR acErr = AC_ERR_SUCCESS;
	SC_ERROR saveErr = SC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	acErr = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// Prepare image parameters from device settings
	//    An image's width, height, and bits per pixel are required to save to
	//    disk. Its size and stride (i.e. pitch) can be calculated from those 3
	//    inputs. Notice that an image's size and stride use bytes as a unit
	//    while the bits per pixel uses bits. Arena defines its pixel formats by
	//    the PFNC (Pixel Format Naming Convention), which embeds the number of
	//    bits per pixel within a pixel format's integer representation.
	printf("%sPrepare image parameters\n", TAB1);

	// get width
	int64_t width = 0;

	acErr = acNodeMapGetIntegerValue(hNodeMap, "Width", &width);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// get height
	int64_t height = 0;

	acErr = acNodeMapGetIntegerValue(hNodeMap, "Height", &height);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// get pixel format
	acNode hPixelFormatNode = NULL;
	acNode hPixelFormatEntryNode = NULL;

	acErr = acNodeMapGetNode(hNodeMap, "PixelFormat", &hPixelFormatNode);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	acErr = acEnumerationGetCurrentEntry(hPixelFormatNode, &hPixelFormatEntryNode);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// get bits per pixel
	int64_t pixelFormatValue = 0;
	size_t bitsPerPixel = 0;

	acErr = acEnumEntryGetIntValue(hPixelFormatEntryNode, &pixelFormatValue);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	acErr = acGetBitsPerPixel(pixelFormatValue, &bitsPerPixel);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// Prepare image writer
	//    The image writer requires 3 arguments to save an image: the image's
	//    parameters, a specified file name or pattern, and the image data to
	//    save. If a file name pattern uses the <timestamp> tag, then a timestamp
	//    must also be provided. Providing these should result in a successfully
	//    saved file on the disk.
	printf("%sPrepare image writer\n", TAB1);
	saveWriter hWriter = NULL;

	// cast width and height to size_t for writer creation
	size_t saveWidth = (size_t)width;
	size_t saveHeight = (size_t)height;

	saveErr = saveWriterCreate(saveWidth, saveHeight, bitsPerPixel, &hWriter);
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	// Update tags
	//    Tags are set on the fly by passing strings into the cascading I/O
	//    operator. Tags are accepted as any string surrounded by angle brackets
	//    <...> while values are accepted as everything else. A value will be set
	//    to the last input tag.
	printf("%sUpdate tags\n", TAB1);

	// set <vendor> tag
	saveErr = saveWriterUpdateTag(hWriter, "<vendor>", "LUCID");
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	printf("%s<vendor> to LUCID\n", TAB2);

	// set <model> tag
	char modelValueBuf[MAX_BUF];
	size_t modelBufLen = MAX_BUF;

	acErr = acNodeMapGetStringValue(hNodeMap, "DeviceModelName", modelValueBuf, &modelBufLen);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	saveErr = saveWriterUpdateTag(hWriter, "<model>", modelValueBuf);
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	printf("%s<model> to %s\n", TAB2, modelValueBuf);

	// set <serial> tag
	char serialValueBuf[MAX_BUF];
	size_t serialBufLen = MAX_BUF;

	acErr = acNodeMapGetStringValue(hNodeMap, "DeviceSerialNumber", serialValueBuf, &serialBufLen);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	saveErr = saveWriterUpdateTag(hWriter, "<serial>", serialValueBuf);
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	printf("%s<serial> to %s\n", TAB2, serialValueBuf);

	// set file name pattern
	saveErr = saveWriterSetFileNamePattern(hWriter, FILE_NAME);
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	// get and save the images
	printf("%sGet %i images\n", TAB1, NUM_IMAGES);

	// start stream
	acErr = acDeviceStartStream(hDevice);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	for (int i = 0; i < NUM_IMAGES; i++)
	{
		// get buffer from device
		acBuffer hBuffer = NULL;
		acErr = acDeviceGetBuffer(hDevice, IMAGE_TIMEOUT, &hBuffer);
		if (acErr != AC_ERR_SUCCESS)
			return acErr;

		// convert the image to a displayable pixel format.
		acBuffer hConverted = NULL;

		acErr = acImageFactoryConvert(hBuffer, PIXEL_FORMAT, &hConverted);
		if (acErr != AC_ERR_SUCCESS)
			return acErr;
		// get image
		uint8_t* pData = NULL;

		acErr = acImageGetData(hConverted, &pData);
		if (acErr != AC_ERR_SUCCESS)
			return acErr;

		// save image
		saveErr = saveWriterSave(hWriter, pData);
		if (saveErr != SC_ERR_SUCCESS)
			return saveErr;

		// Get last file name
		//    The image writer allows for the retrieval of paths, file names, and
		//    extensions. They can be retrieved together or separately, and it is
		//    possible to get the pattern, peek ahead at the next file name, or
		//    get the last file name.
		char fileName[MAX_BUF];
		size_t fileLen = MAX_BUF;

		saveErr = saveWriterGetLastFileName(hWriter, fileName, &fileLen);
		if (saveErr != SC_ERR_SUCCESS)
			return saveErr;

		printf("%sSave image %i at %s\n", TAB1, i, fileName);

		// requeue image buffer
		acErr = acDeviceRequeueBuffer(hDevice, hBuffer);
		if (acErr != AC_ERR_SUCCESS)
			return acErr;
	}

	// stop stream
	acErr = acDeviceStopStream(hDevice);
	if (acErr != AC_ERR_SUCCESS)
		return acErr;

	// destroy image writer
	saveErr = saveWriterDestroy(hWriter);
	if (saveErr != SC_ERR_SUCCESS)
		return saveErr;

	return acErr;
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
	printf("C_Save_FileNamePattern\n");
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
	err = AcquireAndSaveImages(hDevice);
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
