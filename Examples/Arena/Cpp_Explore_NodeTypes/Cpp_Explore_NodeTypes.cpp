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

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Explore: Node Types
//    This example explores the different properties of various node's types
//    including boolean, string, enumeration, integer, and float nodes. The user
//    inputs the node name that they wish to access (leaving out spacing between
//    words) in order to retrieve the node properties, or inputs 'x' to exit.
//    This example includes commented out code that sets values for each node
//    type, retrieves entry by node name for enumeration types, and imposes
//    maximum and minimum values for integer and float types. See
//    Cpp_Explore_Nodes for a complete list of nodes and their respective types.

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// explores nodes of boolean type
// (1) retrieves value
// (2) demonstrates value setter in a comment
void ExploreBoolean(GenApi::CBooleanPtr pBoolean)
{
	// Retrieve value
	std::cout << TAB3 << "Value: ";

	bool value = pBoolean->GetValue();

	std::cout << value << "\n";

	// Set value
	//    {
	//        bool value = true;
	//        pBoolean->SetValue(value);
	//    }
}

// explores nodes of string type
// (1) retrieves value
// (2) retrieves maximum value length
// (3) demonstrates value setter in a comment
void ExploreString(GenApi::CStringPtr pString)
{
	// Retrieve value
	std::cout << TAB3 << "Value: ";

	GenICam::gcstring value = pString->GetValue();

	std::cout << value << "\n";

	// Retrieve maximum length
	std::cout << TAB3 << "Maximum length: ";

	int64_t maxLen = pString->GetMaxLength();

	std::cout << maxLen << "\n";

	// Set value
	//    {
	//        GenICam::gcstring value = "string value";
	//        pString->SetValue(value);
	//    }
}

// explores nodes of type enumeration
// (1) retrieves currently set enum entry node
// (2) retrieves value
// (3) retrieves list of entries
// (4) demonstrates list of symbolics retrieval in a comment
// (5) demonstrates enum entry node retrieval by name in a comment
// (6) demonstrates value setter in a comment
void ExploreEnumeration(GenApi::CEnumerationPtr pEnumeration)
{
	// Retrieve current entry
	std::cout << TAB3 << "Current entry: ";

	GenApi::CEnumEntryPtr pCurrentEntry = pEnumeration->GetCurrentEntry();
	GenICam::gcstring currentEntrySymbolic = pCurrentEntry->GetSymbolic();

	std::cout << currentEntrySymbolic << "\n";

	// Retrieve integer value
	std::cout << TAB3 << "Integer value: ";

	int64_t intValue = pEnumeration->GetIntValue();

	std::cout << intValue << "\n";

	// Retrieve entries
	std::cout << TAB3 << "Entries: ";

	GenApi::NodeList_t entries;
	pEnumeration->GetEntries(entries);

	for (size_t i = 0; i < entries.size(); i++)
	{
		if (i > 0)
			std::cout << ", ";

		GenApi::CEnumEntryPtr pEntry = entries[i];
		std::cout << pEntry->GetSymbolic();
	}
	std::cout << "\n";

	// Retrieve symbolics
	//    {
	//        GenApi::StringList_t symbolics;
	//        pEnumeration->GetSymbolics(symbolics);
	//    }

	// Retrieve entry by name
	//    {
	//        GenICam::gcstring symbolic = "symbolic value";
	//        GenApi::CEnumEntryPtr pEntry = pEnumeration->GetEntryByName(symbolic);
	//    }

	// Set value
	//    {
	//        GenICam::gcstring symbolic = "symbolic value";
	//        GenApi::CEnumEntryPtr pEntry = pEnumeration->GetEntryByName(symbolic);
	//        int64_t intValue = pEntry->GetValue();
	//        pEnumeration->SetIntValue(intValue);
	//    }
}

// explores nodes of type integer
// (1) retrieves value
// (2) retrieves maximum and minimum
// (3) retrieves increment and increment mode
// (4) retrieves representation
// (5) retrieves unit
// (6) demonstrates maximum and minimum imposition in a comment
// (7) demonstrates value setter in a comment
void ExploreInteger(GenApi::CIntegerPtr pInteger)
{
	// Retrieve value
	std::cout << TAB3 << "Value: ";

	int64_t value = pInteger->GetValue();

	std::cout << value << "\n";

	// Retrieve range
	std::cout << TAB3 << "Maximum, minimum: ";

	int64_t max = pInteger->GetMax();
	int64_t min = pInteger->GetMin();

	std::cout << max << ", " << min << "\n";

	// Retrieve increment
	std::cout << TAB3 << "Increment (mode): ";

	int64_t inc = pInteger->GetInc();
	GenApi::EIncMode incMode = pInteger->GetIncMode();
	GenICam::gcstring incModeStr = Arena::EIncModeClass::ToString(incMode);

	std::cout << inc << " (" << incModeStr << ")\n";

	// Retrieve representation
	std::cout << TAB3 << "Representation: ";

	GenApi::ERepresentation rep = pInteger->GetRepresentation();
	GenICam::gcstring repStr = GenApi::ERepresentationClass::ToString(rep);

	std::cout << repStr << "\n";

	// Retrieve unit
	std::cout << TAB3 << "Unit: ";

	GenICam::gcstring unit = pInteger->GetUnit();

	std::cout << unit << "\n";

	// Impose maximum and minimum
	//    {
	//        int64_t newMax = 10;
	//        int64_t newMin = 0;
	//        pInteger->ImposeMax(newMax);
	//        pInteger->ImposeMin(newMin);
	//    }

	// Set value
	//    {
	//        int64_t value = 0;
	//        pInteger->SetValue(value);
	//    }
}

