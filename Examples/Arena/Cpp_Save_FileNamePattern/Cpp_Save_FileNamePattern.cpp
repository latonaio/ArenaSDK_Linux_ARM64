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

// Save: File Name Pattern
//    This example demonstrates saving a set of images according to a file name
//    pattern, which uses the <count> and <timestamp> tags to differentiate
//    between saved images. The essential points of the example include setting
//    the image writer up with a file name pattern and using the cascading I/O
//    operator (<<) to update the timestamp and save each image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// File name pattern
//    File name patterns can use tags to easily customize your file names.
//    Customizable tags can be added to a file name pattern and later set on the
//    fly. Two tags, <count> and <datetime> have been built in to the save
//    library. As seen below, <datetime> can take an argument to specify output.
//    <count> also accepts arguments (local, path, and global) to specify what
//    exactly is being counted.
#define FILE_NAME_PATTERN "Images/Cpp_Save_FileNamePattern/<vendor>_<model>_<serial>_image<count>-<datetime:yyMMdd_hhmmss_fff>.bmp"

// pixel format
#define PIXEL_FORMAT BGR8

// number of images to acquire and save
#define NUM_IMAGES 25

// image timeout (milliseconds)
#define TIMEOUT 2000

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates saving images using a pattern
// (1) prepares image parameters
// (2) prepares image writer with a file name pattern, including <count> and <timestamp> tags
// (3) starts stream and grabs images
// (4) saves images each image without supplying new file names
// (5) requeues buffers and stops stream
void AcquireAndSaveImages(Arena::IDevice* pDevice)
{
	// get width, height, and pixel format nodes
	GenApi::CIntegerPtr pWidth = pDevice->GetNodeMap()->GetNode("Width");
	GenApi::CIntegerPtr pHeight = pDevice->GetNodeMap()->GetNode("Height");
	GenApi::CEnumerationPtr pPixelFormat = pDevice->GetNodeMap()->GetNode("PixelFormat");

	if (!pWidth || !pHeight || !pPixelFormat)
	{
		throw GenICam::GenericException("Width, Height, or PixelFormat node could not be found", __FILE__, __LINE__);
	}

	if (!GenApi::IsReadable(pWidth) || !GenApi::IsReadable(pHeight) || !GenApi::IsReadable(pPixelFormat))
	{
		throw GenICam::GenericException("Width, Height, or PixelFormat node not readable", __FILE__, __LINE__);
	}

	// Prepare image parameters from device settings
	//    An image's width, height, and bits per pixel are required to save to
	//    disk. Its size and stride (i.e. pitch) can be calculated from those 3
	//    inputs. Notice that an image's size and stride use bytes as a unit
	//    while the bits per pixel uses bits. Arena defines its pixel formats by
	//    the PFNC (Pixel Format Naming Convention), which embeds the number of
	//    bits per pixel within a pixel format's integer representation.
	std::cout << TAB1 << "Prepare image parameters\n";

	Save::ImageParams params(
		static_cast<size_t>(pWidth->GetValue()),
		static_cast<size_t>(pHeight->GetValue()),
		Arena::GetBitsPerPixel(pPixelFormat->GetCurrentEntry()->GetValue()));

	// Prepare image writer
	//    The image writer requires 3 arguments to save an image: the image's
	//    parameters, a specified file name or pattern, and the image data to
	//    save. If a file name pattern uses the <timestamp> tag, then a timestamp
	//    must also be provided. Providing these should result in a successfully
	//    saved file on the disk. Because an image's parameters and file name
	//    pattern may repeat, they can be passed into the image writer's
	//    constructor. However, they can also be passed in dynamically using the
	//    cascading I/O operator (<<).
	std::cout << TAB1 << "Prepare image writer\n";

	Save::ImageWriter writer(
		params,
		FILE_NAME_PATTERN);

	// Update tags
	//    Tags are set on the fly by passing strings into the cascading I/O
	//    operator. Tags are accepted as any string surrounded by angle brackets
	//    <...> while values are accepted as everything else. A value will be set
	//    to the last input tag.
	std::cout << TAB1 << "Update tags\n";

	// <vendor> tag
	std::cout << TAB2 << "<vendor> to LUCID\n";

	// get <model> tag
	GenICam::gcstring model = Arena::GetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"DeviceModelName");

	std::cout << TAB2 << "<model> to " << model << "\n";

	// get <serial> tag
	GenICam::gcstring serial = Arena::GetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"DeviceSerialNumber");

	std::cout << TAB2 << "<serial> to " << serial << "\n";

	// update
	writer << "<vendor>"
		   << "LUCID"

		   << "<model>"
		   << model

		   << "<serial>"
		   << serial;

	// start stream
	std::cout << TAB1 << "Start stream\n";

	pDevice->StartStream();

	// get images
	std::cout << TAB1 << "Get " << NUM_IMAGES << " images\n";

	for (int i = 0; i < NUM_IMAGES; i++)
	{
		std::cout << TAB2 << "Save image " << i;

		// get image
		Arena::IImage* pImage = pDevice->GetImage(TIMEOUT);

		// convert the image to a displayable pixel format.
		Arena::IImage* pConverted = Arena::ImageFactory::Convert(
			pImage,
			PIXEL_FORMAT);

		// save image
		writer << pConverted->GetData();

		// Get last file name
		//    The image writer allows for the retrieval of paths, file names, and
		//    extensions. They can be retrieved together or separately, and it is
		//    possible to get the pattern, peek ahead at the next file name, or
		//    get the last file name.
		std::cout << " at " << writer.GetLastFileName(true) << "\n";

		// destroy converted image
		Arena::ImageFactory::Destroy(pConverted);

		// requeue image buffer
		pDevice->RequeueBuffer(pImage);
	}

	// stop stream
	pDevice->StopStream();
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

// Example preparation & clean up
//    Prepares the example by opening the system, creating a device, and
//    configuring it. Cleans up the example by destroying the device and closing
//    the system.
int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Save_FileNamePattern\n";

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

		// run example
		std::cout << "Commence example\n\n";
		AcquireAndSaveImages(pDevice);
		std::cout << "\nExample complete." << std::endl;

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
