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

#include <chrono>  // for std::chrono::milliseconds
#include <thread>  // for std::this_thread::sleep_for
#include <iomanip> // for std::setw

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Callbacks: Polling
//    This example demonstrates configuring a callback with polling. Polling
//    allows for callbacks to be invoked over time.


// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Number of times to poll
#define NUM_POLLS 50

// Time to pass between polling (in milliseconds)
#define ELAPSED_TIME 500

// callback counter
int g_count = 0;

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// callback function
// (1) prints information from the callback
void PrintNodeValue(GenApi::INode* pNode)
{
	// Do something on callback
	//    This function is registered with the callback and invoked when the node
	//    map has been polled if the elapsed time is over the threshold of the
	//    node. In this case, the current device temperature is printed.
	GenApi::CFloatPtr pDeviceTemperature = pNode;

	std::cout << "\r" << TAB3 << std::setw(4) << g_count++ << " Current device temperature: " << pDeviceTemperature->GetValue() << char(167) << pDeviceTemperature->GetUnit() << std::flush;
}

// demonstrates polling
// (1) gets the node map and node to poll
// (2) registers callback
// (3) starts stream
// (4) polls node map at regular intervals, invoking callback
// (5) stops stream
// (6) deregisters callback
void ConfigureCallbackToPollDeviceTemperature(Arena::IDevice* pDevice)
{
	// Get node map and device temperature node
	//    Nodes are polled through their node maps. This example demonstrates
	//    polling the device temperature node. It has a polling time of 1 second,
	//    which means that its callback will not be invoked within 1 second of
	//    the last time it has been polled.
	std::cout << TAB1 << "Get node map and device temperature node\n";

	GenApi::INodeMap* pNodeMap = pDevice->GetNodeMap();
	GenApi::CNodePtr pNode = pNodeMap->GetNode("DeviceTemperature");

	if (!pNode || !GenApi::IsReadable(pNode))
	{
		throw GenICam::GenericException("Node not found/readable", __FILE__, __LINE__);
	}

	// Register callback
	//    Callbacks are registered with a node and a function. This example
	//    demonstrates callbacks being invoked through polling. This happens when
	//    a node map is polled and the polling time is ready to poll again.
	std::cout << TAB1 << "Register callback\n";

	GenApi::CallbackHandleType hCallback = GenApi::Register(pNode, PrintNodeValue);

	// start stream
	std::cout << TAB1 << "Start stream\n";

	pDevice->StartStream();

	// Poll at regular intervals
	//    The polling time of device temperature is 1 second. The callback will
	//    only be invoked if the elapsed time since the last callback is larger
	//    than the polling time. In this example, two counts are shown: a polling
	//    count and a callback count. This is to demonstrate that the callback is
	//    not necessarily invoked every time the node map is polled.
	std::cout << TAB1 << "Start polling\n";

	for (int i = 0; i < NUM_POLLS; i++)
	{
		std::cout << "\r" << TAB1 << std::setw(4) << i << std::flush;

		// sleep for elapsed time
		std::this_thread::sleep_for(std::chrono::milliseconds(ELAPSED_TIME));

		// poll node on elapsed time
		pNodeMap->Poll(ELAPSED_TIME);
	}
	std::cout << TAB2 << NUM_POLLS << "\n";

	// stop stream
	std::cout << TAB1 << "Stop stream\n";

	pDevice->StopStream();

	// Deregister callback
	//    Failing to deregister a callback results in a memory leak. Once a
	//    callback has been registered, it will no longer be invoked when a node
	//    is invalidated.
	std::cout << TAB1 << "Deregister callback\n";

	GenApi::Deregister(hCallback);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Callback_Polling\n";

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
		Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfos[0]);

		// run example
		std::cout << "Commence example\n\n";
		ConfigureCallbackToPollDeviceTemperature(pDevice);
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