// explores nodes of type float
// (1) retrieves value
// (2) retrieves maximum and minimum
// (3) retrieves increment and increment mode
// (4) retrieves representation
// (5) retrieves unit
// (6) retrieves display notation
// (7) retrieves display precision
// (8) demonstrates maximum and minimum imposition in a comment
// (9) demonstrates value setter in a comment
void ExploreFloat(GenApi::CFloatPtr pFloat)
{
	// Retrieve value
	std::cout << TAB3 << "Value: ";

	double value = pFloat->GetValue();

	std::cout << value << "\n";

	// Retrieve range
	std::cout << TAB3 << "Maximum, minimum: ";

	double max = pFloat->GetMax();
	double min = pFloat->GetMin();

	std::cout << max << ", " << min << "\n";

	// Retrieve increment
	std::cout << TAB3 << "Increment (mode): ";

	bool hasIncrement = pFloat->HasInc();

	if (hasIncrement)
	{
		double inc = pFloat->GetInc();
		GenApi::EIncMode incMode = pFloat->GetIncMode();
		GenICam::gcstring incModeStr = Arena::EIncModeClass::ToString(incMode);

		std::cout << inc << " (" << incModeStr << ")\n";
	}
	else
	{
		std::cout << "no increment\n";
	}

	// Retrieve representation
	std::cout << TAB3 << "Representation: ";

	GenApi::ERepresentation rep = pFloat->GetRepresentation();
	GenICam::gcstring repStr = GenApi::ERepresentationClass::ToString(rep);

	std::cout << repStr << "\n";

	// Retrieve unit
	std::cout << TAB3 << "Unit: ";

	GenICam::gcstring unit = pFloat->GetUnit();

	std::cout << unit << "\n";

	// Retrieve display notation
	std::cout << TAB3 << "Display notation: ";

	GenApi::EDisplayNotation dispNotation = pFloat->GetDisplayNotation();
	GenICam::gcstring dispNotationStr = GenApi::EDisplayNotationClass::ToString(dispNotation);

	std::cout << dispNotationStr << "\n";

	// Retrieve display precision
	std::cout << TAB3 << "Display precision: ";

	int64_t dispPrecision = pFloat->GetDisplayPrecision();

	std::cout << dispPrecision << "\n";

	// Impose maximum and minimum
	//    {
	//        double newMax = 10;
	//        double newMin = 0;
	//        pFloat->ImposeMax(newMax);
	//        pFloat->ImposeMin(newMin);
	//    }

	// Set value
	//    {
	//        double value = 0;
	//        pFloat->SetValue(value);
	//    }
}

// controls node exploration
void ExploreNodes(GenApi::INodeMap* pNodeMap)
{
	GenICam::gcstring nodeName = "";
	std::cout << TAB1 << "Input node name to explore ('x' to exit)\n"
			<< TAB2;

	// stay in loop until exit
	while (true)
	{
		char ch = std::cin.get();

		// char(10), enter to accept
		if (ch == char(10))
		{
			// exit manually on 'x'
			if (nodeName == "x")
			{
				std::wcout << TAB2 << "Exit\n";
				break;
			}

			// get node
			GenApi::INode* pNode = pNodeMap->GetNode(nodeName);

			if (pNode)
			{
				// explore by type
				switch (pNode->GetPrincipalInterfaceType())
				{
					case GenApi::intfIBoolean:
						ExploreBoolean(pNode);
						break;
					case GenApi::intfIString:
						ExploreString(pNode);
						break;
					case GenApi::intfIEnumeration:
						ExploreEnumeration(pNode);
						break;
					case GenApi::intfIInteger:
						ExploreInteger(pNode);
						break;
					case GenApi::intfIFloat:
						ExploreFloat(pNode);
						break;
					default:
						std::cout << TAB3 << nodeName << " type not found\n";
				}
			}
			else
			{
				std::cout << TAB3 << nodeName << " not found\n";
			}

			// reset input
			nodeName = "";
			std::cout << TAB1 << "Input node name to explore ('x' to exit)\n"
					<< TAB2;
		}

		// continue accepting input
		else
		{
			nodeName += ch;
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

	std::cout << "Cpp_Explore_NodeTypes\n";

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
		GenApi::INodeMap* pNodeMap = pDevice->GetNodeMap();

		// run example
		std::cout << "Commence example\n\n";
		ExploreNodes(pNodeMap);
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
