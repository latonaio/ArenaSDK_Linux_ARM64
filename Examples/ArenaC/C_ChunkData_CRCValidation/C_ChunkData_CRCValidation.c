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

// Chunk Data: CRC Validation
//    This example demonstrates the use of chunk data to verify data through a
//    Cyclical Redundancy Check (or CRC for short). CRCs are meant to check the
//    validity of sent data. It is performed by doing a series of calculations on
//    the raw data before and after it is sent. If the resultant integer values
//    match, then it is safe to assume the integrity of the data.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// image timeout
#define TIMEOUT 2000

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// configure device to receive CRC chunk and verify CRC
AC_ERROR ConfigureAndValidateCRC(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get node initial values that will be changed in order to return their
	// values at the end of the example
	acNode hChunkModeActiveNode = NULL;
	bool8_t chunkModeActiveInitial = false;

	err = acNodeMapGetNode(hNodeMap, "ChunkModeActive", &hChunkModeActiveNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acBooleanGetValue(hChunkModeActiveNode, &chunkModeActiveInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	// chunk enable node
	acNode hChunkEnableNode = NULL;
	bool8_t chunkModeEnableInitial = false;

	err = acNodeMapGetNode(hNodeMap, "ChunkEnable", &hChunkEnableNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acBooleanGetValue(hChunkEnableNode, &chunkModeEnableInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Activate CRC chunk
	//    Activate chunk data and enable the CRC chunk before starting the
	//    stream. This involves activating chunk mode, selecting the CRC chunk,
	//    and enabling it.
	printf("%sActivate chunk mode and enable CRC chunk\n", TAB1);

	// activate chunk mode
	err = acBooleanSetValue(hChunkModeActiveNode, true);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set chunk selector
	err = acNodeMapSetEnumerationValue(hNodeMap, "ChunkSelector", "CRC");
	if (err != AC_ERR_SUCCESS)
		return err;

	// enable CRC chunk
	err = acBooleanSetValue(hChunkEnableNode, true);
	if (err != AC_ERR_SUCCESS)
		return err;

	// start stream
	printf("%sStart stream and acquire image\n", TAB1);

	err = acDeviceStartStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// acquire images and chunks
	acBuffer hBuffer = NULL;

	err = acDeviceGetBuffer(hDevice, TIMEOUT, &hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Check for completeness
	//    If an image is incomplete, it could be the case that the chunks hold
	//    garbage data value. If incomplete, chunks can still be retrieved but
	//    should be checked against null to verify the data exists.
	printf("%sCheck for completeness\n", TAB2);
	bool8_t isIncomplete = false;

	err = acBufferIsIncomplete(hBuffer, &isIncomplete);
	if (err != AC_ERR_SUCCESS)
		return err;
	if (isIncomplete)
	{
		printf("\n\nError: Payload data incomplete. Please review network \nconfigurations, increase "
			   "packet size, increase inter-packet \ndelay and/or reduce image size, then retry example\n");
		return err;
	}

	// Calculate CRC from data
	//    The CRC can be calculated using a global Arena function. This global
	//    function is called to calculate the CRC internally when verifying the
	//    CRC.
	printf("%sCalculate CRC from data", TAB2);

	// get image width
	size_t width = 0;

	err = acImageGetWidth(hBuffer, &width);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get image height
	size_t height = 0;

	err = acImageGetHeight(hBuffer, &height);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get bits per pixel
	size_t bitsPerPixel = 0;

	err = acImageGetBitsPerPixel(hBuffer, &bitsPerPixel);
	if (err != AC_ERR_SUCCESS)
		return err;

	// calculate image data size
	size_t imageDataSize = width * height * bitsPerPixel / 8;

	// get image data from buffer
	uint8_t* pData = 0;

	err = acImageGetData(hBuffer, &pData);
	if (err != AC_ERR_SUCCESS)
		return err;

	// calculate CRC
	size_t dataCRCValue = 0;

	err = acCalculateCRC32(pData, imageDataSize, &dataCRCValue);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf(" (%zu)\n", dataCRCValue);

	// Retrieve CRC from chunk
	//    When the chunk is enabled, the CRC is calculated by the device and sent
	//    as a piece of chunk data. The CRC value can then be retrieved as a
	//    chunk.
	acNode hChunkCRCNode = NULL;

	err = acChunkDataGetChunk(hBuffer, "ChunkCRC", &hChunkCRCNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%sRetrieve CRC from chunk ", TAB2);
	int64_t chunkCRCValue = 0;

	err = acIntegerGetValue(hChunkCRCNode, &chunkCRCValue);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf(" (%" PRIu64 ")\n", chunkCRCValue);

	// Compare CRCs
	//    Compare CRCs to verify whether the data is correct. If the calculated
	//    CRC and the chunk CRC match, then the data is assumed valid and
	//    correct; otherwise, invalid and incorrect.
	printf("%sCompare chunk CRC to calculated CRC\n", TAB2);
	size_t chunkCRC_size_t = (size_t)chunkCRCValue;

	if (chunkCRC_size_t == dataCRCValue)
		printf("%sCRCs match, data correct\n", TAB3);
	else
		printf("%sCRCs do not match, data incorrect\n", TAB3);

	// Validate CRC automatically
	//    The easiest way to validate a CRC is by using the function provided by
	//    Arena. The functions that verify CRC go through the same steps just
	//    explained above.
	printf("%sValidate CRC automatically\n", TAB2);
	bool8_t verifyCRC = true;

	err = acBufferVerifyCRC(hBuffer, &verifyCRC);
	if (verifyCRC)
		printf("%sCRCs verified (CRCs match, data correct)\n", TAB3);
	else
		printf("%sCRCs invalid (CRCs do not match, data incorrect)\n", TAB3);

	// requeue buffer
	printf("%sRequeue buffer and stop stream\n", TAB2);

	err = acDeviceRequeueBuffer(hDevice, hBuffer);
	if (err != AC_ERR_SUCCESS)
		return err;

	// stop stream
	err = acDeviceStopStream(hDevice);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return nodes to their initial values
	err = acBooleanSetValue(hChunkModeActiveNode, chunkModeActiveInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acBooleanSetValue(hChunkEnableNode, chunkModeEnableInitial);
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
	printf("C_ChunkData_CRCValidation\n");
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
		printf("\nNo camera connected\nPress enter to complete\n");
		getchar();
		return -1;
	}
	acDevice hDevice = NULL;
	err = acSystemCreateDevice(hSystem, 0, &hDevice);
	CHECK_RETURN;

	// run example
	printf("Commence example\n\n");
	err = ConfigureAndValidateCRC(hDevice);
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
