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

// Callbacks: On Node Change
//    This example demonstrates configuring a callback to be invoked when a node
//    is invalidated. A node is invalidated when its value changes or can be
//    invalidated manually. In this example, a callback is registered on
//    PayloadSize. The example shows two ways to invoke a callback: first by
//    changing the value of a dependent node (Height) and then by invalidating
//    PayloadSize manually. Whenever the callback is triggered, the callback
//    function prints the updated value of the invalidated node.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Height values
//    The example changes the value of the height node twice to ensure that the
//    height is changed.
#define HEIGHT_ONE 256
#define HEIGHT_TWO 512

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// callback function
// (1) prints information from the callback
void PrintNodeValue(GenApi::INode* pNode)
{
	// Do something on callback
	//    This function is registered with the callback and invoked when the
	//    registered node is invalidated. In this case, the callback prints the
	//    display name and value of the invalidated node.
	GenApi::CValuePtr pValue = pNode;

	std::cout << TAB3 << "Message from callback\n";
	std::cout << TAB4 << pNode->GetDisplayName() << ": " << pValue->ToString() << "\n";
}

// demonstrates callbacks invoked on node changes
// (1) registers callback on node PayloadSize
// (2) changes Height twice to invalidate PayloadSize, invoking callback
// (3) invalidates PayloadSize manually
// (4) deregisters callback
void ConfigureAndCauseCallback(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	int64_t heightInitial = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height");

	// Register PayloadSize for callbacks
	//    Callbacks are registered with a node and a function. This example
	//    demonstrates callbacks being invoked when the node is invalidated. This
	//    could be when the node value changes, either manually or by the device,
	//    or when the node is invalidated manually.
	std::cout << TAB1 << "Register callback on PayloadSize\n";

	// get node
	GenApi::CIntegerPtr pPayloadSize = pDevice->GetNodeMap()->GetNode("PayloadSize");

	if (!pPayloadSize || !GenApi::IsReadable(pPayloadSize))
	{
		throw GenICam::GenericException("PayloadSize node not found/readable", __FILE__, __LINE__);
	}

	// register callback
	GenApi::CallbackHandleType hCallback = GenApi::Register(pPayloadSize->GetNode(), PrintNodeValue);

	// Modify Height to invoke callback on PayloadSize
	//    The value of PayloadSize depends on a number of other nodes. This
	//    includes Height. Therefore, changing the value of Height changes the
	//    value of and invalidates PayloadSize, which then invokes the callback.
	std::cout << TAB2 << "Change height once\n";

	Arena::SetNodeValue<int64_t>(
		pDevice->GetNodeMap(),
		"Height",
		HEIGHT_ONE);

	std::cout << TAB2 << "Change height twice\n";

	Arena::SetNodeValue<int64_t>(
		pDevice->GetNodeMap(),
		"Height",
		HEIGHT_TWO);

	// Manually invalidate PayloadSize for callback
	//    Apart from changing the value of a node, nodes can be invalidated
	//    manually by calling InvalidateNode. This also invokes the callback.
	std::cout << TAB2 << "Invalidate PayloadSize\n";

	pDevice->GetNodeMap()->GetNode("PayloadSize")->InvalidateNode();

	// Deregister callback
	//    Failing to deregister a callback results in a memory leak. Once a
	//    callback has been registered, it will no longer be invoked when a node
	//    is invalidated.
	std::cout << TAB1 << "Deregister callback\n";

	GenApi::Deregister(hCallback);

	// return nodes to their initial values
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

	std::cout << "Cpp_Callback_OnNodeChange\n";

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
		ConfigureAndCauseCallback(pDevice);
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
