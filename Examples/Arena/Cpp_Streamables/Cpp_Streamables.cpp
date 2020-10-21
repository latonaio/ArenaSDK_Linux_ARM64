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

// Streamables
//    This example introduces streamables, which uses files to pass settings
//    around between devices. This example writes all streamable features from a
//    source device to a file, and then writes them from the file to all other
//    connected devices.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// the name of the file to stream features to/from
#define FILE_NAME "allStreamableFeatures.txt"

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates streamable features
// (1) reads all streamable features from source device
// (2) writes features to file
// (3) reads features from file
// (4) writes features to destination devices
void WriteAndReadStreamables(Arena::IDevice* pSrcDevice, std::vector<Arena::IDevice*> dstDevices)
{
	// Write features to file
	//    Write streamable features from a device to a file. Each node map
	//    requires a separate feature stream object. When writing to a file, if
	//    no features are explicitly selected, all will be written by default.
	std::cout << TAB1 << "Save features from device " << Arena::GetNodeValue<GenICam::gcstring>(pSrcDevice->GetTLDeviceNodeMap(), "DeviceSerialNumber") << " to " << FILE_NAME << "\n";

	Arena::FeatureStream featureStreamSrc(pSrcDevice->GetNodeMap());
	featureStreamSrc.Write("allStreamableFeatures.txt");

	// Read features to devices
	//    Read streamable features from from a file to the rest of the devices.
	//    Again, each node map requires a separate feature stream object. When
	//    reading from a file, all features saved to the file will be loaded to
	//    the device. If a device does not have a feature, it is skipped.
	for (size_t i = 0; i < dstDevices.size(); i++)
	{
		std::cout << TAB1 << "Load features from " << FILE_NAME << " to device " << Arena::GetNodeValue<GenICam::gcstring>(dstDevices[i]->GetTLDeviceNodeMap(), "DeviceSerialNumber") << "\n";

		Arena::FeatureStream featureStreamDst(dstDevices.at(i)->GetNodeMap());
		featureStreamDst.Read("allStreamableFeatures.txt");
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

	std::cout << "Cpp_Streamables\n";
	std::cout << "Example may change device settings -- proceed? ('y' to continue) ";
	char continueExample = 'a';
	std::cin >> continueExample;

	if (continueExample == 'y')
	{
		try
		{
			// prepare example
			Arena::ISystem* pSystem = Arena::OpenSystem();
			pSystem->UpdateDevices(100);
			std::vector<Arena::DeviceInfo> devices = pSystem->GetDevices();
			if (devices.size() == 0)
			{
				std::cout << "\nNo camera connected\nPress enter to complete\n";

				// clear input
				while (std::cin.get() != '\n')
					continue;

				std::getchar();
				return 0;
			}
			else if (devices.size() == 1)
			{
				std::cout << "Warning: only one device connected. Example runs best with at least 2 devices\n";
			}
			Arena::IDevice* pSrcDevice = nullptr;
			std::vector<Arena::IDevice*> dstDevices;
			for (size_t i = 0; i < devices.size(); i++)
			{
				Arena::IDevice* pDevice = pSystem->CreateDevice(devices.at(i));
				if (i == 0)
				{
					pSrcDevice = pDevice;
				}
				else
				{
					dstDevices.push_back(pDevice);
				}
			}

			// run example
			std::cout << "Commence example\n\n";
			WriteAndReadStreamables(pSrcDevice, dstDevices);
			std::cout << "\nExample complete\n";

			// clean up example
			pSystem->DestroyDevice(pSrcDevice);

			for (size_t i = 0; i < dstDevices.size(); i++)
			{
				pSystem->DestroyDevice(dstDevices.at(i));
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
