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
#include "ArenaApi.h"

#define TAB1 "  "
#define TAB2 "    "

// Acquisition: Rapid Acquisition
//    This example demonstrates configuring device settings in order to reduce
//    bandwidth and increase framerate. This includes reducing the region of
//    interest, reducing bits per pixel, increasing packet size, reducing
//    exposure time, and setting a large number of buffers.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Width and height
#define WIDTH 100
#define HEIGHT 100

// Pixel format
#define PIXEL_FORMAT "Mono8"

// Number of images
#define NUM_IMAGES 500

// Maximum packet size
//    In order to grab images at the maximum packet size, the ethernet adapter
//    must be configured appropriately: 'Jumbo packet' must be set to its
//    maximum, 'UDP checksum offload' must be set to 'Rx & Tx Enabled', and
//    'Receive Buffers' must be set to its maximum.
#define MAX_PACKET_SIZE false

// system timeout
#define SYSTEM_TIMEOUT 100

// image timeout
#define IMAGE_TIMEOUT 2000

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// sets integer value safely
// (1) ensures increment
// (2) ensures over minimum
// (3) ensures below maximum
// (4) sets value
int64_t SetIntValue(GenApi::INodeMap* pNodeMap, const char* nodeName, int64_t value)
{
	// get node
	GenApi::CIntegerPtr pInteger = pNodeMap->GetNode(nodeName);

	// Ensure increment
	//    If a node has an increment (all integer nodes & some float nodes), only
	//    multiples of the increment can be set. Ensure this by dividing and then
	//    multiplying by the increment. If a value is between two increments,
	//    this will push it to the lower value. Most minimum values are divisible
	//    by the increment. If not, this must also be considered in the
	//    calculation.
	value = (((value - pInteger->GetMin()) / pInteger->GetInc()) * pInteger->GetInc()) + pInteger->GetMin();

	// Check min/max values
	//    Values must not be less than the minimum or exceed the maximum value of
	//    a node. If a value does so, simply push it within range.
	if (value < pInteger->GetMin())
	{
		value = pInteger->GetMin();
	}

	if (value > pInteger->GetMax())
	{
		value = pInteger->GetMax();
	}

	// set value
	pInteger->SetValue(value);

	// return value for output
	return value;
}

