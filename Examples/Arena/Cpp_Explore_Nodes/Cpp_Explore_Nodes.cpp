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
#include <iomanip>

// Explore: Nodes
//    This example explores traversing the nodes as a tree and fundamental node
//    information including display name, access mode visibility, interface type,
//    and value.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Choose node properties to explore
#define EXPLORE_ACCESS true
#define EXPLORE_VISIBILITY true
#define EXPLORE_TYPE true
#define EXPLORE_VALUE true

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// maintains display indentation
std::string Indent(size_t depth)
{
	std::string indentation = "  ";
	for (size_t i = 0; i < depth; i++)
		indentation += "  ";
	return indentation;
}

// explores node
// (1) retrieves display name
// (2) retrieves accessibility
// (3) retrieves visibility
// (4) retrieves interface type
// (5) retrieves value
void ExploreNode(GenApi::INode* pNode, size_t depth = 0)
{
	// Retrieve display name
	GenICam::gcstring displayName = pNode->GetDisplayName();

	// Retrieve accessibility
	GenApi::EAccessMode accessMode = pNode->GetAccessMode();
	GenICam::gcstring accessModeStr = GenApi::EAccessModeClass::ToString(accessMode);

	// Retrieve visibility
	GenApi::EVisibility visibility = pNode->GetVisibility();
	GenICam::gcstring visibilityStr = GenApi::EVisibilityClass::ToString(visibility);

	// Retrieve interface type
	GenApi::EInterfaceType interfaceType = pNode->GetPrincipalInterfaceType();
	GenICam::gcstring interfaceTypeStr = Arena::EInterfaceTypeClass::ToString(interfaceType);

	// Retrieve value
	GenICam::gcstring value = "-";
	if (GenApi::IsReadable(pNode))
	{
		GenApi::CValuePtr pValue = pNode;
		value = pValue->ToString();
	}

	// print node information
	std::cout << Indent(depth) << displayName << std::setw(50 - displayName.size() - (depth * 2)) << " ";

	if (EXPLORE_ACCESS)
		std::cout << accessModeStr << std::setw(5) << " ";

	if (EXPLORE_VISIBILITY)
		std::cout << visibilityStr << std::setw(14 - visibilityStr.size()) << " ";

	if (EXPLORE_TYPE)
		std::cout << interfaceTypeStr << std::setw(20 - interfaceTypeStr.size()) << " ";

	if (EXPLORE_VALUE)
		std::cout << (value.size() < 50 ? value : "...");

	std::cout << "\n";

	// Explore category node children
	GenApi::CCategoryPtr pCategory = pNode;
	if (pCategory)
	{
		GenApi::FeatureList_t children;
		pCategory->GetFeatures(children);
		for (GenApi::CValuePtr pValue : children)
			ExploreNode(pValue->GetNode(), depth + 1);
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

	std::cout << "Cpp_Explore_Nodes\n";

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
		GenApi::INode* pRoot = pDevice->GetNodeMap()->GetNode("Root");

		// run example
		std::cout << "Commence example\n\n";
		ExploreNode(pRoot);
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
