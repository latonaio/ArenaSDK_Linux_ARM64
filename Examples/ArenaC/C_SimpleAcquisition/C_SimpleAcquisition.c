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
#include <inttypes.h>

#define TAB1 "  "

/// Simple Acquisition
//    This examples demonstrates the most basic code path of acquiring an image
//    using Arena C. This includes device streaming, image acquisition, and clean
//    up.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Image timeout
//    Timeout for grabbing images (in milliseconds). If no image is available at
//    the end of the timeout, the example will return an error. The timeout is
//    the maximum time to wait for an image; however, getting an image will
//    return as soon as an image is available, not waiting the full extent of the
//    timeout.
#define TIMEOUT 2000

// Update Timeout
//    Timeout for detecting camera devices (in milliseconds). If no device is
//    discovered at the end of the timeout, the example will return an error. The
//    timeout is the maximum time to wait to detect a device; the example will
//    resume once the internal list of devices has been updated, not waiting the
//    full extent of the timeout.
#define SYSTEM_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates simplest route to acquiring an image
// (1) start stream
// (2) acquires image
// (3) cleans up
AC_ERROR AcquireImages(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Start stream
	//    Start the stream before grabbing any images. Starting the stream
	//    allocates buffers, which can be passed in as an argument (default: 10),
	//    and begins filling them with data. Starting the stream blocks write
	//    access to many features such as width, height, and pixel format, as
	//    well as acquisition and buffer handling modes, among others. The stream
	//    needs to be stopped later.
	printf("%sStart stream\n", TAB1);

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Get image
	//    Retrieve images after the stream has started. If the timeout expires
	//    before an image is retrieved, example will return an error. Because of
	//    this, the timeout should be at least a bit larger than the exposure
	//    time.
	printf("%sAcquire image\n", TAB1);

	acBuffer hBuffer = NULL;

	err = acDeviceGetBuffer(hDevice, TIMEOUT, &hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Requeue image buffer
	//    Requeue an image buffer when access to it is no longer needed. Notice
	//    that failing to requeue buffers may cause memory to leak and may also
	//    result in the stream engine being starved due to there being no
	//    available buffers.

	err = acDeviceRequeueBuffer(hDevice, hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Stop stream
	//    Stop the stream after all images have been requeued. Failing to stop
	//    the stream will leak memory.
	err = acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	return AC_ERR_SUCCESS;
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
	printf("C_SimpleAcquisition\n");
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
	else
	{
		acDevice hDevice = NULL;
		err = acSystemCreateDevice(hSystem, 0, &hDevice);
		CHECK_RETURN;

		// run example
		printf("Commence example\n\n");
		err = AcquireImages(hDevice);
		CHECK_RETURN;

		// clean up example
		printf("%sClean Up Arena\n", TAB1);
		err = acSystemDestroyDevice(hSystem, hDevice);
		CHECK_RETURN;
		err = acCloseSystem(hSystem);
		CHECK_RETURN;

		printf("\nExample complete\n");
	}

	printf("Press enter to complete\n");
	getchar();
	return 0;
}
