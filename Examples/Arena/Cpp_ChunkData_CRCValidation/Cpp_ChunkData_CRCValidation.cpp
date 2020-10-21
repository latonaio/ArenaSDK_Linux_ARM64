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

// Chunk Data: CRC Validation
//    This example demonstrates the use of chunk data to verify data through a
//    Cyclical Redundancy Check (or CRC for short). CRCs are meant to check the
//    validity of sent data. It is performed by doing a series of calculations on
//    the raw data before and after it is sent. If the resultant integer values
//    match, then it is safe to assume the integrity of the data.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// image timeout
#define TIMEOUT 2000

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// configure device to receive CRC chunk and verify CRC
void ConfigureAndValidateCRC(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	bool chunkModeActiveInitial = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "ChunkModeActive");

	// Activate CRC chunk
	//    Activate chunk data and enable the CRC chunk before starting the
	//    stream. This involves activating chunk mode, selecting the CRC chunk,
	//    and enabling it.
	std::cout << TAB1 << "Activate chunk mode and enable CRC chunk\n";

	// activate chunk mode
	Arena::SetNodeValue<bool>(
		pDevice->GetNodeMap(),
		"ChunkModeActive",
		true);

	// enable CRC chunk
	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"ChunkSelector",
		"CRC");

	Arena::SetNodeValue<bool>(
		pDevice->GetNodeMap(),
		"ChunkEnable",
		true);

	// start stream and acquire image
	std::cout << TAB1 << "Start stream and acquire image\n";

	pDevice->StartStream();
	Arena::IImage* pImage = pDevice->GetImage(TIMEOUT);

	// Check for completeness
	//    Check for image completeness before checking the CRC. If an image is
	//    incomplete, the CRCs will definitely not match. Attempting to verify
	//    the CRC on an incomplete image will throw.
	std::cout << TAB2 << "Check for completeness\n";

	if (pImage->IsIncomplete())
	{
		std::cout << "\nError: Payload data incomplete. Please review network \nconfigurations, "
				  << "increase packet size, increase inter-packet \ndelay and/or reduce image size, then retry example\n";
		return;
	}

	// Calculate CRC from data
	//    The CRC can be calculated using a global Arena function. This global
	//    function is called to calculate the CRC internally when verifying the
	//    CRC.
	std::cout << TAB2 << "Calculate CRC from data";

	const uint8_t* pData = pImage->GetData();
	size_t imageDataSize = pImage->GetWidth() * pImage->GetHeight() * pImage->GetBitsPerPixel() / 8;

	int64_t calcCrc = Arena::CalculateCRC32(pData, imageDataSize);

	std::cout << " (" << calcCrc << ")\n";

	// Retrieve CRC from chunk
	//    When the chunk is enabled, the CRC is calculated by the device and sent
	//    as a piece of chunk data. The CRC value can then be retrieved as a
	//    chunk.
	std::cout << TAB2 << "Retrieve CRC from chunk";

	Arena::IChunkData* pChunkData = pImage->AsChunkData();
	GenApi::CIntegerPtr pChunkCRC = pChunkData->GetChunk("ChunkCRC");

	int64_t chunkCrc = pChunkCRC->GetValue();
	std::cout << " (" << chunkCrc << ")\n";

	// Compare CRCs
	//    Compare CRCs to verify whether the data is correct. If the calculated
	//    CRC and the chunk CRC match, then the data is assumed valid and
	//    correct; otherwise, invalid and incorrect.
	std::cout << TAB2 << "Compare chunk CRC to calculated CRC\n";

	if (chunkCrc == calcCrc)
	{
		std::cout << TAB3 << "CRCs match, data correct\n";
	}
	else
	{
		std::cout << TAB3 << "CRCs do not match, data incorrect\n";
	}

	// Validate CRC automatically
	//    The easiest way to validate a CRC is by using the function provided by
	//    Arena. The functions that verify CRC go through the same steps just
	//    explained above.
	std::cout << TAB2 << "Validate CRC automatically\n";

	if (pChunkData->VerifyCRC())
	{
		std::cout << TAB3 << "CRCs verified (CRCs match, data correct)\n";
	}
	else
	{
		std::cout << TAB3 << "CRCs invalid (CRCs do not match, data incorrect)\n";
	}

	// requeue buffer and stop stream
	std::cout << TAB1 << "Requeue buffer and stop stream\n";

	pDevice->RequeueBuffer(pImage);
	pDevice->StopStream();

	// return nodes to their initial values
	Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "ChunkModeActive", chunkModeActiveInitial);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_ChunkData_CRCValidation\n";

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
		ConfigureAndValidateCRC(pDevice);
		std::cout << "\nExample complete\n";

		// clean up example
		pSystem->DestroyDevice(pDevice);
		Arena::CloseSystem(pSystem);
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\nGenICam exception thrown: " << ge.what() << "\n";
		return -1;
	}
	catch (std::exception& ex)
	{
		std::cout << "\nStandard exception thrown: " << ex.what() << "\n";
		return -1;
	}
	catch (...)
	{
		std::cout << "\nUnexpected exception thrown\n";
		return -1;
	}

	std::cout << "Press enter to complete\n";
	std::getchar();

	if (exceptionThrown)
		return -1;
	else
		return 0;
}
