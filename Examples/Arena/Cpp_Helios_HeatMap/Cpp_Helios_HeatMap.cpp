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

// Helios: Heat Map
//    This example demonstrates saving an RGB heatmap of a 3D image. It captures
//    a 3D image, interprets the ABCY data to retrieve the distance value for each
//    pixel and then converts this data into a BGR and an RGB buffer. The BGR buffer
//    is used to create a jpg heatmap image and the RGB buffer is used to color the
//    ply image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// file name
#define PLY_FILE_NAME "Images/Cpp_Helios_HeatMap.ply"
#define JPG_FILE_NAME "Images/Cpp_Helios_HeatMap.jpg"

// pixel format
#define PIXEL_FORMAT BGR8

// image timeout
#define IMAGE_TIMEOUT 2000

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates saving a bgr heatmap image
// (1) gets image
// (2) interprets ABCY data to get z coordinate
// (3) creates a buffer for BGR and RGB colorings using z data
// (4) creates jpg heatmap image using BGR buffer
// (5) colors ply image using RGB buffer
// (6) saves jpg and ply image
void AcquireImageAndCreateHeatMapColoring(Arena::IDevice* pDevice)
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
	GenICam::gcstring pixelFormatInitial = Arena::GetNodeValue<GenICam::gcstring>(pNodeMap, "PixelFormat");
	GenICam::gcstring operatingModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pNodeMap, "Scan3dOperatingMode");

	// set pixel format
	std::cout << TAB1 << "Set Coord3D_ABCY16s to pixel format\n";

	Arena::SetNodeValue<GenICam::gcstring>(pNodeMap, "PixelFormat", "Coord3D_ABCY16s");

	// set operating mode distance
	std::cout << TAB1 << "Set 3D operating mode to Distance1500mm\n";

	Arena::SetNodeValue<GenICam::gcstring>(pNodeMap, "Scan3dOperatingMode", "Distance1500mm");

	// get the z coordinate scale in order to convert z values to mm
	std::cout << TAB1 << "Get z coordinate scale\n\n";

	Arena::SetNodeValue<GenICam::gcstring>(pNodeMap, "Scan3dCoordinateSelector", "CoordinateC");

	// getting scale as float by casting since SetPly() will expect it passed as float
	float scale = static_cast<float>(Arena::GetNodeValue<double>(pNodeMap, "Scan3dCoordinateScale"));

	// retrieve image
	std::cout << TAB2 << "Acquire image\n";

	pDevice->StartStream();
	Arena::IImage* pImage = pDevice->GetImage(IMAGE_TIMEOUT);

	// prepare info from input buffer
	size_t width = pImage->GetWidth();
	size_t height = pImage->GetHeight();
	size_t size = width * height;
	size_t srcBpp = pImage->GetBitsPerPixel();
	size_t srcPixelSize = srcBpp / 8; // divide by the number of bits in a byte
	const uint8_t* pInput = pImage->GetData();

	// prepare memory output buffer
	size_t dstBpp = Arena::GetBitsPerPixel(PIXEL_FORMAT);
	size_t dstPixelSize = dstBpp / 8;				  // divide by the number of bits in a byte
	size_t dstDataSize = width * height * dstBpp / 8; // divide by the number of bits in a byte
	uint8_t* pOutput = new uint8_t[dstDataSize];
	memset(pOutput, 0, dstDataSize);

	// Prepare coloring buffer for ply image
	//    Saving ply with color takes RGB coloring compared to the BGR coloring
	//    the jpg image uses, therefore we need a separate buffer for this data.
	uint8_t* pColoring = new uint8_t[dstDataSize];
	memset(pColoring, 0, dstDataSize);
	uint8_t* pColor = pColoring;

	// manually convert to BGR image

	const uint8_t* pIn = pInput;
	uint8_t* pOut = pOutput;

	const double RGBmin = 0;
	const double RGBmax = 255;

	const double redColorBorder = 0;
	const double yellowColorBorder = 375;
	const double greenColorBorder = 750;
	const double cyanColorBorder = 1125;
	const double blueColorBorder = 1500;

	// iterate through each pixel and assign a color to it according to a distance
	for (size_t i = 0; i < size; i++)
	{
		// Isolate the z data
		//    The first channel is the x coordinate, second channel is the y coordinate,
		//    the third channel is the z coordinate (which is what we will use to determine
		//    the coloring) and the fourth channel is intensity.
		int16_t z = *reinterpret_cast<const int16_t*>((pIn + 4));

		// Convert z to millimeters
		//    The z data converts at a specified ratio to mm, so by multiplying it by the
		//    Scan3dCoordinateScale for CoordinateC, we are able to convert it to mm and
		//    can then compare it to the maximum distance of 1500mm.
		z = int16_t(double(z) * scale);

		double coordinateColorBlue = 0.0;
		double coordinateColorGreen = 0.0;
		double coordinateColorRed = 0.0;

		// colors between red and yellow
		if ((z >= redColorBorder) && (z <= yellowColorBorder))
		{
			double yellowColorPercentage = z / yellowColorBorder;

			coordinateColorBlue = RGBmin;
			coordinateColorGreen = RGBmax * yellowColorPercentage;
			coordinateColorRed = RGBmax;
		}

		// colors between yellow and green
		else if ((z > yellowColorBorder) && (z <= greenColorBorder))
		{
			double greenColorPercentage = (z - yellowColorBorder) / yellowColorBorder;

			coordinateColorBlue = RGBmin;
			coordinateColorGreen = RGBmax;
			coordinateColorRed = RGBmax - RGBmax * greenColorPercentage;
		}

		// colors between green and cyan
		else if ((z > greenColorBorder) && (z <= cyanColorBorder))
		{
			double cyanColorPercentage = (z - greenColorBorder) / yellowColorBorder;

			coordinateColorBlue = RGBmax * cyanColorPercentage;
			coordinateColorGreen = RGBmax;
			coordinateColorRed = RGBmin;
		}

		// colors between cyan and blue
		else if ((z > cyanColorBorder) && (z <= blueColorBorder))
		{
			double blueColorPercentage = (z - cyanColorBorder) / yellowColorBorder;

			coordinateColorBlue = RGBmax;
			coordinateColorGreen = RGBmax - RGBmax * blueColorPercentage;
			coordinateColorRed = RGBmin;
		}
		else
		{
			coordinateColorBlue = RGBmin;
			coordinateColorGreen = RGBmin;
			coordinateColorRed = RGBmin;
		}

		// set pixel format values and move to next pixel
		*pOut = static_cast<int8_t>(coordinateColorBlue);
		*(pOut + 1) = static_cast<int8_t>(coordinateColorGreen);
		*(pOut + 2) = static_cast<int8_t>(coordinateColorRed);

		pIn += srcPixelSize;
		pOut += dstPixelSize;

		// set RGB pixel coloring for ply
		*pColor = static_cast<int8_t>(coordinateColorRed);
		*(pColor + 1) = static_cast<int8_t>(coordinateColorGreen);
		*(pColor + 2) = static_cast<int8_t>(coordinateColorBlue);
		pColor += dstPixelSize;
	}

	// create jpg image from buffer and save
	std::cout << TAB2 << "Create BGR heatmap using z data from 3D image\n";

	Arena::IImage* pCreate = Arena::ImageFactory::Create(pOutput, dstDataSize, width, height, PIXEL_FORMAT);
	Save::ImageParams jpgParams(width, height, dstBpp);
	Save::ImageWriter jpgWriter(jpgParams, JPG_FILE_NAME);
	jpgWriter << pCreate->GetData();

	std::cout << TAB2 << "Save heatmap image as jpg to " << jpgWriter.GetLastFileName() << "\n";

	// prepare image parameters and writer for ply
	Save::ImageParams plyParams(
		pImage->GetWidth(),
		pImage->GetHeight(),
		pImage->GetBitsPerPixel());

	Save::ImageWriter plyWriter(
		plyParams,
		PLY_FILE_NAME);

	// set default parameters for SetPly()
	bool filterPoints = true;		 // default
	bool isSignedPixelFormat = true; // the example use Coord3D_ABCY16s
	float offsetA = 0.0f;			 // default
	float offsetB = 0.0f;			 // default
	float offsetC = 0.0f;			 // default

	// set the output file format of the image writer to .ply
	plyWriter.SetPly(".ply", filterPoints, isSignedPixelFormat, scale, offsetA, offsetB, offsetC);

	// save image
	plyWriter.Save(pImage->GetData(), pColoring, true);

	std::cout << TAB2 << "Save 3D image as ply to " << plyWriter.GetLastFileName() << "\n\n";

	// clean up
	Arena::ImageFactory::Destroy(pCreate);
	delete[] pOutput;
	pDevice->RequeueBuffer(pImage);
	pDevice->StopStream();

	// return nodes to their initial values
	Arena::SetNodeValue<GenICam::gcstring>(pNodeMap, "Scan3dOperatingMode", operatingModeInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pNodeMap, "PixelFormat", pixelFormatInitial);
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

	std::cout << "Cpp_Helios_HeatMap\n";

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
		AcquireImageAndCreateHeatMapColoring(pDevice);

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
