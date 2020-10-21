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
#define TAB3 "      "

// Sequencer HDR
//    This example demonstrates saving a set of images using the sequencer. It
//    will cycle through 3 sequencer states each one using a different exposure
//    time. The images could then be used to generate an HDR image. Much like a
//    state machine, in order to use the sequencer we must initialize each set
//    appropriately. Each set can have its own exposure time and gain, and will
//    contain information such as the sequencer starting position as well as
//    paths to other sets. A set can have multiple paths where each path has its
//    own next set, trigger source and trigger activation. In this example the
//    sequencer has 3 sets where set 0 goes to set 1, set 1 goes to set 2 and set
//    2 goes back to set 0, all being triggered on Frame Start.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Exposure time
//    Time that the sensor will be exposed when grabbing an image (in
//    microseconds). Exposure time depends on the application, but generally the
//    less available the light, the higher the exposure time. Storing multiple
//    values so that each consecutive state can be saved with varying exposure times.
double EXPOSURE_TIME_0 = 25000.0;
double EXPOSURE_TIME_1 = 50000.0;
double EXPOSURE_TIME_2 = 100000.0;

// timeout for grabbing an image (in milliseconds)
#define IMAGE_TIMEOUT 2000

// number of states in sequencer (<= max range of SequencerSetSelector feature)
#define NUM_SETS 3

// name of file to save
#define FILE_NAME_PATTERN "Images/Cpp_Sequencer_HDR<count>.jpg"

// pixel format
#define PIXEL_FORMAT BGR8

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// set sequencer set to desired values
void SetSequencerSet(GenApi::INodeMap* nodeMap, int64_t setNumber, double exposureTime, int64_t pathNextSet, GenICam::gcstring triggerSource)
{
	// set Sequencer Set Selector to sequence number
	Arena::SetNodeValue<int64_t>(
		nodeMap,
		"SequencerSetSelector",
		setNumber);

	std::cout << TAB2 << "Updating set " << setNumber << ":\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		nodeMap,
		"SequencerFeatureSelector",
		"ExposureTime");

	// set exposure time to the value we want it to save
	Arena::SetNodeValue<double>(
		nodeMap,
		"ExposureTime",
		exposureTime);

	std::cout << TAB3 << "Exposure time = " << exposureTime << "\n";

	// select the path we want it to follow from this set to next set, there can
	// be multiple paths so the first path will always be set to 0
	Arena::SetNodeValue<int64_t>(
		nodeMap,
		"SequencerPathSelector",
		0);

	std::cout << TAB3 << "Path[0] = ";

	// set next state in the sequence, ensure it does not exceed the maximum
	Arena::SetNodeValue<int64_t>(
		nodeMap,
		"SequencerSetNext",
		pathNextSet);

	std::cout << pathNextSet << "\n";

	// set Sequencer Trigger Source to Frame Start
	Arena::SetNodeValue<GenICam::gcstring>(
		nodeMap,
		"SequencerTriggerSource",
		triggerSource);

	std::cout << TAB3 << "Trigger source = " + triggerSource + "\n";

	// Save current state
	//    Once all appropriate settings have been configured, make sure to save
	//    the state to the sequence. Notice that these settings will be lost when
	//    the camera is power-cycled.
	std::cout << TAB3 << "Save sequence set\n";

	GenApi::CCommandPtr pSequencerSetSave = nodeMap->GetNode("SequencerSetSave");
	pSequencerSetSave->Execute();
}

// start streaming, acquire and save images
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

	// prepare image parameters from device settings
	Save::ImageParams params(
		static_cast<size_t>(pWidth->GetValue()),
		static_cast<size_t>(pHeight->GetValue()),
		Arena::GetBitsPerPixel(pPixelFormat->GetCurrentEntry()->GetValue()));

	// prepare image writer
	Save::ImageWriter writer(
		params,
		FILE_NAME_PATTERN);

	// start stream
	std::cout << TAB2 << "Start streaming\n";

	pDevice->StartStream();

	// get an image in each set of sequencer
	std::cout << TAB2 << "Getting " << NUM_SETS << " images\n";

	for (int i = 0; i < NUM_SETS; i++)
	{
		std::cout << TAB3 << "Converting and saving image " << i;

		// get image
		Arena::IImage* pImage = pDevice->GetImage(IMAGE_TIMEOUT);

		// convert the image to a displayable pixel format.
		Arena::IImage* pConverted = Arena::ImageFactory::Convert(
			pImage,
			PIXEL_FORMAT);

		// save image
		writer << pConverted->GetData();

		// get last file name
		std::cout << " at " << writer.GetLastFileName(true) << "\n";

		// destroy converted image
		Arena::ImageFactory::Destroy(pConverted);

		// requeue image buffer
		pDevice->RequeueBuffer(pImage);
	}

	// stop stream
	std::cout << TAB2 << "Stop streaming\n";

	pDevice->StopStream();
}

