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

// Polarization, Color DoLP AoLP
//    This example introduces the basics of working with polarization pixel
//    formats. Specifically, this examples retrieves a 2-channel DoLP, AoLP
//    image, PolarizedDolpAolp_BayerRG8, splits the bayer tile data into into a
//    2x2 grid and converts it to a BGR image for saving, treating the AoLP data
//    as hue, and the DoLP data as saturation. This example shows only how to
//    convert the 8-bit-per-channel form of the pixel format. The
//    12p-bit-per-channel form, while similar, will not produce a correct result.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// name of file to save
#define FILE_NAME_PATTERN "Images/Cpp_Polarization_ColorDolpAolp.jpg"

// pixel format
#define PIXEL_FORMAT BGR8

// image timeout
#define IMAGE_TIMEOUT 2000

// system timeout
#define SYSTEM_TIMEOUT 100

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// splits the bayer tile data into into a 2x2 grid
void SplitTiles(const uint8_t* pSrc, size_t srcDoubleBytesPerPixel, size_t srcStride, size_t srcW, size_t srcH, uint8_t* pDst, size_t dstBytesPerPixel, size_t dstHalfStride)
{
	for (size_t i = 0; i < srcH; i += 2)
	{
		for (size_t j = 0; j < srcW; j += 2)
		{
			*pDst = *pSrc;

			*(pDst + 1) = *(pSrc + 1);

			pSrc += srcDoubleBytesPerPixel;

			pDst += dstBytesPerPixel;
		}

		pSrc += srcStride;

		pDst += dstHalfStride;
	}
}

