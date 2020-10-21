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

// User sets
//    This example introduces user sets, a feature which allows for the saving
//    and loading of multiple configurations. The example starts by changing two
//    features, width and height. The device configuration is then saved to user
//    set 1. The default user set is then loaded, followed by user set 1 again to
//    demonstrate the the device configuration changing back and forth.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// width value to save/load
#define WIDTH 576

// height value to load/save
#define HEIGHT 512

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates user sets
// (1) gets width and height nodes
// (2) changes width and height
// (3) saves user set with width and height changes
// (4) loads default user set, changing width and height to defaults
// (5) loads user set, changing width and height back
void SaveAndLoadUserSets(Arena::IDevice* pDevice)
{
	// Get width and height nodes
	//    This example focuses on width and height nodes to demonstrate the
	//    saving and loading of user sets.
	std::cout << TAB1 << "Get width and height nodes\n";

	GenApi::CIntegerPtr pWidth = pDevice->GetNodeMap()->GetNode("Width");
	GenApi::CIntegerPtr pHeight = pDevice->GetNodeMap()->GetNode("Height");

	if (!pWidth || !GenApi::IsReadable(pWidth) || !GenApi::IsWritable(pWidth))
	{
		throw GenICam::GenericException("Width node not found/readable/writable", __FILE__, __LINE__);
	}

	if (!pHeight || !GenApi::IsReadable(pHeight) || !GenApi::IsWritable(pHeight))
	{
		throw GenICam::GenericException("Height node not found/readable/writable", __FILE__, __LINE__);
	}

	// change width and height
	std::cout << TAB1 << "Change width and height\n";

	pWidth->SetValue(WIDTH);
	pHeight->SetValue(HEIGHT);

	// Save to user set 1
	//    Saving the user set saves the new width and height values. Saving a
	//    user set involves selecting the user set to save to on the selector
	//    node and then executing the save on the command node.
	std::cout << TAB1 << "Save to user set 1\n";

	// select user set 1
	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"UserSetSelector",
		"UserSet1");

	// execute the save
	Arena::ExecuteNode(
		pDevice->GetNodeMap(),
		"UserSetSave");

	// check width and height
	std::cout << TAB2 << "Width " << pWidth->GetValue() << "\n";
	std::cout << TAB2 << "Height " << pHeight->GetValue() << "\n";

	// Load default user set
	//    Loading a user set is quite similar to saving: select the user set to
	//    load using the selector node and then execute the load using the
	//    command node. Notice that loading the default user set changes the
	//    width and height values.
	std::cout << TAB1 << "Load default user set\n";

	// select default user set
	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"UserSetSelector",
		"Default");

	// execute the load
	Arena::ExecuteNode(
		pDevice->GetNodeMap(),
		"UserSetLoad");

	// check width and height
	std::cout << TAB2 << "Width " << pWidth->GetValue() << "\n";
	std::cout << TAB2 << "Height " << pHeight->GetValue() << "\n";

	// Load user set 1
	//    Load user set 1 to return width and height to the values that were
	//    saved earlier. The process is the same as above.
	std::cout << TAB1 << "Load user set 1\n";

	// select user set 1
	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"UserSetSelector",
		"UserSet1");

	// execute the load
	Arena::ExecuteNode(
		pDevice->GetNodeMap(),
		"UserSetLoad");

	// check width and height
	std::cout << TAB2 << "Width " << pWidth->GetValue() << "\n";
	std::cout << TAB2 << "Height " << pHeight->GetValue() << "\n";
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_UserSets\n";
	std::cout << "Example may overwrite device settings saved to user set 1 -- proceed? ('y' to continue) ";
	char continueExample = 'a';
	std::cin >> continueExample;

	if (continueExample == 'y')
	{
		try
		{
			// prepare example
			Arena::ISystem* pSystem = Arena::OpenSystem();
			pSystem->UpdateDevices(100);
			std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();
			if (deviceInfos.size() == 0)
			{
				std::cout << "\nNo camera connected\nPress enter to complete\n";

				// clear input
				while (std::cin.get() != '\n')
					continue;
				std::getchar();
				return 0;
			}
			Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfos[0]);

			// run example
			std::cout << "Commence example\n\n";
			SaveAndLoadUserSets(pDevice);
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
	}

	std::cout << "Press enter to complete\n";

	// clear input
	while (std::cin.get() != '\n')
		continue;

	std::getchar();

	if (exceptionThrown)
		return -1;
	else
		return 0;
}
