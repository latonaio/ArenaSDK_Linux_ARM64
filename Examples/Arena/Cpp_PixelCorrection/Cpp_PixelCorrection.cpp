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
#include <iomanip> // for std::setw

#define TAB1 "  "
#define TAB2 "    "
#define TAB3 "      "

// Pixel Correction
//    This example introduces the basics of pixel correction. A single arbitrary
//    pixel is chosen and added to the device's pixel correction list. These
//    changes are then saved to the camera before being removed.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Pixel values
//    This example does not search for pixels to correct, but instead just uses
//    arbitrary pixels to demonstrate what would be done if a bad pixel were
//    found.
#define PIXEL_X 256
#define PIXEL_Y 128

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates pixel correction
// (1) enables pixel correction
// (2) adds new pixel to correction list
// (3) sets new pixel coordinates
// (4) applies changes to correction list
// (5) saves correction to camera
// (6) removes pixel from correction list
void CorrectPixels(Arena::IDevice* pDevice, int64_t pixelX, int64_t pixelY)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	bool pixelCorrectionEnableInitial = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "DefectCorrectionEnable");

	// Enable pixel correction
	std::cout << TAB1 << "Enable pixel correction\n";

	Arena::SetNodeValue<bool>(
		pDevice->GetNodeMap(),
		"DefectCorrectionEnable",
		true);

	int64_t pixelCorrectionCountInitial = Arena::GetNodeValue<int64_t>(
		pDevice->GetNodeMap(),
		"DefectCorrectionCount");

	// Add new pixel to end of correction list.
	//    Getting a new defect automatically updates the pixel correction index.
	std::cout << TAB1 << "Add pixel to correction list\n";

	Arena::ExecuteNode(
		pDevice->GetNodeMap(),
		"DefectCorrectionGetNewDefect");

	// get updated pixel index
	int64_t pixelCorrectionUpdatedIndex = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "DefectCorrectionIndex");

	std::cout << TAB2 << "Pixel index: " << std::setw(2) << pixelCorrectionUpdatedIndex << " ";

	// Set pixel position
	Arena::SetNodeValue<int64_t>(
		pDevice->GetNodeMap(),
		"DefectCorrectionPositionX",
		pixelX);

	std::cout << "(x: " << std::setw(4) << Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "DefectCorrectionPositionX");

	Arena::SetNodeValue<int64_t>(
		pDevice->GetNodeMap(),
		"DefectCorrectionPositionY",
		pixelY);

	std::cout << ", y: " << std::setw(4) << Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "DefectCorrectionPositionY") << ")\n";

	// Apply correction list
	//    Once the pixel corrections are applied, they will take effect
	//    immediately. However, by power-cycling the camera, the defect list will
	//    go back to its default values.
	std::cout << TAB1 << "Apply correction\n";

	Arena::ExecuteNode(
		pDevice->GetNodeMap(),
		"DefectCorrectionApply");

	// Save correction to camera
	//    Optionally write the correction to the camera to make the changes
	//    persistent (the camera can still be set to default by executing the
	//    DefectCorrectionRestoreDefault node)
	std::cout << TAB1 << "Save correction to camera\n";

	Arena::ExecuteNode(
		pDevice->GetNodeMap(),
		"DefectCorrectionSave");

	// Remove all pixels set through this example by decrementing through the index
	//    The index is updated so that there are no empty indices after
	//    removal, regardless of position
	std::cout << TAB1 << "Find and remove pixel from correction list\n";

	for (int64_t pixelCorrectionIndex = pixelCorrectionUpdatedIndex; pixelCorrectionIndex >= pixelCorrectionCountInitial; pixelCorrectionIndex--)
	{
		// select pixel
		Arena::SetNodeValue<int64_t>(
			pDevice->GetNodeMap(),
			"DefectCorrectionIndex",
			pixelCorrectionIndex);

		std::cout << TAB2 << "Pixel index: " << std::setw(2) << pixelCorrectionIndex << " ";

		// check position
		int64_t x = Arena::GetNodeValue<int64_t>(
			pDevice->GetNodeMap(),
			"DefectCorrectionPositionX");

		std::cout << "(x: " << std::setw(4) << x;

		int64_t y = Arena::GetNodeValue<int64_t>(
			pDevice->GetNodeMap(),
			"DefectCorrectionPositionY");

		std::cout << ", y: " << std::setw(4) << y << ")";

		if (x == pixelX && y == pixelY)
		{
			std::cout << " matches\n" TAB2 "Remove pixel\n";

			// Delete pixel from correction list
			Arena::ExecuteNode(
				pDevice->GetNodeMap(),
				"DefectCorrectionRemove");
			break;
		}
		else
		{
			std::cout << " does not match\n";
		}
	}

	// return nodes to their initial values
	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "DefectCorrectionEnable", pixelCorrectionEnableInitial);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_PixelCorrection\n";

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
		int64_t pixelCorrectionCount = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "DefectCorrectionCount");
		// ensure arbitrary pixel selections are not already corrected
		if (pixelCorrectionCount > 0)
		{
			for (int64_t i = 0; i < pixelCorrectionCount; i++)
			{
				Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "DefectCorrectionIndex", i);
				if (Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "DefectCorrectionPositionX") == PIXEL_X && Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "DefectCorrectionPositionY") == PIXEL_Y)
				{
					std::cout << "\nPixels already corrected\nPress enter to complete\n";
					std::getchar();
					return -1;
				}
			}
		}

		// run example
		std::cout << "Commence example\n\n";
		CorrectPixels(pDevice, PIXEL_X, PIXEL_Y);
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
