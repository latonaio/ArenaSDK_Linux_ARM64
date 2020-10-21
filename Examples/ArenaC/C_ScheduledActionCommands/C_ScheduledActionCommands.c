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

#if defined _WIN32
#include <windows.h>
#elif defined linux
#include <unistd.h>
#endif

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "
#define ERASE_LINE "                            "

#if defined (_WIN32)
#define portable_sleep(x) Sleep(x * 1000)
#elif defined (__linux__)
#define portable_sleep(x) sleep(x)
#endif

 // Scheduled Action Commands
 //    This example introduces scheduling action commands on multiple cameras. The
 //    device settings are configured to allow each device to trigger a single
 //    image using action commands. The system is prepared to receive an action
 //    command and the devices' PTP relationships are synchronized. This allows
 //    actions commands to be fired across all devices, resulting in
 //    simultaneously acquired images with synchronized timestamps
 //    Depending on the initial PTP state of each camera, it can take about
 //    40 seconds for all devices to autonegotiate.

 // =-=-=-=-=-=-=-=-=-
 // =-=- SETTINGS =-=-
 // =-=-=-=-=-=-=-=-=-

 // delta time (ns)
#define DELTA_TIME 1000000000

// exposure time
#define EXPOSURE_TIME 500.0

// maximum number of devices to connect
#define MAX_DEVICES 10

// maximum buffer length
#define MAX_BUF 256

