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

// Save: Introduction
//    This example introduces the basic save capabilities of the save library. It
//    shows the construction of an image parameters object and an image writer,
//    and saves a single image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// pixel format
#define PIXEL_FORMAT BGR8

// File name
//    The relative path and file name to save to. After running the example, an
//    image should exist at the location specified. The image writer chooses the
//    file format by the image's extension. Aside from PNG (.png), images can be
//    saved as JPEG (.jpg), TIFF (.tiff), BMP (.bmp), and raw (.raw) files. For
//    3D file formats the PLY (.ply) extension can be used.
#define FILE_NAME "Images/Cpp_Save_Ply/Cpp_Save_Ply.ply"

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

bool ValidateDevice(Arena::IDevice* pDevice)
{
	GenApi::INodeMap* pNodeMap = pDevice->GetNodeMap();

	// validate if Scan3dCoordinateSelector node exists. If not - probaly not Helios camera used running the example
	GenApi::CEnumerationPtr checkpCoordSelector = pNodeMap->GetNode("Scan3dCoordinateSelector");
	// validate if Scan3dCoordinateOffset node exists. If not - probaly Helios has an old firmware
	GenApi::CFloatPtr checkpCoord = pNodeMap->GetNode("Scan3dCoordinateOffset");

	if (!checkpCoordSelector)
	{
		std::cout << TAB1 << "Scan3dCoordinateSelector node is not found. Please make sure that Helios device is used for the example.\n\n";
		return false;
	}
	else if (!checkpCoord)

	{
		std::cout << TAB1 << "Scan3dCoordinateOffset node is not found. Please update Helios firmware.\n\n";
		return false;
	}
	else
	{
		return true;
	}
}

// demonstrates saving an image
// (1) prepares image parameters
// (2) prepares image writer
// (3) saves image
void SaveImage(Arena::IImage* pImage, const char* filename)
{
	bool isSignedPixelFormat = false;

	if ((pImage->GetPixelFormat() == Coord3D_ABC16s) || (pImage->GetPixelFormat() == Coord3D_ABCY16s))
	{
		isSignedPixelFormat = true;
	}

	// Prepare image parameters
	//    An image's width, height, and bits per pixel are required to save to
	//    disk. Its size and stride (i.e. pitch) can be calculated from those 3
	//    inputs. Notice that an image's size and stride use bytes as a unit
	//    while the bits per pixel uses bits.
	std::cout << TAB1 << "Prepare image parameters\n";

	Save::ImageParams params(
		pImage->GetWidth(),
		pImage->GetHeight(),
		pImage->GetBitsPerPixel());

	// Prepare image writer
	//    The image writer requires 3 arguments to save an image: the image's
	//    parameters, a specified file name or pattern, and the image data to
	//    save. Providing these should result in a successfully saved file on the
	//    disk. Because an image's parameters and file name pattern may repeat,
	//    they can be passed into the image writer's constructor.
	std::cout << TAB1 << "Prepare image writer\n";

	Save::ImageWriter writer(
		params,
		filename);

	// set default parameters for SetPly()
	bool filterPoints = true;
	float scale = 0.25f;
	float offsetA = 0.0f;
	float offsetB = 0.0f;
	float offsetC = 0.0f;

	// set the output file format of the image writer to .ply
	writer.SetPly(".ply", filterPoints, isSignedPixelFormat, scale, offsetA, offsetB, offsetC);

	// Save image
	//    Passing image data into the image writer using the cascading I/O
	//    operator (<<) triggers a save. Notice that the << operator accepts the
	//    image data as a constant unsigned 8-bit integer pointer (const
	//    uint8_t*) and the file name as a character string (const char*).
	std::cout << TAB1 << "Save image\n";

	writer << pImage->GetData();
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Save_Ply\n\n";

	try
	{
		// prepare example
		Arena::ISystem* pSystem = Arena::OpenSystem();
		pSystem->UpdateDevices(100);
		std::vector<Arena::DeviceInfo> devices = pSystem->GetDevices();
		if (devices.size() == 0)
		{
			std::cout << "\nNo camera connected\nPress enter to complete\n";
			std::getchar();
			return 0;
		}
		Arena::IDevice* pDevice = pSystem->CreateDevice(devices[0]);
		pDevice->StartStream();
		Arena::IImage* pImage = pDevice->GetImage(2000);

		bool isDeviceValid = ValidateDevice(pDevice);

		if (isDeviceValid == true)

		{
			// run example

			if ((pImage->GetPixelFormat() == Coord3D_ABC16) || (pImage->GetPixelFormat() == Coord3D_ABCY16) || (pImage->GetPixelFormat() == Coord3D_ABC16s) || (pImage->GetPixelFormat() == Coord3D_ABCY16s))
			{
				std::cout << "Commence example\n\n";
				SaveImage(pImage, FILE_NAME);
				std::cout << "\nExample complete\n";
			}
			else
			{
				std::cout << "This example requires the camera to be in a 3D image format like Coord3D_ABC16, Coord3D_ABCY16, Coord3D_ABC16s or Coord3D_ABCY16s\n\n";
			}
		}

		// clean up example
		pDevice->RequeueBuffer(pImage);
		pDevice->StopStream();
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
