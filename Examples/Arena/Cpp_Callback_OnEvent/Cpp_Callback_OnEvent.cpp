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
#define TAB3 "      "
#define TAB4 "        "

// Callbacks: On Event
//    This example demonstrates configuring a callback with events. Events are a
//    subset of nodes that invoke callbacks through the underlying events engine.
//    The events engine is first initialized to listen for events, then the
//    callback is registered using the timestamp test event node. The event is
//    generated, along with any data generated from the event. The example then
//    waits for the event to process in order to invoke the callback. Registered
//    callbacks must also be deregistered before deinitializing the events engine
//    in order to avoid memory leaks.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Event timeout
#define EVENT_TIMEOUT 2000

// Number of events to generate
#define NUM_EVENTS 5

// timeout for detecting camera devices (in milliseconds).
#define SYSTEM_TIMEOUT 100

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// callback function
// (1) prints node information
void PrintNodeValue(GenApi::INode* pNode)
{
	GenApi::CIntegerPtr pEventTestTimestamp = pNode;

	// Do something on callback
	std::cout << TAB3 << "Message from callback\n";
	std::cout << TAB4 << "Test event timestamp: " << pEventTestTimestamp->GetValue() << "\n";
}

// demonstrates callbacks with events
// (1) gets event node
// (2) initializes events
// (3) registers callback
// (4) generates event
// (5) waits on event, invoking callback
// (6) deregisters callback
// (7) deinitializes events
void ConfigureCallbackOnEventTestTimestamp(Arena::IDevice* pDevice)
{
	// Get event node
	//    Events require callbacks and the events engine to operate. This example
	//    uses the timestamp test event node to show the setup and teardown of
	//    events.
	std::cout << TAB1 << "Get event node\n";

	GenApi::CNodePtr pEventTestTimestamp = pDevice->GetNodeMap()->GetNode("EventTestTimestamp");

	if (!pEventTestTimestamp)
	{
		throw GenICam::GenericException("Event node not found", __FILE__, __LINE__);
	}

	// Initialize events
	//    Initialize the events engine before registering the callback. Events
	//    nodes aren't available until the events engine has been initialized, so
	//    callback registration should come after events engine initialization.
	std::cout << TAB1 << "Initialize events\n";

	pDevice->InitializeEvents();

	// Register callback
	//    Register the callback before generating events. If an event is
	//    generated but no callbacks are registered, the events engine processes
	//    the event as normal, but nothing happens because no function has been
	//    registered to it.
	std::cout << TAB1 << "Register callback\n";

	GenApi::CallbackHandleType hCallback = GenApi::Register(pEventTestTimestamp, PrintNodeValue);

	// generate events
	std::cout << TAB1 << "Generate and wait on events\n";

	for (int i = 0; i < NUM_EVENTS; i++)
	{
		// Generate event
		//    Generate the event before waiting on it. Generating the event
		//    causes the event to occur, but does not process it or invoke the
		//    registered callback.
		std::cout << TAB2 << "Generate event\n";

		Arena::ExecuteNode(
			pDevice->GetNodeMap(),
			"TestEventGenerate");

		// Wait on event
		//    Wait on event to process it, invoking the registered callback. The
		//    data is created from the event generation, not from waiting on it.
		std::cout << TAB2 << "Wait on event\n";

		pDevice->WaitOnEvent(EVENT_TIMEOUT);
	}

	// Deregister callback
	//    Failing to deregister a callback results in a memory leak. Once a
	//    callback has been registered, it will no longer be invoked when a node
	//    is invalidated.
	std::cout << TAB1 << "Deregister callback\n";

	GenApi::Deregister(hCallback);

	// Deinitialize events
	//    Deinitialize the events engine in order to deallocate memory and stop
	//    processing events. Failing to deinitialize events results in a memory
	//    leak.
	std::cout << TAB1 << "Deinitialize events\n";

	pDevice->DeinitializeEvents();
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Callback_OnEvent\n";

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
		ConfigureCallbackOnEventTestTimestamp(pDevice);
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
