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

// Explore: Node Maps
//    This example explores the 5 available node maps of Arena, 4 retrieved from
//    any devices and 1 from the system. It demonstrates traversing nodes
//    retrieved as a complete vector.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Choose node maps to explore
#define EXPLORE_DEVICE true
#define EXPLORE_TL_DEVICE true
#define EXPLORE_TL_STREAM true
#define EXPLORE_TL_INTERFACE true
#define EXPLORE_TL_SYSTEM true

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// explores a node map
// (1) retrieves total number of nodes
// (2) retrieves category nodes
void ExploreNodeMap(GenApi::INodeMap* pNodeMap)
{
	// Get number of nodes
	std::cout << TAB2 << "Number of nodes: ";

	uint64_t numNodes = pNodeMap->GetNumNodes();

	std::cout << numNodes << "\n";

	// Get nodes
	GenApi::NodeList_t nodes;

	pNodeMap->GetNodes(nodes);

	// print category nodes
	std::cout << TAB2 << "Category nodes: ";
	bool firstPass = true;

	for (GenApi::CCategoryPtr pCategoryNode : nodes)
	{
		if (pCategoryNode)
		{
			if (firstPass)
			{
				std::cout << pCategoryNode->GetNode()->GetDisplayName();
				firstPass = false;
			}
			else
				std::cout << ", " << pCategoryNode->GetNode()->GetDisplayName();
		}
	}

	std::cout << "\n";
}

// explores node maps
// (1) retrieves node map from device
// (2) retrieves node maps from corresponding transport layer modules
// (3) explores the device node map
// (4) explores the transport layer device node map
// (5) explores the transport layer stream node map
// (6) explores the transport layer interface node map
// (7) explores the transport layer system node map
void ExploreNodeMaps(Arena::ISystem* pSystem, Arena::IDevice* pDevice)
{
	std::cout << TAB1 << "Retrieve node maps\n";

	// Retrieve node map from device
	GenApi::INodeMap* pNodeMap = pDevice->GetNodeMap();

	// Retrieve node maps from corresponding transport layer modules
	GenApi::INodeMap* pTLDeviceNodeMap = pDevice->GetTLDeviceNodeMap();
	GenApi::INodeMap* pTLStreamNodeMap = pDevice->GetTLStreamNodeMap();
	GenApi::INodeMap* pTLInterfaceNodeMap = pDevice->GetTLInterfaceNodeMap();
	GenApi::INodeMap* pTLSystemNodeMap = pSystem->GetTLSystemNodeMap();

	// Explore device node map
	if (EXPLORE_DEVICE)
	{
		std::cout << TAB1 << "Explore device node map\n";

		ExploreNodeMap(pNodeMap);
	}

	// Explore transport layer device node map
	if (EXPLORE_TL_DEVICE)
	{
		std::cout << TAB1 << "Explore transport layer device node map\n";

		ExploreNodeMap(pTLDeviceNodeMap);
	}

	// Explore transport layer stream node map
	if (EXPLORE_TL_STREAM)
	{
		std::cout << TAB1 << "Explore transport layer stream node map\n";

		ExploreNodeMap(pTLStreamNodeMap);
	}

	// Explore transport layer interface node map
	if (EXPLORE_TL_INTERFACE)
	{
		std::cout << TAB1 << "Explore transport layer interface node map\n";

		ExploreNodeMap(pTLInterfaceNodeMap);
	}

	// Explore transport layer system node map
	if (EXPLORE_TL_SYSTEM)
	{
		std::cout << TAB1 << "Explore transport layer system node map\n";

		ExploreNodeMap(pTLSystemNodeMap);
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

	std::cout << "Cpp_Explore_NodeMaps\n";

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
		ExploreNodeMaps(pSystem, pDevice);
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
