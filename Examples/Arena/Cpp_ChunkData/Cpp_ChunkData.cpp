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

// Chunk Data: Introduction
//    This example introduces the configuration and retrieval of chunk data.
//    Chunk data is data that has been tacked on to the end of image data in
//    order to provide useful information on the image. Configuring chunk data
//    involves activating chunk mode and enabling desired chunks. Retrieving
//    chunk data from an image is similar to retrieving nodes from a node map.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// image timeout
#define TIMEOUT 2000

// number of images to grab
#define NUM_IMAGES 5

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates chunk data
// (1) activates chunk mode
// (2) enables exposure and gain chunks
// (3) starts the stream and gets images
// (4) retrieves exposure and gain chunk data from image
// (5) requeues buffers and stops the stream
void ConfigureAndRetrieveChunkData(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	bool chunkModeActiveInitial = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "ChunkModeActive");
	bool chunkEnableInitial = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "ChunkEnable");

	// Activate chunk mode
	//    Activate chunk mode before enabling chunks; otherwise, ChunkSelector
	//    and ChunkEnable nodes will be unavailable.
	std::cout << TAB1 << "Activate chunk mode\n";

	Arena::SetNodeValue<bool>(
		pDevice->GetNodeMap(),
		"ChunkModeActive",
		true);

	// Enable exposure and gain chunks
	//    Enable desired chunks before starting the stream. Some chunks provide
	//    information already in an image while others not. This example looks at
	//    exposure and gain, two pieces of information unavailable from an image
	//    without chunk data.
	std::cout << TAB1 << "Enable exposure and gain chunks\n";

	// exposure time
	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"ChunkSelector",
		"ExposureTime");

	Arena::SetNodeValue<bool>(
		pDevice->GetNodeMap(),
		"ChunkEnable",
		true);

	// gain
	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"ChunkSelector",
		"Gain");

	Arena::SetNodeValue<bool>(
		pDevice->GetNodeMap(),
		"ChunkEnable",
		true);

	// Start stream and grab images
	//    Start streaming images with chunk data. Image data will now be appended
	//    with exposure and gain data because they were enabled.
	std::cout << TAB1 << "Start stream and grab images\n";

	pDevice->StartStream();
	std::vector<Arena::IImage*> images;

	for (int i = 0; i < NUM_IMAGES; i++)
	{
		std::cout << TAB2 << "Get image " << i << "\n";

		Arena::IImage* pImage = pDevice->GetImage(TIMEOUT);
		images.push_back(pImage);
	}

	// retrieve chunks
	std::cout << TAB1 << "Retrieve chunks\n";

	for (size_t i = 0; i < images.size(); i++)
	{
		// Cast to chunk data
		//    Cast the image to a chunk data object before retrieving chunks.
		//    Chunk data is present at the end of an image's raw data, but better
		//    accessed through this cast.
		std::cout << TAB2 << "Image " << i;

		Arena::IChunkData* pChunkData = images[i]->AsChunkData();

		// Check for completeness
		//    If an image is incomplete, it could be the case that the chunks
		//    hold garbage data value. If incomplete, chunks can still be
		//    retrieved but should be checked against null to verify the data
		//    exists.
		if (pChunkData->IsIncomplete())
		{
			std::cout << " (incomplete)";
			std::cout << "\n\nError: Payload data incomplete. Please review network \nconfigurations,"
					  << "increase packet size, increase inter-packet \ndelay and/or reduce image size, then retry example\n";
			return;
		}

		// Get exposure and gain chunks
		//    Chunk data is retrieved by getting chunks from a chunk data object.
		//    Chunks work the same way as nodes: they have a node type,
		//    additional information, and return null if they don't exist or
		//    cannot be found. For example, the exposure time chunk can access a
		//    maximum, minimum, display name, and unit, just like the exposure
		//    time node.
		GenApi::CFloatPtr pChunkExposureTime = pChunkData->GetChunk("ChunkExposureTime");
		double chunkExposureTime = pChunkExposureTime->GetValue();

		GenApi::CFloatPtr pChunkGain = pChunkData->GetChunk("ChunkGain");
		double chunkGain = pChunkGain->GetValue();

		std::cout << " (exposure = " << chunkExposureTime << ", gain = " << chunkGain << ")\n";
	}

	// requeue buffers
	std::cout << TAB1 << "Requeue buffers\n";

	for (size_t i = 0; i < images.size(); i++)
	{
		pDevice->RequeueBuffer(images[i]);
	}

	// stop stream
	std::cout << TAB1 << "Stop stream\n";

	pDevice->StopStream();

	// return nodes to their initial values
	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "ChunkModeActive", chunkModeActiveInitial);
	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "ChunkEnable", chunkEnableInitial);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_ChunkData\n";

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
		ConfigureAndRetrieveChunkData(pDevice);
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
