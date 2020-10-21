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
#define TAB2 "    "
#define TAB3 "      "
#define TAB4 "        "

// Callbacks: On Node Change
//    This example demonstrates configuring a callback to be invoked when a node
//    is invalidated. A node is invalidated when its value changes or can be
//    invalidated manually. In this example, a callback is registered on
//    PayloadSize. The example shows two ways to invoke a callback: first by
//    changing the value of a dependent node (Height) and then by invalidating
//    PayloadSize manually. Whenever the callback is triggered, the callback
//    function prints the updated value of the invalidated node.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Height values
//    The example changes the value of the height node twice to ensure that the
//    height is changed.
#define HEIGHT_ONE 256
#define HEIGHT_TWO 512

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// system timeout
#define SYSTEM_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// callback function
// (1) prints information from the callback
void PrintNodeValue(acNode hNode, void* pUserData)
{

	// Do something on callback
	//    This function is registered with the callback and invoked when the
	//    registered node is invalidated. In this case, the callback prints the
	//    display name and value of the invalidated node.
	printf("%sMessage from callback\n", TAB3);

	// get node display name
	char pDisplayNameBuf[MAX_BUF];
	size_t displayNameBufLen = MAX_BUF;

	acNodeGetDisplayName(hNode, pDisplayNameBuf, &displayNameBufLen);

	// print PayloadSize value
	char pValueBuf[MAX_BUF];
	size_t valueBufLen = MAX_BUF;

	acValueToString(hNode, pValueBuf, &valueBufLen);
	printf("%s%s: %s\n", TAB4, pDisplayNameBuf, pValueBuf);

	// print device serial number from pUserData
	char* pSerialNumberUserData = (char*)pUserData;

	printf("%sDevice Serial Number: %s\n", TAB4, pSerialNumberUserData);
}

// demonstrates callbacks invoked on node changes
// (1) registers callback on node PayloadSize
// (2) changes Height twice to invalidate PayloadSize, invoking callback
// (3) invalidates PayloadSize manually
// (4) deregisters callback
AC_ERROR ConfigureAndCauseCallback(acSystem hSystem, acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Register PayloadSize for callbacks
	//    Callbacks are registered with a node and a function. This example
	//    demonstrates callbacks being invoked when the node is invalidated. This
	//    could be when the node value changes, either manually or by the device,
	//    or the node is invalidated manually.
	printf("%sRegister callback on PayloadSize\n", TAB1);

	// get payload size node
	acNode hPayloadSizeNode = NULL;

	err = acNodeMapGetNode(hNodeMap, "PayloadSize", &hPayloadSizeNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get device serial number to retrieve during callback. This could be used
	// to identify devices among multiple cameras.
	size_t index = 0;
	char pSerialNumberBuf[MAX_BUF];
	size_t bufLen = MAX_BUF;

	err = acSystemGetDeviceSerial(hSystem, index, pSerialNumberBuf, &bufLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	// register callback
	acCallback hCallback = NULL;

	err = acCallbackRegister(&hCallback, hPayloadSizeNode, PrintNodeValue, &pSerialNumberBuf);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Modify Height to invalidate and invoke callback on PayloadSize
	//    The value of PayloadSize depends on a number of other nodes, including
	//    Height, Width, etc. Therefore, changing the value of Height changes the
	//    value of and invalidates PayloadSize, which then invokes the callback.
	acNode hHeightNode = NULL;

	err = acNodeMapGetNode(hNodeMap, "Height", &hHeightNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get initial height node value that will be changed and returned at the end
	// of the example
	int64_t heightValueInitial = 0;

	err = acIntegerGetValue(hHeightNode, &heightValueInitial);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set new height values
	printf("%sChange height once\n", TAB2);

	err = acIntegerSetValue(hHeightNode, HEIGHT_ONE);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%sChange height twice\n", TAB2);

	err = acIntegerSetValue(hHeightNode, HEIGHT_TWO);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Manually invalidate PayloadSize for callback
	//    Apart from changing the value of a node, nodes can be invalidated
	//    manually by calling InvalidateNode. This also invokes the callback.
	printf("%sInvalidate PayloadSize\n", TAB2);

	err = acNodeInvalidateNode(hPayloadSizeNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Deregister callback
	//    Failing to deregister a callback results in a memory leak. Once a
	//    callback has been registered, it will no longer be invoked when a node
	//    is invalidated.
	printf("%sDeregister callback\n", TAB2);

	err = acCallbackDeregister(hCallback);
	if (err != AC_ERR_SUCCESS)
		return err;

	// return height node to initial value
	err = acIntegerSetValue(hHeightNode, heightValueInitial);
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
	printf("C_Callback_OnNodeChange\n");
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
	err = ConfigureAndCauseCallback(hSystem, hDevice);
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
