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
#include <string.h>	  // defines strcmp functions

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Explore: Node Types
//    This example explores the different properties of various node's types
//    including boolean, string, enumeration, integer, and float nodes. The user
//    inputs the node name that they wish to access (leaving out spacing between
//    words) in order to retrieve the node properties, or inputs 'x' to exit.
//    This example includes commented out code that sets values for each node
//    type, retrieves entry by node name for enumeration types, and imposes
//    maximum and minimum values for integer and float types. See
//    Cpp_Explore_Nodes for a complete list of nodes and their respective types.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// timeout for detecting camera devices (in milliseconds).
#define DEVICE_TIMEOUT 100

// maximum buffer length
#define MAX_BUF 256

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// explores nodes of boolean type
// (1) retrieves value
// (2) demonstrates value setter in a comment
AC_ERROR ExploreBoolean(acNodeMap hNodeMap, acNode hNode, const char* nodeName)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Retrieve value
	bool8_t value = false;

	err = acNodeMapGetBooleanValue(hNodeMap, nodeName, &value);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sValue: %i\n", TAB3, value);

	// Set value
	//{
	//	bool8_t boolValue = true;
	//	err = acBooleanSetValue(hNode, boolValue);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;
	//}

	return err;
}

// explores nodes of string type
// (1) retrieves value
// (2) retrieves maximum value length
// (3) demonstrates value setter in a comment
AC_ERROR ExploreString(acNode hNode)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Retrieve value
	char pValue[MAX_BUF];
	size_t bufLen = MAX_BUF;

	err = acStringGetValue(hNode, pValue, &bufLen);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sValue: %s\n", TAB3, pValue);

	// Retrieve maximum length
	int64_t maxLength = MAX_BUF;

	err = acStringGetMaxLength(hNode, &maxLength);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sMaximum Length: %" PRIu64 "\n", TAB3, maxLength);

	// Set value
	//{
	//	char stringValue[MAX_BUF] = "string value";
	//	err = acStringSetValue(hNode, stringValue);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;
	//}

	return err;
}

// explores nodes of type enumeration
// (1) retrieves currently set symbolic node
// (2) retrieves integer value
// (3) retrieves list of symbolic entries
// (4) demonstrates enumeration node retrieval by name in a comment
// (5) demonstrates value setter in a comment
AC_ERROR ExploreEnumeration(acNode hNode, const char* nodeName)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Retrieve current entry
	char pCurrentBuf[MAX_BUF];
	size_t currentBufLen = MAX_BUF;

	err = acEnumerationGetCurrentSymbolic(hNode, pCurrentBuf, &currentBufLen);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sCurrent entry: %s\n", TAB3, pCurrentBuf);

	// Retrieve integer value
	acNode hEntryNode = NULL;
	int64_t intValue = 0;

	err = acEnumerationGetCurrentEntry(hNode, &hEntryNode);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acEnumEntryGetIntValue(hEntryNode, &intValue);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sInteger value: %" PRId64 "\n", TAB3, intValue);

	// Retrieve symbolic entries
	size_t numEntries = 0;

	err = acEnumerationGetNumEntries(hNode, &numEntries);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sEntries:", TAB3);

	for (size_t index = 0; index < numEntries; index++)
	{
		// get entry by index
		char pSymbolicBuf[MAX_BUF];
		size_t symbolicBufLen = MAX_BUF;

		err = acEnumerationGetSymbolicByIndex(hNode, index, pSymbolicBuf, &symbolicBufLen);
		if (err != AC_ERR_SUCCESS)
			return err;
		if (index > 0)
			printf(", ");
		printf("%s", pSymbolicBuf);
	}
	printf("\n");

	// Retrieve entry by name
	//{
	//	acNode hEnumerationEntryNode = NULL;
	//	err = acEnumerationGetEntryByName(hEntryNode, nodeName, &hEnumerationEntryNode);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;
	//}

	// Set value
	//{
	//	int64_t intValue = 0;
	//	err = acEnumerationSetByIntValue(hNode, intValue);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;
	//}

	return err;
}

// explores nodes of type integer
// (1) retrieves value
// (2) retrieves maximum and minimum
// (3) retrieves increment and increment mode
// (4) retrieves representation
// (5) retrieves unit
// (6) demonstrates maximum and minimum imposition in a comment
// (7) demonstrates value setter in a comment
AC_ERROR ExploreInteger(acNode hNode)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Retrieve value
	int64_t value = 0;

	err = acIntegerGetValue(hNode, &value);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sValue: %" PRId64 "\n", TAB3, value);

	// Retrieve range
	int64_t pMaximum = 0;
	int64_t pMinimum = 0;

	err = acIntegerGetMax(hNode, &pMaximum);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acIntegerGetMin(hNode, &pMinimum);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sMaximum, minimum: %" PRId64 ", %" PRId64 "\n", TAB3, pMaximum, pMinimum);

	// Retrieve increment
	int64_t increment = 0;

	err = acIntegerGetInc(hNode, &increment);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sIncrement (mode): %" PRIu64 "\n", TAB3, increment);

	// Retrieve representation
	AC_REPRESENTATION representation = 0;

	err = acIntegerGetRepresentation(hNode, &representation);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sRepresentation: %" PRIu32 "\n", TAB3, representation);

	// Retrieve unit
	char pUnitBuf[MAX_BUF];
	size_t intUnitBufLen = MAX_BUF;

	err = acIntegerGetUnit(hNode, pUnitBuf, &intUnitBufLen);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sUnit: %s\n", TAB3, pUnitBuf);

	// Impose maximum and minimum
	//{
	//	int64_t imposedMinimum = 0;
	//	int64_t imposedMaximum = 10;
	//	err = acIntegerImposeMin(hNode, imposedMinimum);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;
	//
	//	err = acIntegerImposeMax(hNode, imposedMaximum);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;
	//}

	// Set value
	//{
	//	int64_t intValue = 0;
	//	err = acIntegerSetValue(hNode, intValue);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;
	//}

	return err;
}

