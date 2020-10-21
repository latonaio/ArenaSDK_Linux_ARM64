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
#include <inttypes.h> // defines macros for printf functions
#include <string.h>	  // defines strcmp functions

#define TAB1 "  "
#define TAB2 "    "

// User sets
//    This example introduces user sets, a feature which allows for the saving
//    and loading of multiple configurations. The example starts by changing two
//    features, width and height. The device configuration is then saved to user
//    set 1. The default user set is then loaded, followed by user set 1 again to
//    demonstrate the the device configuration changing back and forth.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// system timeout
#define SYSTEM_TIMEOUT 100

// width value to save/load
#define WIDTH 576

// height value to load/save
#define HEIGHT 512

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates user sets
// (1) gets width and height nodes
// (2) changes width and height
// (3) saves user set with width and height changes
// (4) loads default user set, changing width and height to defaults
// (5) loads user set, changing width and height back
AC_ERROR SaveAndLoadUserSets(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Get width and height nodes
	//    This example focuses on width and height nodes to demonstrate the
	//    saving and loading of user sets.
	printf("%sGet width and height nodes\n", TAB1);
	acNode hWidthNode = NULL;
	acNode hHeightNode = NULL;

	err = acNodeMapGetNode(hNodeMap, "Width", &hWidthNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapGetNode(hNodeMap, "Height", &hHeightNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set width and height
	err = acIntegerSetValue(hWidthNode, WIDTH);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acIntegerSetValue(hHeightNode, HEIGHT);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Save to user set 1
	//    Saving the user set saves the new width and height values. Saving a
	//    user set involves selecting the user set to save to on the selector
	//    node and then executing the save on the command node.
	printf("%sSave to user set 1\n", TAB1);

	// select user set 1
	err = acNodeMapSetEnumerationValue(hNodeMap, "UserSetSelector", "UserSet1");
	if (err != AC_ERR_SUCCESS)
		return err;

	// execute the save
	err = acNodeMapExecute(hNodeMap, "UserSetSave");
	if (err != AC_ERR_SUCCESS)
		return err;

	// get new width and height
	int64_t widthValue = 0;
	int64_t heightValue = 0;

	err = acIntegerGetValue(hWidthNode, &widthValue);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acIntegerGetValue(hHeightNode, &heightValue);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%sWidth %" PRId64 "\n", TAB2, widthValue);
	printf("%sHeight %" PRId64 "\n", TAB2, heightValue);

	// Load default user set
	//    Loading a user set is quite similar to saving: select the user set to
	//    load using the selector node and then execute the load using the
	//    command node. Notice that loading the default user set changes the
	//    width and height values.
	printf("%sLoad default user set\n", TAB1);

	// select default user set
	err = acNodeMapSetEnumerationValue(hNodeMap, "UserSetSelector", "Default");
	if (err != AC_ERR_SUCCESS)
		return err;

	// execute the load
	err = acNodeMapExecute(hNodeMap, "UserSetLoad");
	if (err != AC_ERR_SUCCESS)
		return err;

	// check width and height
	err = acIntegerGetValue(hWidthNode, &widthValue);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acIntegerGetValue(hHeightNode, &heightValue);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%sWidth %" PRId64 "\n", TAB2, widthValue);
	printf("%sHeight %" PRId64 "\n", TAB2, heightValue);

	// Load user set 1
	//    Load user set 1 to return width and height to the values that were
	//    saved earlier. The process is the same as above.
	printf("%sLoad user set 1\n", TAB1);

	// select user set 1
	err = acNodeMapSetEnumerationValue(hNodeMap, "UserSetSelector", "UserSet1");
	if (err != AC_ERR_SUCCESS)
		return err;

	// execute the load
	err = acNodeMapExecute(hNodeMap, "UserSetLoad");
	if (err != AC_ERR_SUCCESS)
		return err;

	// check width and height
	err = acIntegerGetValue(hWidthNode, &widthValue);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acIntegerGetValue(hHeightNode, &heightValue);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf("%sWidth %" PRId64 "\n", TAB2, widthValue);
	printf("%sHeight %" PRId64 "\n", TAB2, heightValue);

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
	printf("C_UserSets\n");
	AC_ERROR err = AC_ERR_SUCCESS;

	// user prompt for possible device settings overwrite
	printf("Example may overwrite device settings saved to user set 1 -- proceed? ('y' to continue) ");
	char continueExample[MAX_BUF];

	if ((fgets(continueExample, sizeof continueExample, stdin)) != NULL)
		;
	if (0 == strcmp(continueExample, "y\n"))
	{

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
		err = SaveAndLoadUserSets(hDevice);
		CHECK_RETURN;
		printf("\nExample complete\n");

		// clean up example
		err = acSystemDestroyDevice(hSystem, hDevice);
		CHECK_RETURN;
		err = acCloseSystem(hSystem);
		CHECK_RETURN;
	}

	printf("Press enter to complete\n");
	getchar();
	return -1;
}
