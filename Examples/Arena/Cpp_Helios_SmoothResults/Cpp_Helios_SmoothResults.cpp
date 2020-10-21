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

// Helios: Smooth Results
//    This example introduces setting different nodes specific to grabbing and saving
//    a 3D image with an emphasis on smooth results.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// file name
#define FILE_NAME "Images/Cpp_Helios_SmoothResults.ply"

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates saving an image
// (1) sets all relevant nodes targeted towards getting smooth results
// (2) gets image
// (3) prepares image parameters and writer
// (4) saves ply image
void AcquireImageWithSmoothResults(Arena::IDevice* pDevice)
{
	GenApi::INodeMap* pNodeMap = pDevice->GetNodeMap();

	// validate if Scan3dCoordinateSelector node exists. If not - probaly not Helios camera used running the example
	GenApi::CEnumerationPtr checkpCoordSelector = pNodeMap->GetNode("Scan3dCoordinateSelector");
	if (!checkpCoordSelector)
	{
		std::cout << TAB1 << "Scan3dCoordinateSelector node is not found. Please make sure that Helios device is used for the example.\n";
		return;
	}

	// validate if Scan3dCoordinateOffset node exists. If not - probaly Helios has an old firmware
	GenApi::CFloatPtr checkpCoord = pNodeMap->GetNode("Scan3dCoordinateOffset");
	if (!checkpCoord)
	{
		std::cout << TAB1 << "Scan3dCoordinateOffset node is not found. Please update Helios firmware.\n";
		return;
	}

	// get node values that will be changed in order to return their values at
	// the end of the example
	GenICam::gcstring pixelFormatInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat");
	GenICam::gcstring operatingModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "Scan3dOperatingMode");
	GenICam::gcstring exposureTimeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureTimeSelector");
	GenICam::gcstring conversionGainInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ConversionGain");
	int64_t imageAccumulationInitial = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Scan3dImageAccumulation");
	bool spatialFilterInitial = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "Scan3dSpatialFilterEnable");
	bool confidenceThresholdInitial = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "Scan3dConfidenceThresholdEnable");

	// set pixel format
	std::cout << TAB1 << "Set Coord3D_ABCY16s to pixel format\n";

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat", "Coord3D_ABCY16s");

	// set operating mode distance
	std::cout << TAB1 << "Set 3D operating mode to Distance1500mm\n";

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "Scan3dOperatingMode", "Distance1500mm");

	// set exposure time
	std::cout << TAB1 << "Set time selector to Exp1000Us\n";

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureTimeSelector", "Exp1000Us");

	// set gain
	std::cout << TAB1 << "Set conversion gain to low\n";

	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ConversionGain", "Low");

	// Set image accumulation
	std::cout << TAB1 << "Set image accumulation to 4\n";

	Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Scan3dImageAccumulation", 4);

	// Enable spatial filter
	std::cout << TAB1 << "Enable spatial filter\n";

	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "Scan3dSpatialFilterEnable", true);

	// Enable confidence threshold
	std::cout << TAB1 << "Enable confidence threshold\n\n";

	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "Scan3dConfidenceThresholdEnable", true);

	pDevice->StartStream();
	Arena::IImage* pImage = pDevice->GetImage(2000);

	// Prepare image parameters
	//    An image's width, height, and bits per pixel are required to
	//    save to disk. Its size and stride (i.e. pitch) can be
	//    calculated from those 3 inputs. Notice that an image's size and
	//    stride use bytes as a unit while the bits per pixel uses bits.
	std::cout << TAB2 << "Prepare image parameters\n";

	Save::ImageParams params(
		pImage->GetWidth(),
		pImage->GetHeight(),
		pImage->GetBitsPerPixel());

	// Prepare image writer
	//    The image writer requires 3 arguments to save an image: the
	//    image's parameters, a specified file name or pattern, and the
	//    image data to save. Providing these should result in a
	//    successfully saved file on the disk. Because an image's
	//    parameters and file name pattern may repeat, they can be passed
	//    into the image writer's constructor.
	std::cout << TAB2 << "Prepare image writer\n";

	Save::ImageWriter writer(
		params,
		FILE_NAME);

	// set parameters for SetPly()
	bool filterPoints = true;		 // default
	bool isSignedPixelFormat = true; // the example use Coord3D_ABCY16s
	float scale = 0.25f;			 // default
	float offsetA = 0.0f;			 // default
	float offsetB = 0.0f;			 // default
	float offsetC = 0.0f;			 // default

	// set the output file format of the image writer to .ply
	writer.SetPly(".ply", filterPoints, isSignedPixelFormat, scale, offsetA, offsetB, offsetC);

	writer << pImage->GetData();

	// save image
	std::cout << TAB2 << "Save image to " << writer.GetLastFileName() << "\n\n";

	// clean up example
	pDevice->RequeueBuffer(pImage);
	pDevice->StopStream();

	// return nodes to their initial values
	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "Scan3dConfidenceThresholdEnable", confidenceThresholdInitial);
	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "Scan3dSpatialFilterEnable", spatialFilterInitial);
	Arena::SetNodeValue<int64_t>(pDevice->GetNodeMap(), "Scan3dImageAccumulation", imageAccumulationInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ConversionGain", conversionGainInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureTimeSelector", exposureTimeInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "Scan3dOperatingMode", operatingModeInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat", pixelFormatInitial);
	std::cout << TAB1 << "Nodes were set back to initial values\n";
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Helios_SmoothResults\n";

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

		std::cout << "Commence example\n\n";

		// run example
		AcquireImageWithSmoothResults(pDevice);

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
