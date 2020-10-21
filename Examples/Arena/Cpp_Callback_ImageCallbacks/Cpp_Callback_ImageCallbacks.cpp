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
#include <vector>
#include <thread>
#include <iomanip> // For setw, setfill

#if defined(__linux__)
#include <unistd.h>
#endif

#define TAB1 "  "
#define TAB2 "    "

// Callback: Image Callbacks
//    This example demonstrates configuring an image callback for a device. Once
//    a callback is registered and the device is streaming, the user-implemented
//    OnImage() function will be called. OnImage will receive a pointer to the
//    image and a pointer to an optional user-specified callback data. OnImage
//    will display the frame id and timestamp of the image before returning.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// number of seconds to stream for
#define STREAM_TIME_SEC 5

#if defined (_WIN32)
#define portable_sleep(x) Sleep(x * 1000)
#elif defined (__linux__)
#define portable_sleep(x) sleep(x)
#endif

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-


// To use callbacks, derive a callback object from Arena::IImageCallback
// The Arena library will call the user-implemented OnImage() when an image
// has been receieved.
class ImageCallback : public Arena::IImageCallback
{
public:
	ImageCallback(std::string serialNumber) : m_serialNumber(serialNumber), m_imageCounter(0) {};

	~ImageCallback() {}

	void OnImage(Arena::IImage* pImage)
	{
		static uint64_t lastTimestamp = 0;

		uint64_t currTimestamp = pImage->GetTimestamp();

		double diffMilliseconds = 0.0;

		if (lastTimestamp != 0)
		{
			diffMilliseconds = (currTimestamp - lastTimestamp) / 1000000.0;
		}

		std::cout << "Serial: [" << m_serialNumber << "], Image#: [" << std::setw(4) << std::setfill('0') << ++m_imageCounter << "], FrameId: [" << std::setw(4) << std::setfill('0') << pImage->GetFrameId() << "], TimeStamp: [" << currTimestamp << "], Diff: [" << diffMilliseconds << " ms]\n";

		lastTimestamp = currTimestamp;
	}

private:
	std::string m_serialNumber;
	uint64_t m_imageCounter;
};

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Callback_ImageCallbacks\n";

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

		std::cout << "Using camera with serial number: " << deviceInfos[0].SerialNumber() << std::endl;

		// Allocate the image callback handler object
		ImageCallback* pCallbackHandler = new ImageCallback(std::string(deviceInfos[0].SerialNumber()));

		// Register the callback handler to the device
		pDevice->RegisterImageCallback(pCallbackHandler);

		pDevice->StartStream();
		portable_sleep(STREAM_TIME_SEC);
		pDevice->StopStream();

		// Deregister the callback handler
		pDevice->DeregisterImageCallback(pCallbackHandler);

		// Free the callback handler object
		delete pCallbackHandler;

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