// explores nodes of type float
// (1) retrieves value
// (2) retrieves maximum and minimum
// (3) retrieves increment and increment mode
// (4) retrieves representation
// (5) retrieves unit
// (6) retrieves display notation
// (7) retrieves display precision
// (8) demonstrates maximum and minimum imposition in a comment
// (9) demonstrates value setter in a comment
AC_ERROR ExploreFloat(acNodeMap hNode)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// Retrieve value
	double value = 0;

	err = acFloatGetValue(hNode, &value);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sValue: %.0f \n", TAB3, value);

	// Retrieve range
	double maximum = 0;
	double minimum = 0;

	err = acFloatGetMax(hNode, &maximum);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acFloatGetMin(hNode, &minimum);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sMaximum, minimum: %.0lf, %.0lf \n", TAB3, maximum, minimum);

	// Retrieve increment
	bool8_t hasInc = false;

	err = acFloatHasInc(hNode, &hasInc);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sIncrement (mode): ", TAB3);
	if (hasInc)
	{
		double increment = 0;
		err = acFloatGetInc(hNode, &increment);
		if (err != AC_ERR_SUCCESS)
			return err;
		printf("%lf\n", increment);
	}
	else
		printf("no increment\n");

	// Retrieve representation
	AC_REPRESENTATION representation = 0;

	err = acFloatGetRepresentation(hNode, &representation);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sRepresentation: %" PRIu32 "\n", TAB3, representation);

	// Retrieve unit
	char pUnitBuf[MAX_BUF];
	size_t bufLen = MAX_BUF;

	err = acFloatGetUnit(hNode, pUnitBuf, &bufLen);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sUnit: %s\n", TAB3, pUnitBuf);

	// Retrieve display notation
	AC_DISPLAY_NOTATION displayNotation = 0;

	err = acFloatGetDisplayNotation(hNode, &displayNotation);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sDisplay notation: %" PRIu32 "\n", TAB3, displayNotation);

	// Retrieve display precision
	int64_t displayPrecision = 0;

	err = acFloatGetDisplayPrecision(hNode, &displayPrecision);
	if (err != AC_ERR_SUCCESS)
		return err;
	printf("%sDisplay precision: %" PRIu64 "\n", TAB3, displayPrecision);

	// Impose maximum and minimum
	//{
	//	double imposedMinimum = 0;
	//	double imposedMaximum = 10;

	//	err = acFloatImposeMin(hNode, imposedMinimum);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;

	//	err = acFloatImposeMax(hNode, imposedMaximum);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;
	//}

	// Set value
	//{
	//	double floatValue = 0;
	//	err = acFloatSetValue(hNode, floatValue);
	//	if (err != AC_ERR_SUCCESS)
	//		return err;
	//}

	return err;
}

// controls node exploration
AC_ERROR ExploreNodes(acDevice hDevice)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get device node map from device
	acNodeMap hNodeMap = NULL;

	err = acDeviceGetNodeMap(hDevice, &hNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	// stay in loop until exit
	while (true)
	{
		// 'x' to exit
		char nodeName[MAX_BUF];

		printf("%sInput node name to explore ('x' to exit)\n%s", TAB1, TAB2);
		if ((fgets(nodeName, sizeof nodeName, stdin)) != NULL)
			;
		if (0 == strcmp(nodeName, "x\n"))
		{
			printf("%sExit\n", TAB2);
			break;
		}

		// change end of string character in nodeName so it is accepted as input
		acNode hNode = NULL;
		size_t length = strlen(nodeName);

		if (length > 0 && nodeName[length - 1] == '\n')
		{
			nodeName[length - 1] = '\0';
		}

		// get node
		err = acNodeMapGetNode(hNodeMap, nodeName, &hNode);
		if (err != AC_ERR_SUCCESS)
			return err;
		if (hNode)
		{
			// get node interface type
			AC_INTERFACE_TYPE interfaceType = 0;
			err = acNodeGetPrincipalInterfaceType(hNode, &interfaceType);
			if (err != AC_ERR_SUCCESS)
				return err;

			// explore by type
			switch (interfaceType)
			{
				case AC_INTERFACE_TYPE_BOOLEAN:
					ExploreBoolean(hNodeMap, hNode, nodeName);
					break;
				case AC_INTERFACE_TYPE_STRING:
					ExploreString(hNode);
					break;
				case AC_INTERFACE_TYPE_ENUMERATION:
					ExploreEnumeration(hNode, nodeName);
					break;
				case AC_INTERFACE_TYPE_INTEGER:
					ExploreInteger(hNode);
					break;
				case AC_INTERFACE_TYPE_FLOAT:
					ExploreFloat(hNode);
					break;
				default:
					printf("%s%s type not found\n", TAB3, nodeName);
			}
		}
		else
			printf("%s%s not found\n", TAB3, nodeName);
	}
	return 0;
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
	printf("C_Explore_NodeTypes\n");
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
		return -1;
	}
	acDevice hDevice = NULL;
	err = acSystemCreateDevice(hSystem, 0, &hDevice);
	CHECK_RETURN;

	// run example
	printf("Commence example\n\n");
	err = ExploreNodes(hDevice);
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
