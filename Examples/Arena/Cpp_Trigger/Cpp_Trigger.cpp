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
#include "GenTL.h"

#ifdef __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#include "GenICam.h"

#ifdef __linux__
#pragma GCC diagnostic pop
#endif

#include "ArenaApi.h"

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Trigger: Introduction
//    This example introduces basic trigger configuration and use. In order to
//    configure trigger, enable trigger mode and set the source and selector. The
//    trigger must be armed before it is prepared to execute. Once the trigger is
//    armed, execute the trigger and retrieve an image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// image timeout
#define TIMEOUT 2000

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates basic trigger configuration and use
// (1) sets trigger mode, source, and selector
// (2) starts stream
// (3) waits until trigger is armed
// (4) triggers image
// (5) gets image
// (6) requeues buffer
// (7) stops stream
void ConfigureTriggerAndAcquireImage(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	GenICam::gcstring triggerSelectorInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSelector");
	GenICam::gcstring triggerModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode");
	GenICam::gcstring triggerSourceInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSource");

	// Set trigger selector
	//    Set the trigger selector to FrameStart. When triggered, the device will
	//    start acquiring a single frame. This can also be set to
	//    AcquisitionStart or FrameBurstStart.
	std::cout << TAB1 << "Set trigger selector to FrameStart\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"TriggerSelector",
		"FrameStart");

	// Set trigger mode
	//    Enable trigger mode before setting the source and selector and before
	//    starting the stream. Trigger mode cannot be turned on and off while the
	//    device is streaming.
	std::cout << TAB1 << "Enable trigger mode\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"TriggerMode",
		"On");

	// Set trigger source
	//    Set the trigger source to software in order to trigger images without
	//    the use of any additional hardware. Lines of the GPIO can also be used
	//    to trigger.
	std::cout << TAB1 << "Set trigger source to Software\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"TriggerSource",
		"Software");

	// Start stream
	//    When trigger mode is off and the acquisition mode is set to stream
	//    continuously, starting the stream will have the camera begin acquiring
	//    a steady stream of images. However, with trigger mode enabled, the
	//    device will wait for the trigger before acquiring any.
	std::cout << TAB1 << "Start stream\n";

	pDevice->StartStream();

	// Trigger Armed
	//    Continually check until trigger is armed. Once the trigger is armed, it
	//    is ready to be executed.
	std::cout << TAB2 << "Wait until trigger is armed\n";
	bool triggerArmed = false;

	do
	{
		triggerArmed = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "TriggerArmed");
	} while (triggerArmed == false);

	// Trigger an image
	//    Trigger an image manually, since trigger mode is enabled. This triggers
	//    the camera to acquire a single image. A buffer is then filled and moved
	//    to the output queue, where it will wait to be retrieved.
	std::cout << TAB2 << "Trigger image\n";

	Arena::ExecuteNode(
		pDevice->GetNodeMap(),
		"TriggerSoftware");

	// Get image
	//    Once an image has been triggered, it can be retrieved. If no image has
	//    been triggered, trying to retrieve an image will hang for the duration
	//    of the timeout and then throw an exception.
	std::cout << TAB2 << "Get image";

	Arena::IImage* pImage = pDevice->GetImage(TIMEOUT);

	std::cout << " (" << pImage->GetWidth() << "x" << pImage->GetHeight() << ")\n";

	// requeue buffer
	std::cout << TAB2 << "Requeue buffer\n";

	pDevice->RequeueBuffer(pImage);

	// Stop the stream
	std::cout << TAB1 << "Stop stream\n";

	pDevice->StopStream();

	// return nodes to their initial values
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSource", triggerSourceInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode", triggerModeInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSelector", triggerSelectorInitial);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Trigger\n";

	try
	{
		// prepare example
		Arena::ISystem* pSystem = Arena::OpenSystem();
		pSystem->UpdateDevices(1000);
		std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();
		if (deviceInfos.size() == 0)
		{
			std::cout << "\nNo camera connected\nPress enter to complete\n";
			std::getchar();
			return 0;
		}
		Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfos[0]);

		// run example
		std::cout << "Commence example\n\n";
		ConfigureTriggerAndAcquireImage(pDevice);
		std::cout << "\nExample complete\n";

		// clean up example
		pSystem->DestroyDevice(pDevice);
		Arena::CloseSystem(pSystem);
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\nGenICam exception thrown: " << ge.what() << "\n";
		exceptionThrown = true;
	}
	catch (std::exception& ex)
	{
		std::cout << "Standard exception thrown: " << ex.what() << "\n";
		exceptionThrown = true;
	}
	catch (...)
	{
		std::cout << "Unexpected exception thrown\n";
		exceptionThrown = true;
	}

	std::cout << "Press enter to complete\n";
	std::getchar();

	if (exceptionThrown)
		return -1;
	else
		return 0;
}
