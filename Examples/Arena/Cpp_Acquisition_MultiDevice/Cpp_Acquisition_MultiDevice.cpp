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
#include <thread>
#include <mutex>
#include <iomanip> // std:setw

#define TAB1 "  "
#define TAB2 "    "

std::mutex g_print_mtx; // for making cout thread safe

// Acquisition: Multi-Device
//    This example introduces the basics of image acquisition for multiple devices
//    and creating multiple threads. This includes creating all discovered
//    devices, creating a thread for each device to acquire images. The thread
//    then starts acquiring images by grabbing and requeuing buffers, before
//    finally stopping the image stream.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Image timeout
//    Timeout for grabbing images (in milliseconds). If no image is available at
//    the end of the timeout, an exception is thrown. The timeout is the maximum
//    time to wait for an image; however, getting an image will return as soon as
//    an image is available, not waiting the full extent of the timeout.
#define TIMEOUT 2000

// number of images to grab
#define NUM_IMAGES 250

// readable identifier for the thread writing to std::cout
#define THREAD_INFO std::setw(20) << threadName << " | "

// thread safe print statement
#define PRINT_LOCK(statement)                              \
	{                                                      \
		{                                                  \
			std::lock_guard<std::mutex> lock(g_print_mtx); \
			statement;                                     \
		}                                                  \
	}

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates acquisition
// (1) starts the stream
// (2) gets a number of images
// (3) requeues buffers
// (4) stops the stream
void AcquireImages(Arena::IDevice* const pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	GenICam::gcstring acquisitionModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode");

	// Set acquisition mode
	//    Set acquisition mode before starting the stream. Starting the stream
	//    requires the acquisition mode to be set beforehand. The acquisition
	//    mode controls the number of images a device acquires once the stream
	//    has been started. Setting the acquisition mode to 'Continuous' keeps
	//    the stream from stopping.
	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"AcquisitionMode",
		"Continuous");

	// Enable stream auto negotiate packet size
	//    Setting the stream packet size is done before starting the stream.
	//    Setting the stream to automatically negotiate packet size instructs the
	//    camera to receive the largest packet size that the system will allow.
	//    This generally increases frame rate and results in fewer interrupts per
	//    image, thereby reducing CPU load on the host system. Ethernet settings
	//    may also be manually changed to allow for a larger packet size.
	Arena::SetNodeValue<bool>(
		pDevice->GetTLStreamNodeMap(),
		"StreamAutoNegotiatePacketSize",
		true);

	// Enable stream packet resend
	//    Enable stream packet resend before starting the stream. Images are sent
	//    from the camera to the host in packets using UDP protocol, which
	//    includes a header image number, packet number, and timestamp
	//    information. If a packet is missed while receiving an image, a packet
	//    resend is requested and this information is used to retrieve and
	//    redeliver the missing packet in the correct order.
	Arena::SetNodeValue<bool>(
		pDevice->GetTLStreamNodeMap(),
		"StreamPacketResendEnable",
		true);

	// name the thread using the device model and serial number for more readable output
	std::string deviceSerialNumber = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "DeviceSerialNumber").c_str();
	std::string deviceModel = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "DeviceModelName").c_str();
	std::string threadName = deviceModel + deviceSerialNumber;

	// Start stream
	//    Start the stream before grabbing any images. Starting the stream
	//    allocates buffers, which can be passed in as an argument (default: 10),
	//    and begins filling them with data. Starting the stream blocks write
	//    access to many features such as width, height, and pixel format, as
	//    well as acquisition and buffer handling modes, among others. The stream
	//    needs to be stopped later.
	PRINT_LOCK(std::cout << THREAD_INFO << TAB1 << "Start stream\n";)

	pDevice->StartStream();

	// get images
	PRINT_LOCK(std::cout << THREAD_INFO << TAB1 << "Getting " << NUM_IMAGES << " images\n";)

	for (int i = 0; i < NUM_IMAGES; i++)
	{
		try
		{
			// Get image
			//    Retrieve images after the stream has started. If the timeout
			//    expires before an image is retrieved, the example will throw.
			//    Because of this, the timeout should be at least a bit larger than
			//    the exposure time.
			PRINT_LOCK(std::cout << THREAD_INFO << TAB2 << "Getting image " << i << '\n';)

			Arena::IImage* pImage = pDevice->GetImage(TIMEOUT);

			// Requeue image buffer
			//    Requeue an image buffer when access to it is no longer needed.
			//    Notice that failing to requeue buffers may cause memory to leak and
			//    may also result in the stream engine being starved due to there
			//    being no available buffers.
			pDevice->RequeueBuffer(pImage);

			PRINT_LOCK(std::cout << THREAD_INFO << TAB2 << "image " << i << " received and requeued\n";)
		}
		catch (GenICam::TimeoutException& ge)
		{
			PRINT_LOCK(std::cout << THREAD_INFO << TAB1 << "GenICam exception thrown: " << ge.what() << std::endl;)
			break;
		}
	}

	// Stop stream
	//    Stop the stream after all images have been requeued. Failing to stop
	//    the stream will leak memory.
	PRINT_LOCK(std::cout << THREAD_INFO << TAB1 << "Stop stream\n";)

	pDevice->StopStream();

	// return nodes to their initial values
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode", acquisitionModeInitial);
}

// demonstrates threads setup
// (1) creates threads vector
// (2) creates threads and start them
// (3) adds threads handler to vector
// (4) waits for all threads to join
void AcquireImagesConcurrently(std::vector<Arena::IDevice*> vDevices)
{
	// keep all threads in a vector to operate on them later
	std::vector<std::thread> vThreads = std::vector<std::thread>();

	// create a thread for each device
	for (const auto& pDevice : vDevices)
	{
		// create and start theard
		std::thread thread = std::thread(AcquireImages, pDevice);

		// threads can not be copied, only moved. Therefore, std::move is used
		vThreads.push_back(std::move(thread));
	}

	// wait for all threads to finish
	for (auto& thread : vThreads)
	{
		// join thread if it is joinable
		if (thread.joinable())
		{
			thread.join();
		}
	}
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Acquisition_MultiDevice\n";

	try
	{
		// prepare example
		Arena::ISystem* pSystem = Arena::OpenSystem();
		pSystem->UpdateDevices(100);
		std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();
		if (deviceInfos.size() == 0)
		{
			std::cout << "\nNo camera connected\nPress enter to complete\n";
			std::getchar();
			return 0;
		}
		if (deviceInfos.size() < 2)
		{
			std::cout << "\nThis example is recommended to run with more than one device to demonstrate the multithreading clearly\n";
		}

		// create all dicovered devices and add them to a vector
		std::vector<Arena::IDevice*> vDevices = std::vector<Arena::IDevice*>();
		for (auto& deviceInfo : deviceInfos)
		{
			Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfo);
			vDevices.push_back(pDevice);
		}

		// run example
		std::cout << "Commence example\n\n";
		AcquireImagesConcurrently(vDevices);
		std::cout << "\nExample complete\n";

		// clean up example
		for (auto& pDevice : vDevices)
		{
			pSystem->DestroyDevice(pDevice);
		}

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