// system timeout
#define SYSTEM_TIMEOUT 100

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates action commands
// (1) manually sets exposure, trigger and action command settings
// (2) prepares devices for action commands
// (3) synchronizes devices and fire action command
// (4) retrieves images with synchronized timestamps
AC_ERROR SynchronizeCamerasAndTriggerImage(acSystem hSystem, acDevice* phDevices, size_t numDevices)
{
	AC_ERROR err = AC_ERR_SUCCESS;

	// get node map and retrieve initial node values that will be changed in
	// order to return their values at the end of the example
	acNodeMap hNodeMaps[MAX_DEVICES];
	char pExposureAutoInitials[MAX_DEVICES][MAX_BUF];
	double exposureTimeInitials[MAX_DEVICES] = { 0 };
	char triggerModeInitials[MAX_DEVICES][MAX_BUF];
	char triggerSourceInitials[MAX_DEVICES][MAX_BUF];
	char triggerSelectorInitials[MAX_DEVICES][MAX_BUF];
	char actionUnconditionalModeInitials[MAX_DEVICES][MAX_BUF];
	int64_t actionSelectorInitials[MAX_DEVICES] = { 0 };
	int64_t actionGroupKeyInitials[MAX_DEVICES] = { 0 };
	int64_t actionGroupMaskInitials[MAX_DEVICES] = { 0 };
	bool ptpEnableInitials[MAX_DEVICES] = { false };
	char transferControlModeInitials[MAX_DEVICES][MAX_BUF];
	int64_t packetSizeInitials[MAX_DEVICES] = { 0 };

	for (size_t i = 0; i < numDevices; i++)
	{
		size_t exposureAutoLen = MAX_BUF;
		size_t triggerModeBufLen = MAX_BUF;
		size_t triggerSourceBufLen = MAX_BUF;
		size_t triggerSelectorBufLen = MAX_BUF;
		size_t actionUnconditionalModeBufLen = MAX_BUF;
		size_t transferControlModeBufLen = MAX_BUF;

		err = acDeviceGetNodeMap(phDevices[i], &hNodeMaps[i]);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = // exposure
			acNodeMapGetEnumerationValue(hNodeMaps[i], "ExposureAuto", pExposureAutoInitials[i], &exposureAutoLen) |
			acNodeMapGetFloatValue(hNodeMaps[i], "ExposureTime", &exposureTimeInitials[i]) |
			// trigger
			acNodeMapGetEnumerationValue(hNodeMaps[i], "TriggerMode", triggerModeInitials[i], &triggerModeBufLen) |
			acNodeMapGetEnumerationValue(hNodeMaps[i], "TriggerSource", triggerSourceInitials[i], &triggerSourceBufLen) |
			acNodeMapGetEnumerationValue(hNodeMaps[i], "TriggerSelector", triggerSelectorInitials[i], &triggerSelectorBufLen) |
			// action commands
			acNodeMapGetEnumerationValue(hNodeMaps[i], "ActionUnconditionalMode", actionUnconditionalModeInitials[i], &actionUnconditionalModeBufLen) |
			acNodeMapGetIntegerValue(hNodeMaps[i], "ActionSelector", &actionSelectorInitials[i]) |
			acNodeMapGetIntegerValue(hNodeMaps[i], "ActionGroupKey", &actionGroupKeyInitials[i]) |
			acNodeMapGetIntegerValue(hNodeMaps[i], "ActionGroupMask", &actionGroupMaskInitials[i]) |
			// ptp
			acNodeMapGetBooleanValue(hNodeMaps[i], "PtpEnable", &ptpEnableInitials[i]) |
			// transfer control
			acNodeMapGetEnumerationValue(hNodeMaps[i], "TransferControlMode", transferControlModeInitials[i], &transferControlModeBufLen) |
			// packet size
			acNodeMapGetIntegerValue(hNodeMaps[i], "DeviceStreamChannelPacketSize", &packetSizeInitials[i]);

		if (err != AC_ERR_SUCCESS)
			printf("\nWarning: failed to retrieve one or more initial node values.\n");
	}

	// prepare all cameras
	printf("%sSetup\n", TAB1);

	for (size_t i = 0; i < numDevices; i++)
	{
		// get serial number
		char deviceSerialNumberBuf[MAX_BUF];
		size_t deviceSerialNumberBufLen = MAX_BUF;

		err = acNodeMapGetEnumerationValue(hNodeMaps[i], "DeviceSerialNumber", deviceSerialNumberBuf, &deviceSerialNumberBufLen);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%sPrepare camera %s\n", TAB2, deviceSerialNumberBuf);

		// Manually set exposure time
		//    In order to get synchronized images, the exposure time must be
		//    synchronized as well.
		double exposureTime = 0;
		printf("%sExposure: ", TAB3);

		err = acNodeMapSetEnumerationValue(hNodeMaps[i], "ExposureAuto", "Off");
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapSetFloatValue(hNodeMaps[i], "ExposureTime", EXPOSURE_TIME);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapGetFloatValue(hNodeMaps[i], "ExposureTime", &exposureTime);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%.3f \n", exposureTime);

		// Enable trigger mode and set source to action
		//    To trigger a single image using action commands, trigger mode must
		//    be enabled, the source set to an action command, and the selector
		//    set to the start of a frame.
		char triggerSourceBuf[MAX_BUF];
		size_t triggerSourceBufLen = MAX_BUF;
		printf("%sTrigger: ", TAB3);

		err = acNodeMapSetEnumerationValue(hNodeMaps[i], "TriggerMode", "On");
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapSetEnumerationValue(hNodeMaps[i], "TriggerSource", "Action0");
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapSetEnumerationValue(hNodeMaps[i], "TriggerSelector", "FrameStart");
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapGetEnumerationValue(hNodeMaps[i], "TriggerSource", triggerSourceBuf, &triggerSourceBufLen);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%s\n", triggerSourceBuf);

		// Prepare the device to receive an action command
		//    Action unconditional mode allows a camera to accept action from an
		//    application without write access. The device key, group key, and
		//    group mask must match similar settings in the system's TL node map.
		printf("%sAction commands: ", TAB3);

		err = acNodeMapSetEnumerationValue(hNodeMaps[i], "ActionUnconditionalMode", "On");
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapSetIntegerValue(hNodeMaps[i], "ActionSelector", 0);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapSetIntegerValue(hNodeMaps[i], "ActionDeviceKey", 1);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapSetIntegerValue(hNodeMaps[i], "ActionGroupKey", 1);
		if (err != AC_ERR_SUCCESS)
			return err;
		err = acNodeMapSetIntegerValue(hNodeMaps[i], "ActionGroupMask", 1);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("prepared\n");

		// Synchronize devices by enabling PTP
		//    Enabling PTP on multiple devices causes them to negotiate amongst
		//    themselves so that there is a single master device while all the
		//    rest become slaves. The slaves' clocks all synchronize to the
		//    master's clock.
		bool ptpEnable = false;
		printf("%sPTP: ", TAB3);

		err = acNodeMapSetBooleanValue(hNodeMaps[i], "PtpEnable", true);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapGetBooleanValue(hNodeMaps[i], "PtpEnable", &ptpEnable);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%s\n", ptpEnable ? "enabled" : "disabled");

		// Use max supported packet size. We use transfer control to ensure that only one camera
		// is transmitting at a time.
		acNodeMap TLStreamNodeMap;

		err = acDeviceGetTLStreamNodeMap(phDevices[i], &TLStreamNodeMap);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapSetBooleanValue(TLStreamNodeMap, "StreamAutoNegotiatePacketSize", true);
		if (err != AC_ERR_SUCCESS)
			return err;

		// Enable user controlled transfer control
		//    Synchronized cameras will begin transmiting images at the same time.
		//    To avoid missing packets due to collisions, we will use transfer control
		//    to control when each camera transmits the image.
		printf("%sTransfer Control: ", TAB3);

		err = acNodeMapSetEnumerationValue(hNodeMaps[i], "TransferControlMode", "UserControlled");
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapSetEnumerationValue(hNodeMaps[i], "TransferOperationMode", "Continuous");
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapExecute(hNodeMaps[i], "TransferStop");
		if (err != AC_ERR_SUCCESS)
			return err;

		char txControlMode[MAX_BUF];
		char txOperationMode[MAX_BUF];
		size_t txControlModeBufLen = MAX_BUF;
		size_t txOperationModeBufLen = MAX_BUF;

		err = acNodeMapGetEnumerationValue(hNodeMaps[i], "TransferControlMode", txControlMode, &txControlModeBufLen);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapGetEnumerationValue(hNodeMaps[i], "TransferOperationMode", txOperationMode, &txOperationModeBufLen);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%s - %s - Transfer Stopped\n", txControlMode, txOperationMode);
	}

	// prepare system
	acNodeMap hTLSystemNodeMap = NULL;
	printf("%sPrepare system\n", TAB2);

	// Prepare the system to broadcast an action command
	//    The device key, group key, group mask, and target IP must all match
	//    similar settings in the devices' node maps. The target IP acts as a
	//    mask.
	printf("%sAction commands: ", TAB3);

	err = acSystemGetTLSystemNodeMap(hSystem, &hTLSystemNodeMap);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapSetIntegerValue(hTLSystemNodeMap, "ActionCommandDeviceKey", 1);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapSetIntegerValue(hTLSystemNodeMap, "ActionCommandGroupKey", 1);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapSetIntegerValue(hTLSystemNodeMap, "ActionCommandGroupMask", 1);
	if (err != AC_ERR_SUCCESS)
		return err;

	err = acNodeMapSetIntegerValue(hTLSystemNodeMap, "ActionCommandTargetIP", 0xFFFFFFFF);
	if (err != AC_ERR_SUCCESS)
		return err;

	printf(" prepared \n");

	// Wait for devices to negotiate their PTP relationship
	//    Before starting any PTP-dependent actions, it is important to wait for
	//    the devices to complete their negotiation; otherwise, the devices may
	//    not yet be synced. Depending on the initial PTP state of each camera,
	//    it can take about 40 seconds for all devices to autonegotiate.
	//    Below, we wait for the PTP status of each device until
	//    there is only one 'Master' and the rest are all 'Slaves'.
	//    During the negotiation phase, multiple devices may initially come up as 
	//    Master so we will wait until the ptp negotiation completes.
	printf("%sWait for devices to negotiate. This can take up to about 40s.\n", TAB1);

	size_t j;
	int i = 0;
	while (true)
	{
		bool8_t masterFound = false;
		bool8_t restartSyncCheck = false;

		// check devices
		for (j = 0; j < numDevices; j++)
		{
			// get PTP status
			char ptpStatusBuf[MAX_BUF];
			size_t ptpStatusBufLen = MAX_BUF;

			err = acNodeMapGetEnumerationValue(hNodeMaps[j], "PtpStatus", ptpStatusBuf, &ptpStatusBufLen);
			if (err != AC_ERR_SUCCESS)
				return err;

			if (strcmp(ptpStatusBuf, "Master") == 0)
			{
				if (masterFound)
				{
					// Multiple masters -- ptp negotiation is not complete
					restartSyncCheck = true;
					break;
				}

				masterFound = true;
			}
			else if (strcmp(ptpStatusBuf, "Slave") != 0)
			{
				// Uncalibrated state -- ptp negotiation is not complete
				restartSyncCheck = true;
				break;
			}
		}
		
		// A single master was found and all remaining cameras are slaves
		if (!restartSyncCheck && masterFound)
			break;

		portable_sleep(1);

		// for output
		if (i % 10 == 0)
		{
			printf("\r%s\r%s", ERASE_LINE, TAB2);
			fflush(stdout);
		}

		printf(".");
		fflush(stdout);

		i++;
	}

	// start stream
	printf("%sStart stream\n", TAB1);

	for (size_t i = 0; i < numDevices; i++)
	{
		err = acDeviceStartStream(phDevices[i]);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// Set up timing and broadcast action command
	//    Action commands must be scheduled for a time in the future. This can be
	//    done by grabbing the PTP time from a device, adding a delta to it, and
	//    setting it as an action command's execution time.
	printf("%sSet action command to %i nanoseconds from now\n", TAB1, DELTA_TIME);

	// execute latch
	err = acNodeMapExecute(hNodeMaps[0], "PtpDataSetLatch");
	if (err != AC_ERR_SUCCESS)
		return err;

	// get latch
	int64_t ptpDataSetLatchValue = 0;

	err = acNodeMapGetIntegerValue(hNodeMaps[0], "PtpDataSetLatchValue", &ptpDataSetLatchValue);
	if (err != AC_ERR_SUCCESS)
		return err;

	// set execute time to future time
	int64_t deltaTime = DELTA_TIME;

	err = acNodeMapSetIntegerValue(hTLSystemNodeMap, "ActionCommandExecuteTime", ptpDataSetLatchValue + deltaTime);
	if (err != AC_ERR_SUCCESS)
		return err;

	// Fire action command
	//    Action commands are fired and broadcast to all devices, but only
	//    received by the devices matching desired settings.
	printf("%sFire action command\n", TAB1);

	err = acNodeMapExecute(hTLSystemNodeMap, "ActionCommandFireCommand");
	if (err != AC_ERR_SUCCESS)
		return err;

	// get images and check timestamps
	printf("%sGet images\n", TAB1);

	for (size_t i = 0; i < numDevices; i++)
	{
		// get serial
		char pSerialNumberBuf[MAX_BUF];
		size_t serialnumberBufLen = MAX_BUF;

		err = acSystemGetDeviceSerial(hSystem, i, pSerialNumberBuf, &serialnumberBufLen);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%sImage from device %s \n", TAB2, pSerialNumberBuf);

		// Compare timestamps
		//    Scheduling action commands amongst PTP synchronized devices results
		//    synchronized images with synchronized timestamps.
		acBuffer hBuffer = NULL;
		uint64_t timestamp = 0;
		printf("%sTimestamp: ", TAB3);

		// Initiate image transfer from current camera
		err = acNodeMapExecute(hNodeMaps[i], "TransferStart");
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acDeviceGetBuffer(phDevices[i], 3000, &hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acNodeMapExecute(hNodeMaps[i], "TransferStop");
		if (err != AC_ERR_SUCCESS)
			return err;

		err = acImageGetTimestamp(hBuffer, &timestamp);
		if (err != AC_ERR_SUCCESS)
			return err;

		printf("%" PRIu64 "\n", timestamp);

		// requeue image buffer
		err = acDeviceRequeueBuffer(phDevices[i], hBuffer);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// stop stream
	printf("%sStop stream\n", TAB1);

	for (size_t i = 0; i < numDevices; i++)
	{
		err = acDeviceStopStream(phDevices[i]);
		if (err != AC_ERR_SUCCESS)
			return err;
	}

	// return nodes to their initial values
	for (size_t i = 0; i < numDevices; i++)
	{
		// packet size affects the exposure range so we restore it first
		err = acNodeMapSetIntegerValue(hNodeMaps[i], "DeviceStreamChannelPacketSize", packetSizeInitials[i]);
		if (err != AC_ERR_SUCCESS)
			printf("\nWarning: failed to set one or more node values back to its initial value.\n");

		// exposure
		if (0 == strcmp(pExposureAutoInitials[i], "Off"))
		{
			err = acNodeMapSetFloatValue(hNodeMaps[i], "ExposureTime", exposureTimeInitials[i]);
			if (err != AC_ERR_SUCCESS)
				return err;
		}
		err = acNodeMapSetEnumerationValue(hNodeMaps[i], "ExposureAuto", pExposureAutoInitials[i]) |
			// trigger
			acNodeMapSetEnumerationValue(hNodeMaps[i], "TriggerSelector", triggerSelectorInitials[i]) |
			acNodeMapSetEnumerationValue(hNodeMaps[i], "TriggerSource", triggerSourceInitials[i]) |
			acNodeMapSetEnumerationValue(hNodeMaps[i], "TriggerMode", triggerModeInitials[i]) |
			// action commands
			acNodeMapSetIntegerValue(hNodeMaps[i], "ActionGroupMask", actionGroupMaskInitials[i]) |
			acNodeMapSetIntegerValue(hNodeMaps[i], "ActionGroupKey", actionGroupKeyInitials[i]) |
			acNodeMapSetIntegerValue(hNodeMaps[i], "ActionSelector", actionSelectorInitials[i]) |
			acNodeMapSetEnumerationValue(hNodeMaps[i], "ActionUnconditionalMode", actionUnconditionalModeInitials[i]) |
			// ptp
			acNodeMapSetBooleanValue(hNodeMaps[i], "PtpEnable", ptpEnableInitials[i]) |
			// transfer control
			acNodeMapSetEnumerationValue(hNodeMaps[i], "TransferControlMode", transferControlModeInitials[i]);

		if (err != AC_ERR_SUCCESS)
			printf("\nWarning: failed to set one or more node values back to its initial value.\n");
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
	printf("C_ScheduledActionCommands\n");
	AC_ERROR err = AC_ERR_SUCCESS;

	printf("\nNote: The PTP auto-negotiation phase can take about 40s depending on the " 
		"initial PTP state of each device\n\n");

	printf("Example may overwrite 'ActionDeviceKey' -- proceed? ('y' to continue) ");
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
			printf("\nNo camera connected, example requires at least 2 cameras\n");
			printf("Press enter to complete\n");
			getchar();
			return -1;
		}

		else if (numDevices == 1)
		{
			printf("\nOnly one camera connected, example requires at least 2 cameras\n");
			printf("Press enter to complete\n");
			getchar();
			return -1;
		}

		if (numDevices > MAX_DEVICES)
		{
			printf("\nWarning: too many cameras, example set to run with only %d cameras\n", MAX_DEVICES);
		}

		acDevice hDevices[MAX_DEVICES];

		for (size_t index = 0; index < numDevices; index++)
		{

			err = acSystemCreateDevice(hSystem, index, &hDevices[index]);
			CHECK_RETURN;
		}

		// run example
		printf("Commence example\n\n");

		err = SynchronizeCamerasAndTriggerImage(hSystem, hDevices, numDevices);
		CHECK_RETURN;

		printf("\nExample complete\n");

		// clean up example
		for (size_t i = 0; i < numDevices; i++)
		{
			err = acSystemDestroyDevice(hSystem, hDevices[i]);
			CHECK_RETURN;
		}
		err = acCloseSystem(hSystem);
		CHECK_RETURN;
	}

	printf("Press enter to complete\n");
	getchar();
	return -1;
}
