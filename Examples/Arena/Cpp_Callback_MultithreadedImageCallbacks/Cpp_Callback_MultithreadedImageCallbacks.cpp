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

#define TAB1 "  "
#define TAB2 "    "

// Callback: Multithreaded Image Callbacks
//    This example demonstrates configuring a callback within a thread. Once the
//    thread has been launched, each new image is acquired and the callback is
//    triggered to retrieve the image's frame ID. Once the callback function
//    exits, the image buffer is requeued. After all images have been acquired,
//    the thread exits and memory is cleaned up.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// image timeout
#define TIMEOUT 2000

// number of images to grab
#define NUM_IMAGES 25

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// callback interface
class IImageCallback
{
public:
	// defines callback behavior
	virtual void OnImage(Arena::IImage* pImage) = 0;

	// defines exit condition
	virtual bool ContinueAcquiring() = 0;
};

// callback implementation
class MyImageCallback : public IImageCallback
{
public:
	// constructor
	MyImageCallback(int numImages) :
		k_numImages(numImages),
		m_count(0)
	{
	}

	virtual ~MyImageCallback() {};

	// implements callback
	virtual void OnImage(Arena::IImage* pImage)
	{
		std::cout << TAB2 << "Image callback triggered (frame ID " << pImage->GetFrameId() << ")\n";
		m_count++;
	}

	// implements exit condition
	virtual bool ContinueAcquiring()
	{
		return m_count < k_numImages;
	}

private:
	const int k_numImages;
	int m_count;
};

// callback dispatcher
class Dispatcher
{
public:
	// registers callbacks
	void Register(IImageCallback* pCallback)
	{
		m_callbacks.push_back(pCallback);
	}

	// triggers callbacks
	virtual void OnImage(Arena::IImage* pImage)
	{
		for (auto pCallback : m_callbacks)
			pCallback->OnImage(pImage);
	}

	// checks on exit condition
	virtual bool ContinueAcquiring()
	{
		for (auto pCallback : m_callbacks)
		{
			bool continueAcquiring = pCallback->ContinueAcquiring();
			if (!continueAcquiring)
				return false;
		}

		return true;
	}

private:
	std::vector<IImageCallback*> m_callbacks;
};

// acquisition thread
// (1) starts stream
// (2) gets image
// (3) invokes callback
// (4) requeues image buffer
// (5) stops stream and exits thread
void AcquisitionThread(Arena::IDevice* pDevice, Dispatcher& dispatcher)
{
	// start stream upon entering acquisition thread
	std::cout << TAB1 << "Enter acquisition thread\n";

	pDevice->StartStream();

	// acquire images
	while (dispatcher.ContinueAcquiring())
	{
		// get image
		std::cout << TAB2 << "Image retrieved\n";

		Arena::IImage* pImage = pDevice->GetImage(TIMEOUT);

		// Trigger callback
		dispatcher.OnImage(pImage);

		// requeue image
		pDevice->RequeueBuffer(pImage);
	};

	// stop stream before exiting acquisition thread
	std::cout << TAB1 << "Exit acquisition thread\n";

	pDevice->StopStream();
}

// main thread
// (1) prepares callback and dispatcher
// (2) launches acquisition thread
// (3) cleans up threading and memory
void MainThread(Arena::IDevice* pDevice)
{
	// Prepare callback
	MyImageCallback* pImageCallback = new MyImageCallback(NUM_IMAGES);

	// Prepare dispatcher
	Dispatcher dispatcher;
	dispatcher.Register(pImageCallback);

	// Launch acquisition thread
	std::thread acquisitionThread = std::thread(AcquisitionThread, pDevice, std::ref(dispatcher));

	// clean up threading/memory
	acquisitionThread.join();
	delete pImageCallback;
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Callback_MultithreadedImageCallbacks\n";

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
		MainThread(pDevice);
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