void AcquireImagesUsingSequencer(Arena::IDevice* pDevice)
{
	// get node values that will be changed in order to return their values at
	// the end of the example
	GenICam::gcstring exposureAutoInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureAuto");
	GenICam::gcstring gainAutoInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "GainAuto");
	double exposureTimeInitial = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "ExposureTime");

	// get max and min exposure time to ensure set of exposure times are within this range
	GenApi::CFloatPtr pFloat = pDevice->GetNodeMap()->GetNode("ExposureTime");
	double exposureTimeMax = pFloat->GetMax();
	double exposureTimeMin = pFloat->GetMin();

	// if largest exposure times is not within the exposure time range, set largest exposure
	// time to max value and set the remaining exposure times to half the value of the
	// state before
	if (EXPOSURE_TIME_2 > exposureTimeMax || EXPOSURE_TIME_2 < exposureTimeMin)
	{
		EXPOSURE_TIME_2 = exposureTimeMax;
		EXPOSURE_TIME_1 = EXPOSURE_TIME_2 / 2;
		EXPOSURE_TIME_0 = EXPOSURE_TIME_1 / 2;
	}

	// Disable automatic exposure and gain
	//    Disable automatic exposure  and gain before setting an exposure time.
	//    Automatic exposure and gain controls whether they are set manually or
	//    automatically by the device. Setting automatic exposure and gain to
	//    'Off' stops the device from automatically updating the exposure time
	//    while streaming.
	std::cout << TAB1 << "Disable automatic exposure and gain\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"ExposureAuto",
		"Off");

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"GainAuto",
		"Off");

	// if sequencer mode is on, turn it off
	if (Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "SequencerMode") == "On")
	{
		std::cout << TAB1 << "Turn sequencer mode off\n";

		Arena::SetNodeValue<GenICam::gcstring>(
			pDevice->GetNodeMap(),
			"SequencerMode",
			"Off");
	}

	// Put sequencer in configuration mode
	//    Sequencer configuration mode must be on while making changes to the
	//    sequencer sets.
	std::cout << TAB1 << "Turn sequencer configuration mode on\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"SequencerConfigurationMode",
		"On");

	// set up sequencer sets
	std::cout << TAB1 << "Set up sequencer sets\n";

	SetSequencerSet(pDevice->GetNodeMap(), 0, EXPOSURE_TIME_0, 1, "FrameStart");
	SetSequencerSet(pDevice->GetNodeMap(), 1, EXPOSURE_TIME_1, 2, "FrameStart");
	SetSequencerSet(pDevice->GetNodeMap(), 2, EXPOSURE_TIME_2, 0, "FrameStart");

	// sets the sequencer starting set to be set 0
	Arena::SetNodeValue<int64_t>(
		pDevice->GetNodeMap(),
		"SequencerSetStart",
		0);

	// turn off configuration mode
	std::cout << TAB1 << "Turn sequencer configuration mode off\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"SequencerConfigurationMode",
		"Off");

	// Turn on sequencer
	//    When sequencer mode is on and the device is streaming it will follow
	//    the sequencer sets according to their saved settings.
	std::cout << TAB1 << "Turn sequencer mode on\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"SequencerMode",
		"On");

	// Acquire and Save images
	//    This function will start the stream, acquire an image in each set of
	//    the sequencer using its corresponding settings, save each image and
	//    then stop the stream.
	AcquireAndSaveImages(pDevice);

	// turn off sequencer mode
	std::cout << TAB1 << "Turn sequencer mode off\n";

	Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"SequencerMode",
		"Off");

	// return nodes to their initial values
	Arena::SetNodeValue<double>(pDevice->GetNodeMap(), "ExposureTime", exposureTimeInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "GainAuto", gainAutoInitial);
	Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "ExposureAuto", exposureAutoInitial);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Sequencer_HDR\n";

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
		AcquireImagesUsingSequencer(pDevice);
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
