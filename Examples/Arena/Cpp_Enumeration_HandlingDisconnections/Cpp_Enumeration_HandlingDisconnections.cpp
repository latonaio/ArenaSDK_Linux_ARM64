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

#include <iomanip>			  // for std::setw
#include <thread>			  // for std::thread
#include <mutex>			  // for std::mutex, std::unique_lock
#include <condition_variable> // for std::condition_variable
#include <algorithm>		  // for std::find_if
#include <string>			  // for std::to_string

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "
#define TAB4 "        "

// Enumeration: Handling Disconnections
//    This example demonstrates a multi-threaded approach to handling device
//    disconnections. It spawns two threads, each with a different
//    responsibility. First, the acquisition thread is responsible for acquiring
//    images when the device is connected. Second, the enumeration thread handles
//    disconnections by reconnecting the device and notifying the acquisition
//    thread.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// image timeout
#define IMAGE_TIMEOUT 2000

// update timeout
#define UPDATE_TIMEOUT 100

// maximum number of images
#define MAX_IMAGES 500

// Global settings
//    This example uses global scope for variables and information shared between
//    threads. This is not a best practice but is done this way for the sake of
//    simplicity. This structure could just as easily be placed in a class.

// arena
Arena::IDevice* g_pDevice = nullptr;
GenICam::gcstring g_serialNumber = "";
uint32_t g_subnetMask = 0;

// threading
std::mutex g_deviceConnectedMutex;
std::mutex g_deviceDisconnectedMutex;
std::condition_variable g_deviceConnected;
std::condition_variable g_deviceDisconnected;

// other
bool g_isRunning = false;

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// reconnects a device when disconnected
// (1) On disconnection, activate
// (2) Search for device
// (3) Reconnect device
// (4) Notify all on reconnection
// (5) Notify all on exit
void EnumerationThread(Arena::ISystem* g_pSystem)
{
	while (g_isRunning)
	{
		// wait until device is disconnected and thread is notified to start
		std::unique_lock<std::mutex> deviceDisconnectedLock(g_deviceDisconnectedMutex);
		g_deviceDisconnected.wait(deviceDisconnectedLock);

		while (g_isRunning && !g_pDevice)
		{

			// Search for device
			//    When the device has been disconnected, this thread waits for it
			//    to reconnect, constantly updating and scanning the device list
			//    for the lost device.
			g_pSystem->UpdateDevices(UPDATE_TIMEOUT);

			std::vector<Arena::DeviceInfo> deviceInfos = g_pSystem->GetDevices();
			if (deviceInfos.size() > 0)
			{

				GenICam::gcstring serialNumber = g_serialNumber;
				auto it = std::find_if(
					deviceInfos.begin(),
					deviceInfos.end(),
					[&serialNumber](Arena::DeviceInfo deviceInfo) {
						return deviceInfo.SerialNumber() == g_serialNumber;
					});

				if (it != deviceInfos.end())
				{
					// Recreate device and notify other thread
					//    Once the device has been found, recreate it and notify the
					//    acquisition thread that it can stop waiting and continue
					//    acquiring images.
					std::cout << "\r" << TAB4 << "Device reconnected\n"
							  << std::flush;

					g_pDevice = g_pSystem->CreateDevice(*it);

					uint32_t subnetMaskReconnect = (it)->SubnetMask();

					// Ensure appropriate network settings
					//    Check that the device is on the same subnet after
					//    reconnecting the camera. If the camera/adapter are on a non
					//    169.254.*.* subnet but not using a persistent IP or DHCP,
					//    the camera will automatically be assigned an LLA and pick a
					//    169.254.*.* IP/subnet after reconnecting, causing the
					//    example to exit. There are several ways to fix this issue:
					//    (1) by setting a static IP to the ethernet port, (2)
					//    forcing an IP address whenever the device is updated (see
					//    Cpp_ForceIP), (3) running ArenaConfig to configure the
					//    adapter settings, or (4) setting up a persistent IP on the
					//    camera using IPConfigUtility
					if (subnetMaskReconnect != g_subnetMask)
					{
						std::cout << TAB2 << "Error: Subnet has changed upon reconnecting\n";
						std::cout << TAB3 << "Subnet at example start:   " << g_subnetMask << "\n";
						std::cout << TAB3 << "Subnet after reconnection: " << subnetMaskReconnect << "\n";
						std::cout << TAB1 << "\nPress enter to exit example\n";
						g_isRunning = false;
						g_deviceConnected.notify_all();
					}

					g_deviceConnected.notify_all();
				}
			}
		}
	};

	// Notify other thread on exit
	//    If the device is disconnected at the time of exit, the other thread
	//    will be waiting for reconnection. Sending this notification allows the
	//    other thread to stop waiting and continue to exit.
	std::cout << TAB3 << "\nNotify other thread on exit\n";

	g_deviceConnected.notify_all();
}