// demonstrates configuration for high frame rates
// (1) lowers image size
// (2) maximizes packet size
// (3) minimizes exposure time
// (4) sets high number of buffers
// (5) waits until after acquisition to requeue buffers
void AcquireImagesRapidly(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	int64_t widthInitial = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width");
	int64_t heightInitial = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height");
	GenICam::gcstring pixelFormatInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat");
	int64_t deviceStreamChannelPacketSizeInitial;
	if (MAX_PACKET_SIZE)
	{
		deviceStreamChannelPacketSizeInitial = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "DeviceStreamChannelPacketSize");
	}
	GenICam::gcstring exposureAutoInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureAuto");
	double exposureTimeInitial = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "ExposureTime");

	// Set low width and height
	//    Reducing the size of an image reduces the amount of bandwidth required
	//    for each image. The less bandwidth required per image, the more images
	//    can be sent over the same bandwidth.
	std::cout << TAB1 << "Set low width and height";

	int64_t width = SetIntValue(
		pDevice->GetNodeMap(),
		"Width",
		WIDTH);

	int64_t height = SetIntValue(
		pDevice->GetNodeMap(),
		"Height",
		HEIGHT);

	std::cout << " (" << width << "x" << height << ")\n";

	// Set small pixel format
	//    Similar to reducing the ROI, reducing the number of bits per pixel also
	//    reduces the bandwidth required for each image. The smallest pixel
	//    formats are 8-bit bayer and 8-bit mono (i.e. BayerRG8 and Mono8).
	std::cout << TAB1 << "Set small pixel format (" << PIXEL_FORMAT << ")\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"PixelFormat",
		PIXEL_FORMAT);

	// Set maximum stream channel packet size
	//    Maximizing packet size increases frame rate by reducing the amount of
	//    overhead required between images. This includes both extra
	//    header/trailer data per packet as well as extra time from intra-packet
	//    spacing (the time between packets). In order to grab images at the
	//    maximum packet size, the ethernet adapter must be configured
	//    appropriately: 'Jumbo packet' must be set to its maximum, 'UDP checksum
	//    offload' must be set to 'Rx & Tx Enabled', and 'Received Buffers' must
	//    be set to its maximum.
	if (MAX_PACKET_SIZE)
	{
		std::cout << TAB1 << "Set maximum device stream channel packet size";

		GenApi::CIntegerPtr pDeviceStreamChannelPacketSize = pDevice->GetNodeMap()->GetNode("DeviceStreamChannelPacketSize");
		if (!pDeviceStreamChannelPacketSize || !GenApi::IsReadable(pDeviceStreamChannelPacketSize) || !GenApi::IsWritable(pDeviceStreamChannelPacketSize))
		{
			throw GenICam::GenericException("DeviceStreamChannelPacketSize node not found/readable/writable", __FILE__, __LINE__);
		}

		std::cout << " (" << pDeviceStreamChannelPacketSize->GetMax() << " " << pDeviceStreamChannelPacketSize->GetUnit() << ")\n";

		pDeviceStreamChannelPacketSize->SetValue(pDeviceStreamChannelPacketSize->GetMax());
	}

	// Set low exposure time
	//    Decreasing exposure time can increase frame rate by reducing the amount
	//    of time it takes to grab an image. Reducing the exposure time past
	//    certain thresholds can cause problems related to not having enough
	//    light. In certain situations this can be mitigated by increasing gain
	//    and/or environmental light.
	std::cout << TAB1 << "Set minimum exposure time";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"ExposureAuto",
		"Off");

	GenApi::CFloatPtr pExposureTime = pDevice->GetNodeMap()->GetNode("ExposureTime");

	if (!pExposureTime || !GenApi::IsReadable(pExposureTime) || !GenApi::IsWritable(pExposureTime))
	{
		throw GenICam::GenericException("ExposureTime node not found/readable/writable", __FILE__, __LINE__);
	}

	std::cout << " (" << pExposureTime->GetMin() << " " << pExposureTime->GetUnit() << ")\n";

	pExposureTime->SetValue(pExposureTime->GetMin());

	// Start stream with high number of buffers
	//    Increasing the number of buffers can increase speeds by reducing the
	//    amount of time taken to requeue buffers. In this example, one buffer is
	//    used for each image. Of course, the amount of buffers that can be used
	//    is limited by the amount of space in memory.
	std::cout << TAB1 << "Start stream with " << NUM_IMAGES << " buffers\n";
	std::vector<Arena::IImage*> images;

	pDevice->StartStream(NUM_IMAGES);

	for (int i = 1; i <= NUM_IMAGES; i++)
	{
		// Get image
		//    By starting the stream with enough buffers for all images without
		//    requeuing, performance is improved. While effective, this method is
		//    even more restricted by the amount of available memory.
		std::cout << ((i % 250 == 0 && i != 0) || i == NUM_IMAGES ? "\n" : "\r") << TAB2 << "Get image " << i << std::flush;

		Arena::IImage* pImage = pDevice->GetImage(IMAGE_TIMEOUT);
		images.push_back(pImage);
	}

	// requeue buffer for each image
	std::cout << TAB1 << "\nRequeue buffers\n";

	for (size_t i = 0; i < images.size(); i++)
	{
		pDevice->RequeueBuffer(images[i]);
	}

	// stop stream
	std::cout << TAB1 << "Stop stream\n";

	pDevice->StopStream();

	// return nodes to their initial values
	if (exposureAutoInitial == "Off")
	{
		Arena::SetNodeValue<double>(pDevice->GetNodeMap(), "ExposureTime", exposureTimeInitial);
	}
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureAuto", exposureAutoInitial);
	if (MAX_PACKET_SIZE)
	{
		Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "DeviceStreamChannelPacketSize", deviceStreamChannelPacketSizeInitial);
	}
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat", pixelFormatInitial);
	Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width", widthInitial);
	Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height", heightInitial);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Acquisition_RapidAcquisition\n";

	try
	{
		// prepare example
		Arena::ISystem* pSystem = Arena::OpenSystem();
		pSystem->UpdateDevices(SYSTEM_TIMEOUT);
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
		AcquireImagesRapidly(pDevice);
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
		std::cout << "\nStandard exception thrown: " << ex.what() << "\n";
		exceptionThrown = true;
	}
	catch (...)
	{
		std::cout << "\nUnexpected exception thrown\n";
		exceptionThrown = true;
	}

	std::cout << "Press enter to complete\n";
	std::getchar();

	if (exceptionThrown)
		return -1;
	else
		return 0;
}
