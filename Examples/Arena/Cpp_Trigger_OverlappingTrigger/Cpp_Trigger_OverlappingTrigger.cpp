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
#include <iomanip> // for std::setw
#include <chrono>  // for std::chrono
#include <thread>  // for std::this_thread

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

// Trigger: Overlapping Trigger on Exposure End Events
//    This example demonstrates the use of trigger and exposure end events to
//    minimize the amount of unused exposure time between images. This is done by
//    setting the device to start exposing (or trigger) right when the last
//    exposure has just finished (or exposure end event). After receiving each
//    exposure end event notification, the next trigger is executed to acquire
//    the next image. Once all triggers have executed, the images are retrieved
//    from the output queue. This example shows that there is little time between
//    triggering images, and that the exposure time is close to the time between
//    triggers.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Number of images to grab
#define NUM_IMAGES 10

// Event timeout
#define EVENT_TIMEOUT 3000

// image timeout
#define IMAGE_TIMEOUT 2000

// system timeout
#define SYSTEM_TIMEOUT 100

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates basic trigger configuration and use
// (1) sets trigger mode, source, and selector
// (2) sets acquisition mode, event selector, and turns on event notifications
// (3) starts stream
// (4) waits on trigger armed, executes trigger, and waits on exposureEnd event for all images
// (5) grabs each image and requeues image buffer for all images
// (6) stops stream
void OverlapTriggerOnExposureEndEvent(Arena::IDevice* pDevice)
{
	// Initialize events
	//    Events nodes aren't available until the events engine has been
	//    initialized.
	pDevice->InitializeEvents();

	// get node values that will be changed in order to return their values at
	// the end of the example
	GenICam::gcstring triggerSelectorInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSelector");
	GenICam::gcstring triggerModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode");
	GenICam::gcstring triggerSourceInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerSource");
	GenICam::gcstring triggerOverlapInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerOverlap");
	GenICam::gcstring acquisitionModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode");
	GenICam::gcstring eventSelectorInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "EventSelector");
	GenICam::gcstring eventNotificationInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "EventNotification");
	GenICam::gcstring exposureAutoInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureAuto");

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

	GenApi::CCommandPtr pTriggerSoftwareNode = pDevice->GetNodeMap()->GetNode("TriggerSoftware");

	// Set trigger overlap
	//    Trigger overlap defines when a trigger can start accepting a new frame.
	//    Setting trigger to overlap with the previous frame allows the camera to
	//    being exposing the new frame while the camera is still reading out the
	//    sensor data acquired from the previous frame.
	std::cout << TAB1 << "Set trigger overlap to PreviousFrame\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"TriggerOverlap",
		"PreviousFrame");

	// Set acquisition mode
	//    When trigger mode is off and the acquisition mode is set to stream
	//    continuously, starting the stream will have the camera begin acquiring
	//    a steady stream of images. However, with trigger mode enabled, the
	//    device will wait for the trigger before acquiring any.
	std::cout << TAB1 << "Set acquisition mode to: \"Continuous\"\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"AcquisitionMode",
		"Continuous");

	// Set event selector
	//    We want to trigger and wait to be notified as soon as a certain event
	//    occurs while making the image. Here we choose to be notified at the end
	//    of the exposure of an image.
	std::cout << TAB1 << "Set event selector to: \"ExposureEnd\"\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"EventSelector",
		"ExposureEnd");

	// Set event notification
	//    After choosing which event to be notified about, enabling the
	//    EventNotification node will turn on the notification for the event
	//    selected in the EventSelector node.
	std::cout << TAB1 << "Enable event notifications for \"ExposureEnd\"\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"EventNotification",
		"On");

	// Retrieve exposure time
	//    The exposure time is similar to the time between triggering images.
	//    This is shown by turning off automatic exposure, retrieving the
	//    exposure time and converting to nanoseconds.
	std::cout << TAB1 << "Exposure time: ";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"ExposureAuto",
		"Off");

	double exposureTime = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "ExposureTime");

	// convert to nanoseconds
	double exposureTimeNs = exposureTime * 1000;
	std::cout << exposureTimeNs << " ns\n ";

	// start stream
	const size_t numberOfImages = NUM_IMAGES;
	const int64_t waitOnEventTimeout = EVENT_TIMEOUT;
	std::cout << TAB1 << "Start stream with " << numberOfImages << " buffers\n";

	pDevice->StartStream(numberOfImages);

	// Trigger an image
	//    Trigger an image manually, since trigger mode is enabled. This triggers
	//    the camera to acquire a single image. A buffer is then filled and moved
	//    to the output queue, where it will wait to be retrieved.
	for (size_t i = 0; i < numberOfImages; i++)
	{
		std::cout << TAB2 << "\rImage " << std::setw(4) << i << std::flush << " triggered";

		// Trigger Armed
		//    Continually check until trigger is armed. Once the trigger is
		//    armed, it is ready to be executed.
		bool isTriggerArmed = false;

		do
		{
			isTriggerArmed = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "TriggerArmed");
		} while (isTriggerArmed == false);

		pTriggerSoftwareNode->Execute();

		// Wait on event
		//    Wait on event to process before continuing. The data is created
		//    from the event generation, not from waiting on it.
		pDevice->WaitOnEvent(waitOnEventTimeout);
		std::cout << " and ExposureEnd Event notification arrived";
	}

	std::cout << "\n"
			  << TAB1 << "Done grabbing " << std::setw(4) << numberOfImages << " images\n";

	// retrieve images and timestamps
	uint64_t numberOfImages_64_t = (uint64_t)numberOfImages;
	const int64_t imageTimeout = IMAGE_TIMEOUT;
	uint64_t timestampNs[NUM_IMAGES] = { 0 };

	for (uint64_t i = 0; i < numberOfImages_64_t; i++)
	{
		Arena::IImage* pImage = pDevice->GetImage(imageTimeout);
		timestampNs[i] = pImage->GetTimestampNs();

		std::cout << TAB2 << "Image " << std::setw(4) << i << " timestamp: " << pImage->GetTimestampNs() << " ns";

		if (i > 0)
		{
			// calculate time between triggers
			uint64_t triggerTimestampDifference = timestampNs[i] - timestampNs[i - 1];

			std::cout << " (" << triggerTimestampDifference << " ns since last trigger)\n";
		}
		else
			std::cout << "\n";

		pDevice->RequeueBuffer(pImage);
	}

	// stop stream
	std::cout << TAB1 << "Stop stream\n"
			  << std::endl;

	pDevice->StopStream();

	// Deinitialize events
	//    Deinitialize the events engine in order to deallocate memory and stop
	//    processing events. Failing to deinitialize events results in a memory
	//    leak.
	pDevice->DeinitializeEvents();

	// return nodes to their initial values
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureAuto", exposureAutoInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "EventNotification", eventNotificationInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "EventSelector", eventSelectorInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode", acquisitionModeInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerOverlap", triggerOverlapInitial);
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

	std::cout << "Cpp_Trigger_OverlappingTrigger\n";

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
		OverlapTriggerOnExposureEndEvent(pDevice);
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
