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

#define TAB1 "  "

// Streamables
//    This example introduces streamables, which uses files to pass settings
//    around between devices. This example writes all streamable features from a
//    source device to a file, and then writes them from the file to all other
//    connected devices.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// the name of the file to stream features to/from
#define FILE_NAME "allStreamableFeatures.txt"

// maximum number of devices to stream features to
#define MAX_DEVICES 10

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates streamable features
// (1) reads all streamable features from source device
// (2) writes features to file
// (3) reads features from file
// (4) writes features to destination devices
AC_ERROR WriteAndReadStreamables(acDevice hSrcDevice, acDevice* phDstDevices, size_t numDevices)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Write features to file
	//    Write streamable features from a device to a file. Each node map
	//    requires a separate feature stream object. When writing to a file, if
	//    no features are explicitly selected, all will be written by default.
	printf("%sSave features from device 0 to %s\n", TAB1, FILE_NAME);

	// get node map
	acNodeMap hNodeMapSrc = NULL;

	err = acDeviceGetNodeMap(hSrcDevice, &hNodeMapSrc);
	if (err != AC_ERR_SUCCESS)
		return err;

	// create feature stream object
	acFeatureStream hFeatureStreamSrc = NULL;

	err = acFeatureStreamCreate(hNodeMapSrc, &hFeatureStreamSrc);
	if (err != AC_ERR_SUCCESS)
		return err;

	// stream features to file
	err = acFeatureStreamWriteFileName(hFeatureStreamSrc, FILE_NAME);
	if (err != AC_ERR_SUCCESS)
		return err;

	// destroy feature stream object
	err = acFeatureStreamDestroy(hFeatureStreamSrc);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Read features to devices
	//    Read streamable features from from a file to the rest of the devices.
	//    Again, each node map requires a separate feature stream object. When
	//    reading from a file, all features saved to the file will be loaded to
	//    the device. If a device does not have a feature, it is skipped.
	size_t i;
	for (i = 0; i < numDevices; i++)
	{
		printf("%sLoad features from %s to device %zu\n", TAB1, FILE_NAME, i);

		// get node map
		acNodeMap hNodeMapDst = NULL;

		err = acDeviceGetNodeMap(phDstDevices[i], &hNodeMapDst);
		if (err != AC_ERR_SUCCESS)
			return err;

		// create feature stream object
		acFeatureStream hFeatureStreamDst = NULL;

		err = acFeatureStreamCreate(hNodeMapDst, &hFeatureStreamDst);
		if (err != AC_ERR_SUCCESS)
			return err;

		// load features from file
		err = acFeatureStreamReadFileName(hFeatureStreamDst, FILE_NAME);
		if (err != AC_ERR_SUCCESS)
			return err;

		// destroy feature stream object
		err = acFeatureStreamDestroy(hFeatureStreamDst);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

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
	printf("C_Streamables\n");
	printf("Example may change device settings -- proceed? ('y' to continue) ");
	char continueExample = getchar();
	getchar(); // newline

	if (continueExample == 'y')
	{
		AC_ERROR err = AC_ERR_SUCCESS;

		// prepare example
		acSystem hSystem = NULL;
		err = acOpenSystem(&hSystem);
		CHECK_RETURN;
		err = acSystemUpdateDevices(hSystem, 100);
		CHECK_RETURN;
		size_t numDevices = 0;
		err = acSystemGetNumDevices(hSystem, &numDevices);
		CHECK_RETURN;
		if (numDevices == 0)
		{
			printf("\nNo camera connected, example requires at least 1 camera\n");
			printf("Press enter to complete\n");
			getchar();
			return -1;
		}
		else if (numDevices == 1)
		{
			printf("Warning: only one camera connected, example runs best with at least 2 cameras\n");
		}
		else if (numDevices > MAX_DEVICES)
		{
			printf("Warning: too many cameras, example set to run with only %d cameras\n", MAX_DEVICES);
		}

		acDevice hSrcDevice = NULL;
		acDevice phDstDevices[MAX_DEVICES - 1];
		size_t i = 0;
		for (i = 0; i < (numDevices <= MAX_DEVICES ? numDevices : MAX_DEVICES); i++)
		{
			if (i == 0)
			{
				err = acSystemCreateDevice(hSystem, i, &hSrcDevice);
				CHECK_RETURN;
			}
			else
			{
				err = acSystemCreateDevice(hSystem, i, &(phDstDevices[i - 1]));
				CHECK_RETURN;
			}
		}

		// run example
		printf("Commence example\n\n");
		err = WriteAndReadStreamables(hSrcDevice, phDstDevices, (numDevices <= MAX_DEVICES ? numDevices - 1 : MAX_DEVICES - 1));
		CHECK_RETURN;
		printf("\nExample complete\n");

		// clean up example
		err = acSystemDestroyDevice(hSystem, hSrcDevice);
		CHECK_RETURN;

		for (i = 0; i < numDevices - 1; i++)
		{
			err = acSystemDestroyDevice(hSystem, phDstDevices[i]);
		}
		err = acCloseSystem(hSystem);
		CHECK_RETURN;
	}

	printf("Press enter to complete\n");
	getchar();
	return 0;
}
