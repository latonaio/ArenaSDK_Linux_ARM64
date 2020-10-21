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

#define _USE_MATH_DEFINES
#include <math.h>

#define TAB1 "  "
#define TAB2 "    "

// Polarization, DoLP AoLP
//    This example introduces the basics of working with polarization pixel
//    formats. Specifically, this examples retrieves a 2-channel DoLP, AoLP
//    image, PolarizedDolpAolp_Mono8, and converts it to a BGR image for saving,
//    treating the AoLP data as hue, and the DoLP data as saturation.  This
//    example shows only how to convert the 8-bit-per-channel form of the pixel
//    format. The 12p-bit-per-channel form, while similar, will not produce a
//    correct result.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// name of file to save
#define FILE_NAME_PATTERN "Images/Cpp_Polarization_DolpAolp.jpg"

// pixel format
#define PIXEL_FORMAT BGR8

// image timeout
#define IMAGE_TIMEOUT 2000

// system timeout
#define SYSTEM_TIMEOUT 100

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates acquisition
// (1) acquires image
// (2) manually converts to HSV
// (3) treats AoLP as hue
// (4) treats DoLP as saturation
// (5) converts HSV to displayable pixel format
// (6) saves to disk
void ConvertDoLPAoLPToPixelFormat(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	GenICam::gcstring pixelFormatInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat");

	// Change to DoLP AoLP pixel format
	//    DoLP AoLP pixel formats are a 2-channel image. The first channel holds
	//    DoLP (Degree of Linear Polarization) data while the second holds AoLP
	//    (Angle of Linear Polarization) data.
	std::cout << TAB1 << "Set PolarizedDolpAolp_Mono8 to pixel format\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"PixelFormat",
		"PolarizedDolpAolp_Mono8");

	// retrieve image
	std::cout << TAB1 << "Acquire image\n";

	pDevice->StartStream();
	Arena::IImage* pImage = pDevice->GetImage(IMAGE_TIMEOUT);

	// Manually convert to HSV image
	//     Treat the AoLP data as hue and the DoLP data as saturation, and manually
	//     convert from HSV to the desired pixel format: algorithm available on Wikipedia:
	//     https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV
	size_t width = pImage->GetWidth();
	size_t height = pImage->GetHeight();
	size_t size = width * height;
	size_t srcBpp = pImage->GetBitsPerPixel();
	size_t srcPixelSize = srcBpp / 8;
	uint64_t srcPixelFormat = pImage->GetPixelFormat();
	const uint8_t* pInput = pImage->GetData();

	// prepare memory output buffer
	size_t dstBpp = Arena::GetBitsPerPixel(PIXEL_FORMAT);
	size_t dstPixelSize = dstBpp / 8;
	size_t dstDataSize = width * height * dstBpp / 8;
	uint8_t* pOutput = new uint8_t[dstDataSize];
	memset(pOutput, 0, dstDataSize);

	// Convert 2-channel 8-bits-per-channel DoLP AoLP
	//    The first channel of each pixel holds the DoLP data and has a maximum
	//    value of 255; the second channel of each pixel holds the AoLP data and
	//    has a maximum value of 201.
	std::cout << TAB1 << "Using AoLP as hue and DoLP as saturation, convert from HSV to " << PIXEL_FORMAT << "\n";

	if (srcPixelFormat == LUCID_PolarizedDolpAolp_Mono8)
	{
		const uint8_t* pIn = pInput;
		uint8_t* pOut = pOutput;
		for (size_t i = 0; i < size; i++)
		{
			// Separate the AoLP and DoLP channels
			//    The first channel is the DoLP (degree of linear polarization)
			//    channel. The second channel is the AoLP (angle of linear
			//    polarization) channel.
			double dolpValue = static_cast<double>(*pIn);
			double aolpValue = static_cast<double>(*(pIn + 1));

			// Map to hue, saturation, and value
			//    For the formula to work, double the AoLP for the hue and map
			//    the DoLP values between 0 and 1 for saturation.  Maximize value
			//    to keep things bright.
			double hue = (aolpValue)*2.0;
			if (hue > 255.0)
				hue = 255.0;
			double saturation = dolpValue / 255.0;
			double value = 255.0;

			// Calculate chroma, hue', and x (second largest component)
			//    These intermediary values help in the conversion of HSV to the
			//    displayable pixel format.
			double c = value * saturation;
			double h = hue / 60.0;
			double x = c * (1 - abs(fmod(h, 2.0) - 1));

			// Calculate blue, green, and red
			//    Blue, green, and red can be calculated from hue, saturation,
			//    and value, and the intermediary values chroma, hue', and x.
			double blue = 0.0;
			double green = 0.0;
			double red = 0.0;

			// colors between red and yellow
			if ((h >= 0) && (h <= 1))
			{
				blue = value - c;
				green = x + value - c;
				red = value;
			}

			// colors between yellow and green
			else if ((h >= 1) && (h <= 2))
			{
				blue = value - c;
				green = value;
				red = x + value - c;
			}

			// colors between green and cyan
			else if ((h >= 2) && (h <= 3))
			{
				blue = x + value - c;
				green = value;
				red = value - c;
			}

			// colors between cyan and blue
			else if ((h >= 3) && (h <= 4))
			{
				blue = value;
				green = x + value - c;
				red = value - c;
			}

			// colors between blue and magenta
			else if ((h >= 4) && (h <= 5))
			{
				blue = value;
				green = value - c;
				red = x + value - c;
			}

			// colors between magenta and red
			else if ((h >= 5) && (h <= 6))
			{
				blue = x + value - c;
				green = value - c;
				red = value;
			}

			// set pixel format values and move to next pixel
			*pOut = static_cast<uint8_t>(blue);
			*(pOut + 1) = static_cast<uint8_t>(green);
			*(pOut + 2) = static_cast<uint8_t>(red);

			pIn += srcPixelSize;
			pOut += dstPixelSize;
		}
	}
	else
	{
		throw GenICam::GenericException("This example requires PolarizedDolpAolp_Mono8 pixel format", __FILE__, __LINE__);
	}

	// create image from buffer and save
	std::cout << TAB1 << "Save image to ";

	Arena::IImage* pCreate = Arena::ImageFactory::Create(pOutput, dstDataSize, width, height, PIXEL_FORMAT);
	Save::ImageParams params(width, height, dstBpp);
	Save::ImageWriter writer(params, FILE_NAME_PATTERN);
	writer << pCreate->GetData();

	std::cout << writer.GetLastFileName() << "\n";

	// clean up
	Arena::ImageFactory::Destroy(pCreate);
	delete[] pOutput;
	pDevice->RequeueBuffer(pImage);
	pDevice->StopStream();

	// return nodes to their initial values
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat", pixelFormatInitial);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Polarization_DolpAolp\n";

	try
	{
		// prepare example
		Arena::ISystem* pSystem = Arena::OpenSystem();
		pSystem->UpdateDevices(SYSTEM_TIMEOUT);
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
		ConvertDoLPAoLPToPixelFormat(pDevice);
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
