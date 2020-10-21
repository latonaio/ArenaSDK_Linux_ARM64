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
#include <stdbool.h>  // defines boolean type and values

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Pixel Correction
//    This example introduces the basics of pixel correction. A single arbitrary
//    pixel is chosen and added to the device's pixel correction list. These
//    changes are then saved to the camera before being removed.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Pixel values
//    This example does not search for pixels to correct, but instead just uses
//    arbitrary pixels to demonstrate what would be done if a bad pixel were
//    found.
#define PIXEL_X 256
#define PIXEL_Y 128

// system timeout
#define SYSTEM_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates pixel correction
// (1) enables pixel correction
// (2) adds new pixel to correction list
// (3) sets new pixel coordinates
// (4) applies changes to correction list
// (5) saves correction to camera
// (6) removes pixel from correction list
AC_ERROR CorrectPixels(acNodeMap hNodeMap, int64_t pixelX, int64_t pixelY)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node values that will be changed in order to return their values at
	// the end of the example
	bool8_t pixelCorrectionEnableInitial = false;

	err = acNodeMapGetBooleanValue(hNodeMap, "DefectCorrectionEnable", &pixelCorrectionEnableInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Enable pixel correction
	printf("%sEnable pixel correction\n", TAB1);

	err = acNodeMapSetBooleanValue(hNodeMap, "DefectCorrectionEnable", true);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get initial correction count
	int64_t pixelCorrectionCountInitial = 0;

	err = acNodeMapGetIntegerValue(hNodeMap, "DefectCorrectionCount", &pixelCorrectionCountInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Add new pixel to end of correction list.
	//    Getting a new defect automatically updates the pixel correction index.
	printf("%sAdd pixel to correction list\n", TAB1);

	err = acNodeMapExecute(hNodeMap, "DefectCorrectionGetNewDefect");
	if (err != AC_ERR_SUCCESS)
		return err;

	// get updated pixel index
	int64_t pixelCorrectionUpdatedIndex = 0;

	err = acNodeMapGetIntegerValue(hNodeMap, "DefectCorrectionIndex", &pixelCorrectionUpdatedIndex);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%sPixel index: %2" PRId64, TAB2, pixelCorrectionUpdatedIndex);

	// Set pixel position
	err = acNodeMapSetIntegerValue(hNodeMap, "DefectCorrectionPositionX", pixelX);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapSetIntegerValue(hNodeMap, "DefectCorrectionPositionY", pixelY);
	if (err != AC_ERR_SUCCESS)
		return err;

	// retrieve pixel position to ensure the x and y values were set correctly
	int64_t pixelCorrectionPositionX = 0;
	int64_t pixelCorrectionPositionY = 0;

	err = acNodeMapGetIntegerValue(hNodeMap, "DefectCorrectionPositionX", &pixelCorrectionPositionX);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapGetIntegerValue(hNodeMap, "DefectCorrectionPositionY", &pixelCorrectionPositionY);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf(" (x: %4" PRId64, pixelCorrectionPositionX);
	printf(", y: %4" PRId64 ")\n", pixelCorrectionPositionY);

	// Apply correction list
	//    Once the pixel corrections are applied, they will take effect
	//    immediately. However, by power-cycling the camera, the defect list will
	//    go back to its default values.
	printf("%sApply correction\n", TAB1);

	err = acNodeMapExecute(hNodeMap, "DefectCorrectionApply");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Save correction to camera
	//    Optionally write the correction to the camera to make the changes
	//    permanent (the camera can still be set to default by executing the
	//    DefectCorrectionRestoreDefault node)
	printf("%sSave correction to camera\n", TAB1);

	err = acNodeMapExecute(hNodeMap, "DefectCorrectionSave");
	if (err != AC_ERR_SUCCESS)
		return err;

	// Remove all pixels set through this example by decrementing through the index
	//    The index is updated so that there are no empty indices after
	//    removal, regardless of position
	printf("%sFind and remove pixel from correction list\n", TAB1);

	for (int64_t pixelCorrectionIndex = pixelCorrectionUpdatedIndex; pixelCorrectionIndex >= pixelCorrectionCountInitial; pixelCorrectionIndex--)
	{
		// select pixel
		err = acNodeMapSetIntegerValue(hNodeMap, "DefectCorrectionIndex", pixelCorrectionIndex);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%sPixel index: %2" PRId64, TAB2, pixelCorrectionIndex);

		// check pixel correction position
		int64_t x = 0;
		int64_t y = 0;

		err = acNodeMapGetIntegerValue(hNodeMap, "DefectCorrectionPositionX", &x);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapGetIntegerValue(hNodeMap, "DefectCorrectionPositionY", &y);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf(" (x: %4" PRId64, x);
		printf(", y: %4" PRId64 ")", y);

		if (x == pixelX && y == pixelY)
		{
			printf(" matches\n");

			// Delete pixel from correction list
			printf("%sRemove pixel\n", TAB2);

			err = acNodeMapExecute(hNodeMap, "DefectCorrectionRemove");
			if (err != AC_ERR_SUCCESS)
				return err;
			break;
		}
		else
		{
			printf(" does not match\n");
		}
	}

	// return nodes to their initial values
	err = acNodeMapSetBooleanValue(hNodeMap, "DefectCorrectionEnable", pixelCorrectionEnableInitial);
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
	printf("C_PixelCorrection\n");
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
	acNodeMap hNodeMap = NULL;
	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	CHECK_RETURN

	// ensure arbitrary pixel selections are not already corrected
	int64_t pixelX = PIXEL_X;
	int64_t pixelY = PIXEL_Y;
	int64_t pixelCorrectionCount = 0;

	err = acNodeMapGetIntegerValue(hNodeMap, "DefectCorrectionCount", &pixelCorrectionCount);
	CHECK_RETURN

	if (pixelCorrectionCount > 0)
	{
		for (int64_t i = 0; i < pixelCorrectionCount; i++)
		{
			err = acNodeMapSetIntegerValue(hNodeMap, "DefectCorrectionIndex", i);
			CHECK_RETURN

			// get positions of each corrected pixel
			int64_t x = 0;
			int64_t y = 0;

			err = acNodeMapGetIntegerValue(hNodeMap, "DefectCorrectionPositionX", &x);
			CHECK_RETURN
			err = acNodeMapGetIntegerValue(hNodeMap, "DefectCorrectionPositionY", &y);
			CHECK_RETURN

			// return error if chosen pixel has already been corrected
			if (x == pixelX && y == pixelY)
			{
				printf("\nPixels already corrected\nPress enter to complete\n");
				getchar();
				return -1;
			}
		}
	}

	// run example
	printf("Commence example\n\n");
	err = CorrectPixels(hNodeMap, pixelX, pixelY);
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