// acquires images while device is connected
// (1) starts stream
// (2) retrieves images
// (3) catches disconnections, destroying device appropriately
// (4) waits for reconnection
// (5) restarts stream, continuing to retrieve images
// (6) stops stream
void AcquisitionThread(Arena::ISystem* g_pSystem)
{
	// start stream
	int numImages = 1;
	g_pDevice->StartStream();

	while (g_isRunning)
	{
		// Lock device across threads
		//    Use a mutex to lock the device across threads, ensuring that
		//    simultaneous writes/reads across threads don't clobber one another.
		std::unique_lock<std::mutex> deviceConnectedLock(g_deviceConnectedMutex);

		try
		{
			if (!g_pDevice)
			{
				// Wait for reconnection if necessary
				//    When the device has been disconnected, the enumeration thread
				//    is notified to start running, while the acquisition
				//    thread waits on the enumeration thread to reconnect the
				//    device. This is done through a conditional variable. This
				//    conditional variable has also been set up to stop waiting
				//    if the application has been terminated.
				g_deviceDisconnected.notify_all();

				g_deviceConnected.wait(deviceConnectedLock);

				// Restart stream if connected
				//    Check that the device has been reconnected in case the the
				//    notification has been sent to quit the application. If the
				//    device has been reconnected, restart the stream so that it
				//    can continue acquiring images.
				if (g_pDevice)
				{
					g_pDevice->StartStream();
				}
			}

			// Get images while connected
			//    While the device is connected and streaming, grab images. This
			//    example just counts the images and requeues their buffers;
			//    however, using the image factory to copy or convert the images
			//    for display or processing might be a useful addition.
			while (g_isRunning && numImages <= MAX_IMAGES)
			{
				// get image
				std::cout << "\r" << TAB3 << "Get image " << numImages << std::flush;

				Arena::IImage* pImage = g_pDevice->GetImage(IMAGE_TIMEOUT);
				numImages++;
				g_pDevice->RequeueBuffer(pImage);
			};
		}
		catch (GenICam::TimeoutException&)
		{
			// Catch disconnection
			//    Disconnections will most likely show themselves as read/write
			//    timeouts. This is caused as the host attempts to signal the
			//    device, but the device doesn't respond, timing out.
			std::cout << "\n"
					  << TAB4 << "Device disconnected\n";

			g_pSystem->DestroyDevice(g_pDevice);
			g_pDevice = NULL;
		}

		if (numImages > MAX_IMAGES)
		{
			std::cout << "\n"
					  << TAB3 << "Acquisition completed, press enter to continue\n";
			g_isRunning = false;
		}
	};

	// stop stream
	if (g_pDevice)
	{
		g_pDevice->StopStream();
	}
	// notify other thread on exit
	g_deviceDisconnected.notify_all();
}

// manages Arena objects and threads
// (1) prepare global scope
// (2) start acquisition and enumeration threads
// (3) wait for key press
// (3) stop acquisition and clean up threads
void RunExample(Arena::ISystem* g_pSystem)
{
	// Start acquisition and enumeration threads and wait for key
	//    Spawn a thread for acquisition and a thread for enumeration. Keeping
	//    this kind of work off the main thread allows for multiple things
	//    happening simultaneously in the case of more complex applications.
	std::cout << TAB1 << "Start acquisition and enumeration threads\n";

	g_isRunning = true;

	std::thread acquisitionThread(AcquisitionThread, g_pSystem);
	std::thread enumerationThread(EnumerationThread, g_pSystem);

	// Stop acquisition on enter pressed
	//    Because the main thread pushed all the work into the worker threads, it
	//    is free to wait for user input to end the application.
	std::cout << TAB2 << "Press enter to stop acquisition\n";
	std::getchar();

	// Join threads
	//    When user input is received, the application must send a signal to the
	//    worker threads that this has happened. This is accomplished with a
	//    global boolean. The application should then wait for each thread to
	//    complete before continuing its shutdown.
	g_isRunning = false;

	acquisitionThread.join();
	enumerationThread.join();
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Enumeration_HandlingDisconnections\n";
	std::cout << "Please manually disconnect and reconnect device as device acquires images\n";

	try
	{
		// prepare example
		Arena::ISystem* g_pSystem = Arena::OpenSystem();
		g_pSystem->UpdateDevices(100);
		std::vector<Arena::DeviceInfo> deviceInfos = g_pSystem->GetDevices();

		if (deviceInfos.size() == 0)
		{
			std::cout << "Waiting for a device...\n";
			while (true)
			{
				g_pSystem->UpdateDevices(100);
				deviceInfos = g_pSystem->GetDevices();
				if (deviceInfos.size() != 0)
				{
					break;
				}
			}
		}

		g_serialNumber = deviceInfos[0].SerialNumber();
		g_subnetMask = deviceInfos[0].SubnetMask();
		g_pDevice = g_pSystem->CreateDevice(deviceInfos[0]);

		// run example
		std::cout << "Commence example\n\n";
		RunExample(g_pSystem);
		std::cout << "\nExample complete\n";

		// clean up device and system
		if (g_pDevice)
		{
			g_pSystem->DestroyDevice(g_pDevice);
		}
		Arena::CloseSystem(g_pSystem);
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
