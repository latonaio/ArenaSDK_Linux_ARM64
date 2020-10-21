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
#include "SaveApi.h"

#define TAB1 "  "
#define TAB2 "    "

// Lookup Tables: Introduction
//    This example introduces the lookup tables (LUT), which are used to
//    transform image data into a desired output format. LUTs give an output
//    value for each of a range of index values. This example enables a lookup
//    table node to invert the intensity of a single image. This is done by
//    accessing the LUT index node and setting the LUT node values to the newly
//    calculated pixel intensity value. It takes some time to update each pixel
//    with the new value. The example then saves the new image by saving to the
//    image writer.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Slope for calculating the new intensity value to set
#define SLOPE -1

// File name
#define FILE_NAME "Images/Cpp_LUT/image.png"

// timeout for detecting camera devices (in milliseconds).
#define SYSTEM_TIMEOUT 100

// image timeout
#define IMAGE_TIMEOUT 2000

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates using the lookup table to invert intensity
// (1) enables lookup table
// (2) substitutes each pixel with its inverted value
// (3) retrieves image
// (4) cleans up image
void InvertIntensity(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	bool lutEnableInitial = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "LUTEnable");

	// Enable lookup table
	std::cout << TAB1 << "Enable lookup table\n";

	Arena::SetNodeValue<bool>(
		pDevice->GetNodeMap(),
		"LUTEnable",
		true);

	// Invert values
	std::cout << TAB1 << "Invert values\n";

	// get nodes for LUT index and value
	GenApi::CIntegerPtr pLUTIndex = pDevice->GetNodeMap()->GetNode("LUTIndex");
	GenApi::CIntegerPtr pLUTValue = pDevice->GetNodeMap()->GetNode("LUTValue");
	if (!pLUTIndex || !pLUTValue)
	{
		throw GenICam::GenericException("Requisite node(s) LUTIndex and/or LUTValue do(es) not exist", __FILE__, __LINE__);
	}

	for (int64_t i = 0; i <= pLUTIndex->GetMax(); i++)
	{
		// select pixel value
		pLUTIndex->SetValue(i);

		// set substitution value
		int64_t value = (SLOPE * i) + pLUTIndex->GetMax();
		pLUTValue->SetValue(value);

		if (i % 1024 == 0)
			std::cout << TAB2;

		if (i % 256 == 255)
			std::cout << '.';

		if (i % 1024 == 1023)
			std::cout << '\n';
	}

	// get image
	pDevice->StartStream();
	Arena::IImage* pImage = pDevice->GetImage(IMAGE_TIMEOUT);

	// save image
	Save::ImageParams params(
		pImage->GetWidth(),
		pImage->GetHeight(),
		pImage->GetBitsPerPixel());
	Save::ImageWriter writer(
		params,
		FILE_NAME);
	writer << pImage->GetData();

	// clean up image
	pDevice->RequeueBuffer(pImage);
	pDevice->StopStream();

	// return nodes to their initial values
	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "LUTEnable", lutEnableInitial);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_LUT\n";
	std::cout << "Example may change device settings -- proceed? ('y' to continue) ";
	char continueExample = 'a';
	std::cin >> continueExample;

	if (continueExample == 'y')
	{
		try
		{
			// prepare example
			Arena::ISystem* pSystem = Arena::OpenSystem();
			pSystem->UpdateDevices(SYSTEM_TIMEOUT);
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
			InvertIntensity(pDevice);
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