// demonstrates acquisition
// (1) acquires image
// (2) splits bayer tile data into into 2x2 grid
// (3) manually converts to HSV
// (4) treats AoLP as hue
// (5) treats DoLP as saturation
// (6) converts HSV to displayable pixel format
// (7) saves to disk
void ConvertDoLPAoLPToPixelFormat(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	GenICam::gcstring pixelFormatInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat");

	// Change to DoLP AoLP pixel format
	//    DoLP AoLP pixel formats are a 2-channel image. The first channel holds
	//    DoLP (Degree of Linear Polarization) data while the second holds AoLP
	//    (Angle of Linear Polarization) data.
	std::cout << TAB1 << "Set PolarizedDolpAolp_BayerRG8 to pixel format\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"PixelFormat",
		"PolarizedDolpAolp_BayerRG8");

	// retrieve image
	std::cout << TAB1 << "Acquire image\n";

	pDevice->StartStream();
	Arena::IImage* pImage = pDevice->GetImage(IMAGE_TIMEOUT);

	// src info
	uint64_t srcPF = pImage->GetPixelFormat();
	size_t srcWidth = pImage->GetWidth();
	size_t srcHeight = pImage->GetHeight();
	size_t srcBitsPerPixel = Arena::GetBitsPerPixel(srcPF);
	size_t srcBytesPerPixel = srcBitsPerPixel / 8;
	size_t srcDoubleBytesPerPixel = srcBytesPerPixel * 2;
	size_t srcStride = srcWidth * srcBitsPerPixel / 8;

	const uint8_t* pSrc = pImage->GetData();

	// dst info
	uint64_t dstPF = srcPF;
	size_t dstWidth = srcWidth;
	size_t dstHeight = srcHeight;
	size_t dstBitsPerPixel = Arena::GetBitsPerPixel(dstPF);
	size_t dstBytesPerPixel = dstBitsPerPixel / 8;
	size_t dstStride = dstWidth * dstBitsPerPixel / 8;
	size_t dstHalfStride = dstStride / 2;
	size_t dstDataSize = dstWidth * dstHeight * dstBitsPerPixel / 8;
	size_t dstHalfDataSize = dstDataSize / 2;

	uint8_t* pDst = new uint8_t[dstDataSize];

	// reference to starting position of source image to read from
	const uint8_t* pSrcTopLeft = pSrc;
	const uint8_t* pSrcTopRight = pSrc + srcBytesPerPixel;
	const uint8_t* pSrcBottomLeft = pSrc + srcStride;
	const uint8_t* pSrcBottomRight = pSrc + srcStride + srcBytesPerPixel;

	// reference to starting position of each quadrant of destination 2x2 grid to
	// write to
	uint8_t* pDstTopLeft = pDst;
	uint8_t* pDstTopRight = pDst + dstHalfStride;
	uint8_t* pDstBottomLeft = pDst + dstHalfDataSize;
	uint8_t* pDstBottomRight = pDst + dstHalfDataSize + dstHalfStride;

	// split bayer tile data into 2x2 grid
	std::cout << TAB1 << "Splitting bayer tile data into 2x2 grid\n";

	SplitTiles(pSrcTopLeft, srcDoubleBytesPerPixel, srcStride, srcWidth, srcHeight, pDstTopLeft, dstBytesPerPixel, dstHalfStride);
	SplitTiles(pSrcTopRight, srcDoubleBytesPerPixel, srcStride, srcWidth, srcHeight, pDstTopRight, dstBytesPerPixel, dstHalfStride);
	SplitTiles(pSrcBottomLeft, srcDoubleBytesPerPixel, srcStride, srcWidth, srcHeight, pDstBottomLeft, dstBytesPerPixel, dstHalfStride);
	SplitTiles(pSrcBottomRight, srcDoubleBytesPerPixel, srcStride, srcWidth, srcHeight, pDstBottomRight, dstBytesPerPixel, dstHalfStride);

	// create image with new data
	Arena::IImage* pCreate = Arena::ImageFactory::Create(pDst, dstDataSize, dstWidth, dstHeight, dstPF);

	// Manually convert to HSV image
	//     Treat the AoLP data as hue and the DoLP data as saturation, and manually
	//     convert from HSV to the desired pixel format: algorithm available on Wikipedia:
	//     https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV
	size_t width = pCreate->GetWidth();
	size_t height = pCreate->GetHeight();
	size_t size = width * height;
	size_t inBitsPerPixel = pCreate->GetBitsPerPixel();
	size_t inPixelSize = inBitsPerPixel / 8;
	uint64_t inPixelFormat = pCreate->GetPixelFormat();
	const uint8_t* pInput = pCreate->GetData();

	// prepare memory output buffer
	size_t outBitsPerPixel = Arena::GetBitsPerPixel(PIXEL_FORMAT);
	size_t outPixelSize = outBitsPerPixel / 8;
	size_t outDataSize = width * height * outBitsPerPixel / 8;
	uint8_t* pOutput = new uint8_t[outDataSize];
	memset(pOutput, 0, outDataSize);

	// Convert 2-channel 8-bits-per-channel DoLP AoLP
	//    The first channel of each pixel holds the DoLP data and has a maximum
	//    value of 255; the second channel of each pixel holds the AoLP data and
	//    has a maximum value of 201.
	std::cout << TAB1 << "Using AoLP as hue and DoLP as saturation, convert from HSV to " << PIXEL_FORMAT << "\n";

	if (inPixelFormat == LUCID_PolarizedDolpAolp_BayerRG8)
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

			pIn += inPixelSize;
			pOut += outPixelSize;
		}
	}
	else
	{
		throw GenICam::GenericException("This example requires PolarizedDolpAolp_BayerRG8 pixel format", __FILE__, __LINE__);
	}

	// create image from buffer and save
	std::cout << TAB1 << "Save image to ";

	Arena::IImage* pHSVImage = Arena::ImageFactory::Create(pOutput, outDataSize, width, height, PIXEL_FORMAT);
	Save::ImageParams params(width, height, outBitsPerPixel);
	Save::ImageWriter writer(params, FILE_NAME_PATTERN);
	writer << pHSVImage->GetData();

	std::cout << writer.GetLastFileName() << "\n";

	// clean up
	Arena::ImageFactory::Destroy(pCreate);
	Arena::ImageFactory::Destroy(pHSVImage);
	delete[] pDst;
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

	std::cout << "Cpp_Polarization_ColorDolpAolp\n";

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
