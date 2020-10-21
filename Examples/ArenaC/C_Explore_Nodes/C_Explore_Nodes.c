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
#include <inttypes.h> //defines macros for printf functions
#include <stdbool.h>  // defines boolean type and values

// Explore: Nodes
//    This example explores traversing the nodes as a tree and fundamental node
//    information including display name, access mode visibility, interface type,
//    and value.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Choose node types to explore
#define EXPLORE_ACCESS true
#define EXPLORE_VISIBILITY true
#define EXPLORE_TYPE true
#define EXPLORE_VALUE true

// Device Timeout
//    Timeout for detecting camera devices (in milliseconds). If no device is
//    discovered at the end of the timeout, the example will return an error. The
//    timeout is the maximum time to wait to detect a device; the example will
//    resume once the internal list of devices has been updated, not waiting the
//    full extent of the timeout.
#define DEVICE_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// maintains display indentation
void Indent(size_t depth)
{
	for (size_t i = 0; i < depth; i++)
		printf(" ");
}
// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// explores node
// (1) retrieves display name
// (2) retrieves accessibility
// (3) retrieves visibility
// (4) retrieves interface type
// (5) retrieves value
AC_ERROR ExploreNode(acSystem hSystem, acDevice hDevice, acNode hNode, size_t depth)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Retrieve display name
	char pDisplayNameBuf[MAX_BUF];
	size_t bufLen = MAX_BUF;
	err = acNodeGetDisplayName(hNode, pDisplayNameBuf, &bufLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Retrieve accessibility
	AC_ACCESS_MODE accessMode = 0;
	err = acNodeGetAccessMode(hNode, &accessMode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Retrieve visibility
	AC_VISIBILITY visibility = 0;
	err = acNodeGetVisibility(hNode, &visibility);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Retrieve interface type
	AC_INTERFACE_TYPE interfaceType = 0;
	err = acNodeGetPrincipalInterfaceType(hNode, &interfaceType);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Retrieve value
	bool8_t isReadable = true;
	err = acIsReadable(hNode, &isReadable);
	if (err != AC_ERR_SUCCESS)
		return err;

	char pValueBuf[MAX_BUF];
	size_t valueBufLen = MAX_BUF;
	err = acValueToString(hNode, pValueBuf, &valueBufLen);
	if (err != AC_ERR_SUCCESS)
		return err;

	// print node information
	Indent(depth);
	printf("%s", pDisplayNameBuf);

	if (EXPLORE_ACCESS)
	{
		Indent(50 - (depth + bufLen));
		printf("%" PRId32, accessMode);
	}

	if (EXPLORE_VISIBILITY)
	{
		Indent(10);
		printf("%" PRId32, visibility);
	}

	if (EXPLORE_TYPE)
	{
		Indent(10);
		printf("%" PRId32, interfaceType);
	}

	if (EXPLORE_VALUE)
	{
		Indent(10);

		if (!isReadable)
			printf("-");
		else if (valueBufLen > 50)
			printf("...");
		else
			printf("%s", pValueBuf);
	}
	printf("\n");

	if (interfaceType == AC_INTERFACE_TYPE_CATEGORY)
	{
		size_t numFeatures = 0;
		err = acCategoryGetNumFeatures(hNode, &numFeatures);
		if (err != AC_ERR_SUCCESS)
			return err;
		acNode hFeatureNode = NULL;
		depth = (depth + 1);
		for (size_t index = 0; index < numFeatures; index++)
		{

			err = acCategoryGetFeature(hNode, index, &hFeatureNode);
			if (err != AC_ERR_SUCCESS)
				return err;

			ExploreNode(hSystem, hDevice, hFeatureNode, depth);
		}
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
	printf("C_Explore_Nodes\n");
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

	// begin example
	acNodeMap hNodeMap = NULL;
	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// get node by index
	size_t index = 0;
	acNode hNode = NULL;
	err = acNodeMapGetNodeByIndex(hNodeMap, index, &hNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	// run explore node example
	printf("Commence example\n\n");
	size_t depth = 0;
	err = ExploreNode(hSystem, hDevice, hNode, depth);
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
