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

#pragma once

#include <functional>

#include "IImage.h"
#include "ArenaDefs.h"


namespace Arena
{
	class IImageCallback
	{
	public:
		virtual ~IImageCallback() {};
		virtual void OnImage(Arena::IImage* pImage) = 0;

	protected:
		IImageCallback() {};
	};

	/**
	 * @class IDevice
	 *
	 * An interface to a device object
	 *
	 * Devices constitute the core of the Arena SDK, providing the means to
	 * interacting with physical devices. They are created and destroyed via the
	 * system (Arena::ISystem).
	 *
	 * \code{.cpp}
	 * 	// creating and destroying a device
	 * 	{
	 * 		std::vector<Arena::DeviceInfo> devInfos = pSystem->GetDevices();
	 * 		Arena::IDevice* pDevice = pSystem->CreateDevice(devInfos[0]);
	 * 		// ...
	 * 		pSystem->DestroyDevice(pDevice);
	 * 	}
	 * \endcode
	 *
	 * A device manages its images and chunk data (Arena::IBuffer, Arena::IImage,
	 * Arena::IChunkData), events, and node maps (GenApi::INodeMap) by:
	 *  - starting and stopping the stream (Arena::IDevice::StartStream,
	 *    Arena::IDevice::StopStream),
	 *  - retrieving and requeuing images and chunk data
	 *    (Arena::IDevice::GetBuffer, Arena::IDevice::GetImage,
	 * Arena::IDevice::RequeueBuffer),
	 *  - handling events (Arena::IDevice::InitializeEvents,
	 *    Arena::IDevice::DeinitializeEvents, Arena::IDevice::WaitOnEvent),
	 *  - and providing access to its node maps (Arena::IDevice::GetNodemap,
	 *    Arena::IDevice::GetTLDeviceNodeMap,
	 *    Arena::IDevice::GetTLInterfaceNodeMap,
	 *    Arena::IDevice::GetTLStreamNodeMap)
	 *
	 * @warning 
	 *  - Must be destroyed; otherwise, memory will leak
	 *
	 * @see 
	 *  - Arena::ISystem
	 *  - Arena::DeviceInfo
	 *  - Arena::IDevice
	 *  - Arena::IBuffer
	 *  - Arena::IImage
	 *  - Arena::IChunkData
	 */
	class ARENA_API IDevice
	{
	public:

		/**
		 * @fn virtual bool IsConnected()
		 *
		 *  - Type: bool
		 *  - Indicates if device is connected
		 *
		 * <B> IsConnected </B> returns true if a device has been opened and
		 * maintains a valid communication socket. The device is opened when
		 * (Arena::ISystem::CreateDevice) is called. If the connection to the device
		 * is lost this will return false.
		 *
		 * More specifically, for GigE devices, this flag is set to false when
		 * the Arena is not able to refresh the heartbeat on the device. If an
		 * operation times out more than 3 times the device will be flagged as
		 * not connected.
		 *
		 * @see 
		 *  - Arena::ISystem::CreateDevice
		 */
		virtual bool IsConnected() = 0;

		/**
		 * @fn virtual void StartStream(size_t numBuffers = 10)
		 *
		 * @param numBuffers
		 *  - Type: size_t
		 *  - Default: 10
		 *  - Number of internal buffers to use in the acquisition engine
		 *
		 * @return 
		 *  - none
		 *
		 * <B> StartStream </B> causes the device to begin streaming image/chunk
		 * data
		 * (Arena::IBuffer, Arena::IImage, Arena::IChunkData). It must be called
		 * before image or chunk data buffers are retrieved
		 * (Arena::IDevice::GetBuffer, Arena::IDevice::GetImage). The stream must be
		 * stopped (Arena::IDevice::StopStream) when no longer needed.
		 *
		 * Basically, calling <B> StartStream </B> prepares and starts the
		 * underlying streaming engine. The streaming engine primarily consists
		 * of a number of buffers, an input and an output queue, and a worker
		 * thread to run off of the main thread. All buffers are first placed in
		 * the input queue. When a buffer reaches its turn, it is filled with
		 * data. Once complete, it is moved to the output queue. At this point a
		 * buffer might be retrieved by the user (Arena::IDevice::GetBuffer,
		 * Arena::IDevice::GetImage) and then returned to the input queue
		 * (Arena::IDevice::RequeueBuffer).
		 *
		 * More specifically, <B> StartStream </B>:
		 *  - allocates and announces a number of buffers according to the
		 *    numBuffers parameter
		 *  - pushes all buffers to the input queue
		 *  - opens a stream channel socket
		 *  - configures the IP and port on the device
		 *  - fires a dummy packet to help with firewalls
		 *  - requests a test packet to ensure configured packet size is
		 *    appropriate
		 *  - starts the worker thread and begins listening for packets related
		 *    to the acquisition engine
		 *  - has the device lock out certain features (e.g. 'Width', 'Height')
		 *    that cannot be changed during the stream
		 *  - executes the 'AcquisitionStart' feature in order to have the
		 *    device start sending packets
		 *
		 * \code{.cpp}
		 * 	// setting width and height to max before starting the stream
		 * 	// because these features are locked out while streaming
		 * 	{
		 * 		GenApi::INodeMap* pNodeMap = pDevice->GetNodeMap();
		 * 		
		 * 		GenApi::CIntegerPtr pWidth = pNodeMap->GetNode("Width");
		 * 		pWidth->SetValue(pWidth->GetMax());
		 * 		
		 * 		GenApi::CIntegerPtr pHeight = pNodeMap->GetNode("Height");
		 * 		pHeight->SetValue(pHeight->GetMax());
		 * 		
		 * 		pDevice->StartStream();
		 * 	}
		 * \endcode
		 *
		 * All stream configurations must be completed before starting the
		 * stream. This includes, among other things, the buffer handling mode
		 * ('StreamBufferHandlingMode') found on the stream node map
		 * (Arena::IDevice::GetTLStreamNodeMap). Setting the buffer handling mode
		 * configures what the streaming engine does with buffers as they are
		 * filled and moved between queues. There are three modes to choose from:
		 *  - 'OldestFirst' is the default buffer handling mode. As buffers are
		 *    filled with data, they get pushed to the back of the output queue.
		 *    When a buffer is requested (Arena::IDevice::GetBuffer,
		 *    Arena::IBuffer::GetImage), the buffer at the front of the queue is
		 *    returned. If there are no input buffers available, the next
		 *    incoming image is dropped and the lost frame count
		 *    ('StreamLostFrameCount') is incremented.
		 *  - 'OldestFirstOverwrite' is similar to 'OldestFirst' except for
		 *    what happens when there are no input buffers. Instead of dropping
		 *    an image, the oldest buffer in the output queue gets returned to
		 *    the input queue so that its data can be overwritten.
		 *  - 'NewestOnly' only ever has a single buffer in the output queue.
		 *    If a second buffer gets placed into the output queue, the older
		 *    buffer gets returned to the back of the input queue. If there are
		 *    no input buffers available, the next image is dropped and the lost
		 *    frame count
		 *    ('StreamLostFrameCount') is incremented.
		 *
		 * \code{.cpp}
		 * 	// setting the buffer handling mode to 'NewestOnly' before starting the stream
		 * 	{
		 * 		GenApi::INodeMap* pStreamNodeMap = pDevice->GetTLStreamNodeMap();
		 * 		
		 * 		GenApi::CEnumerationPtr pStreamBufferHandlingMode = pStreamNodeMap->GetNode("StreamBufferHandlingMode");
		 * 		GenApi::CEnumEntryPtr pNewestOnly = pStreamBufferHandlingMode->GetNode("NewestOnly");
		 * 		pStreamBufferHandlingMode->SetIntValue(pNewOnly->GetValue());
		 * 		
		 * 		pDevice->StartStream()
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - Stream must already be configured prior to call
		 *  - Updates write access to certain nodes
		 *  - May only be called once per stream without stopping
		 *  - Stream must be stopped
		 *  - May throw GenICam::GenericException or other derived exception
		 *  - Minimum number of buffers is 1
		 *
		 * @see 
		 *  - Arena::IBuffer
		 *  - Arena::IImage
		 *  - Arena::IChunkData
		 *  - Arena::IDevice::GetBuffer
		 *  - Arena::IDevice::GetImage
		 *  - Arena::IDevice::StopStream
		 *  - Arena::IDevice::RequeueBuffer
		 *  - Arena::IDevice::GetTLStreamNodeMap
		 *  - Arena::IBuffer::GetImage
		 */
		virtual void StartStream(size_t numBuffers = 10) = 0;

		/**
		 * @fn virtual void StopStream()
		 *
		 * @return 
		 *  - none
		 *
		 * <B> StopStream </B> stops the device from streaming image/chunk data
		 * (Arena::IBuffer, Arena::IImage, Arena::IChunkData) and cleans up the
		 * stream. The stream must be stopped when streaming is no longer needed.
		 *
		 * <B> StopStream </B> reverses the set up of the stream:
		 *  - stops the worker thread
		 *  - shuts down the stream channel socket
		 *  - executes the 'AcquisitionStop' feature in order to stop the
		 *    device from sending packets
		 *  - has the device unlock features that had been locked for streaming
		 *    (e.g. 'Width', 'Height')
		 *  - revokes all buffers and cleans up their allocated memory
		 *
		 * Buffers used internally are allocated when the stream has started
		 * (Arena::IDevice::StartStream) and deallocated when it has stopped
		 * (Arena::IDevice::StopStream). If an image has been retrieved
		 * (Arena::IDevice::GetImage), it can be copied (Arena::ImageFactory::Copy)
		 * or saved before stopping the stream. If image data were accessed after
		 * stopping the stream, the memory would be deallocated and the behavior
		 * undefined.
		 *
		 * \code{.cpp}
		 * 	// retrieving an image and copying it before requeuing its buffer and stopping the stream
		 * 	{
		 * 		Arena::IImage* pImage = pDevice->GetImage();
		 * 		Arena::IImage* pCopiedImage = Arena::ImageFactory::Copy(pImage);
		 * 		
		 * 		pDevice->RequeueBuffer(pImage);
		 * 		pDevice->StopStream();
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - Stream must be stopped
		 *  - Updates write access to certain nodes
		 *  - Disallows retrieval of image/chunk data from device
		 *  - Deallocates image/chunk data that has not been copied to memory
		 *    or disk
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer
		 *  - Arena::IImage
		 *  - Arena::IChunkData
		 *  - Arena::IDevice::StartStream
		 *  - Arena::IDevice::StopStream
		 *  - Arena::IDevice::GetImage
		 *  - Arena::ImageFactory::Copy
		 */
		virtual void StopStream() = 0;

		/**
		 * @fn virtual IImage* GetImage(uint64_t timeout)
		 *
		 * @param timeout
		 *  - Type: uint64_t
		 *  - Unit: milliseconds
		 *  - Maximum time to wait for an image
		 *
		 * @return 
		 *  - Type:: Arena::IImage*
		 *  - Pointer to the next buffer in the output queue if it contains
		 *    image data
		 *  - Otherwise, null
		 *
		 * <B> GetImage </B> retrieves an image (Arena::IImage) from the device.
		 * It must be called after the stream has started
		 * (Arena::IDevice::StartStream) and before the stream has stopped
		 * (Arena::IDevice::StopStream). Retrieved images must be requeued
		 * (Arena::IDevice::RequeueBuffer).
		 *
		 * <B> GetImage </B> is essentially the same call as <B> GetBuffer </B>
		 * (Arena::IDevice::GetBuffer), just with an additional step. Internally, <B>
		 * GetImage </B> calls <B> GetBuffer </B>, which returns a buffer. It
		 * then attempts to cast the buffer to an image before returning it. If
		 * the data in the buffer is that of an image, the cast succeeds and the
		 * image is returned successfully. If not, the cast fails and NULL is
		 * returned.
		 *
		 * When called, <B> GetBuffer </B> checks the output queue for
		 * image/chunk data, grabbing the first buffer in the queue. If nothing
		 * is in the output queue, the call will wait until something arrives. If
		 * nothing arrives before expiration of the timeout, a
		 * GenICam::TimeoutException is thrown.
		 *
		 * <B> GetBuffer </B> is a blocking call. If it is called with a timeout
		 * of 20 seconds and nothing arrives in the output queue, then its thread
		 * will be blocked for the full 20 seconds. However, as the timeout is a
		 * maximum, as soon as something arrives in the output queue, it will be
		 * returned, not waiting for the full timeout. A timeout value of 0
		 * ensures the call will not block, throwing instead of waiting if
		 * nothing is in the output queue.
		 *
		 * <B> GetImage </B> does not verify image quality. If an incomplete
		 * image is returned, the call will not throw and the image will be
		 * returned as is. It is recommended to verify each image's completion
		 * (Arena::IImage::IsIncomplete).
		 *
		 * \code{.cpp}
		 * 	// retrieving an image, verifying its completion, and requeuing its buffer
		 * 	{
		 * 		Arena::IImage* pImage = pDevice->GetImage(100);
		 * 		if (pImage->IsIncomplete())
		 * 		{
		 * 			// handle error
		 * 			// ...
		 * 		}
		 * 		// do something
		 * 		// ...
		 * 		pDevice->RequeueBuffer(pImage);
		 * 	}
		 * \endcode
		 *
		 * Best practices recommends that buffers be requeued
		 * (Arena::IDevice::RequeueBuffer) as soon as they are no longer needed in
		 * order to prevent starvation of the acquisition engine. If image data
		 * is needed for a longer period of time (i.e. for processing), it is
		 * recommended to copy the data (Arena::ImageFactory::Create) and requeue
		 * the buffer.
		 *
		 * @warning 
		 *  - Does not guarantee valid image data
		 *  - Returns NULL for non-image formats
		 *  - Images should be requeued to prevent starvation of the
		 *    acquisition engine
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IImage
		 *  - Arena::IDevice::StartStream
		 *  - Arena::IDevice::StopStream
		 *  - Arena::IDevice::RequeueBuffer
		 *  - Arena::IDevice::GetBuffer
		 *  - Arena::IImage::IsIncomplete
		 *  - Arena::ImageFactory::Create
		 */
		virtual IImage* GetImage(uint64_t timeout) = 0;

		/**
		 * @fn virtual IBuffer* GetBuffer(uint64_t timeout)
		 *
		 * @param timeout
		 *  - Type: uint64_t
		 *  - Unit: milliseconds
		 *  - Maximum time to wait for a buffer
		 *
		 * @return 
		 *  - Type: Arena::IBuffer*
		 *  - Pointer to the next buffer in the output queue
		 *
		 * <B> GetBuffer </B> retrieves a buffer (Arena::IBuffer) from the
		 * device. It must be called after the stream has started
		 * (Arena::IDevice::StartStream) and before the stream has stopped
		 * (Arena::IDevice::StopStream). Retrieved images must be requeued
		 * (Arena::IDevice::RequeueBuffer).
		 *
		 * The data returned from <B> GetBuffer </B> represents different payload
		 * types: an image without chunk, an image with chunk, or just chunk
		 * data. Note that a buffer of chunk data payload type may contain image
		 * data, but cannot be cast to an image because the image data is treated
		 * as a chunk. The payload type can be queried
		 * (Arena::IBuffer::GetPayloadType), which returns an enum
		 * (Arena::EBufferPayloadType):
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImage
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImageExtendedChunk
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeChunkData
		 *
		 * <B> GetBuffer </B> is essentially the same call as <B> GetImage </B>,
		 * just more generic. Internally, <B> GetImage </B> calls <B> GetBuffer
		 * </B> and then casts the returned buffer to an image. <B> GetBuffer
		 * </B> does not perform the cast, instead returning the raw buffer data.
		 * The buffer object may be queried for its type
		 * (Arena::IBuffer::GetPayloadType, Arena::IBuffer::HasImageData,
		 * Arena::IBuffer::HasChunkData) and cast to the appropriate type.
		 *
		 * \code{.cpp}
		 * 	// handling buffers with image data, chunk data, and both
		 * 	// notice the second if-statement does not contain 'else'
		 * 	// this is because buffers may have both image and chunk data
		 * 	{
		 * 		Arena::IBuffer* pBuffer = pDevice->GetBuffer(100);
		 * 		
		 * 		if (pBuffer->HasImageData())
		 * 		{
		 * 			Arena::IImage* pImage = dynamic_cast<Arena::IImage*>(pBuffer);
		 * 			// ...
		 * 		}
		 * 		
		 * 		if (pBuffer->HasChunkData())
		 * 		{
		 * 			Arena::IChunkData* pChunkData = dynamic_cast<Arena::IChunkData*>(pBuffer);
		 * 			// ...
		 * 		}
		 * 	}
		 * \endcode
		 *
		 * When called, <B> GetBuffer </B> checks the output queue for
		 * image/chunk data, grabbing the first buffer in the queue. If nothing
		 * is in the output queue, the call will wait until something arrives. If
		 * nothing arrives before expiration of the timeout, a
		 * GenICam::TimeoutException is thrown.
		 *
		 * <B> GetBuffer </B> is a blocking call. If it is called with a timeout
		 * of 20 seconds and nothing arrives in the output queue, then its thread
		 * will be blocked for the full 20 seconds. However, as the timeout is a
		 * maximum, as soon as something arrives in the output queue, it will be
		 * returned, not waiting for the full timeout. A timeout value of 0
		 * ensures the call will not block, throwing instead of waiting if
		 * nothing is in the output queue.
		 *
		 * Best practices recommends that buffers be requeued
		 * (Arena::IDevice::RequeueBuffer) as soon as they are no longer needed. If
		 * image data is needed for a longer period of time (i.e. for
		 * processing), it is recommended to copy the data
		 * (Arena::ImageFactory::Create) and requeue the buffer.
		 *
		 * @warning 
		 *  - Does not guarantee valid data
		 *  - Buffers should be requeued
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer
		 *  - Arena::IImage
		 *  - Arena::IChunkData
		 *  - Arena::IDevice::StartStream
		 *  - Arena::IDevice::StopStream
		 *  - Arena::IDevice::RequeueBuffer
		 *  - Arena::IBuffer::GetPayloadType
		 *  - Arena::IBuffer::HasImageData
		 *  - Arena::IBuffer::HasChunkData
		 *  - Arena::ImageFactory::Create
		 *  - Arena::EBufferPayloadType
		 */
		virtual IBuffer* GetBuffer(uint64_t timeout) = 0;

		/**
		 * @fn virtual void RequeueBuffer(IBuffer* pBuffer)
		 *
		 * @param pBuffer
		 *  - Type: Arena::IBuffer*
		 *  - Buffer to requeue
		 *
		 * @return 
		 *  - none
		 *
		 * <B> RequeueBuffer </B> relinquishes control of a buffer
		 * (Arena::IBuffer, Arena::IImage, Arena::IChunkData) back to Arena. It
		 * must be called after a buffer has been retrieved
		 * (Arena::IDevice::GetBuffer, Arena::IDevice::GetImage).
		 *
		 * When called, <B> RequeueBuffer </B> deallocates any lazily
		 * instantiated memory and returns the internal buffer to the acquisition
		 * engine's input queue, where it can be filled with new data. If enough
		 * buffers have been removed from the acquisition engine (i.e. not
		 * requeued), it is possible to starve the acquisition engine. If this
		 * happens and depending on the buffer handling mode
		 * (Arena::IDevice::GetTLStreamNodeMap, 'StreamBufferHandlingMode'), data
		 * may start being dropped or buffers may start being recycled.
		 *
		 * Best practices recommends that buffers be requeued as soon as they are
		 * no longer needed. If image data is needed for a longer period of time
		 * (i.e. for processing), it is recommended to copy the data
		 * (Arena::ImageFactory::Create) and requeue the buffer.
		 *
		 * It is important to only call <B> RequeueBuffer </B> on buffers
		 * retrieved from a device (Arena::IDevice::GetBuffer,
		 * Arena::IDevice::GetImage), and not on images created through the image
		 * factory (Arena::ImageFactory).
		 *
		 * \code{.cpp}
		 * 	// one image is retrieved from a buffer and then requeued
		 * 	// another is created through the image factory and then destroyed
		 * 	{
		 * 		Arena::IImage* pRetrievedImage = pDevice->GetImage();
		 * 		// ...
		 * 		pDevice->RequeueBuffer(pRetrievedImage);
		 * 		
		 * 		Arena::IImage* pCreatedImage = Arena::ImageFactory::Create(pData, height, width, bpp, size, pixelFormat);
		 * 		// ...
		 * 		Arena::ImageFactory::Destroy(pCreatedImage);
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - Buffers should be requeued
		 *  - Used only on buffers retrieved from a device, not on images
		 *    created through the image factory
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer
		 *  - Arena::IImage
		 *  - Arena::IChunkData
		 *  - Arena::ImageFactory
		 *  - Arena::IDevice::GetBuffer
		 *  - Arena::IDevice::GetImage
		 *  - Arena::IDevice::GetTLStreamNodeMap
		 *  - Arena::ImageFactory::Create
		 *  - Arena::ImageFactory::Destroy
		 */
		virtual void RequeueBuffer(IBuffer* pBuffer) = 0;

		/**
		 * @fn virtual void InitializeEvents()
		 *
		 * @return 
		 *  - none
		 *
		 * <B> InitializeEvents </B> causes the underlying events engine to start
		 * listening for events. It must be called before waiting on events
		 * (Arena::IDevice::WaitOnEvent). The event infrastructure must be turned off
		 * (Arena::IDevice::DeinitializeEvents) when no longer needed.
		 *
		 * The underlying events engine works very similarly to the acquisition
		 * engine, except that event data is processed instead of image data. It
		 * consists of 100 buffers, an input and an output queue, and event
		 * registration information. When an event fires, the events engine takes
		 * an event buffer from the input queue, stores all relevant data, and
		 * places it in the output queue. When <B> WaitOnEvent </B>
		 * (Arena::IDevice::WaitOnEvent) is called, the engine takes the buffer from
		 * the output queue, processes its data, and returns it to the input
		 * queue.
		 *
		 * More specifically, <B> InitializeEvents </B>:
		 *  - allocates and registers 100 buffers for the events engine
		 *  - places all buffers into the input queue
		 *  - opens a message channel socket
		 *  - configures the IP and port, and sets the packet size
		 *  - fires a dummy packet to help with firewalls
		 *  - starts the worker thread listening for event packets
		 *
		 * Events are transmitted from the device through the GigE Vision message
		 * channel. Arena processes event data internally, which it attaches to
		 * the device node map (not the GenTL device node map) using a
		 * GenApi::EventAdapter. The appropriate nodes are then updated in the
		 * node map. It can be helpful to incorporate callbacks
		 * (GenApi::Callback) to be notified when these events occur.
		 *
		 * \code{.cpp}
		 * 	// initializing events, registering and firing a test event, and deinitializing an event
		 * 	// (same code snippet as Arena::IDevice::WaitOnEvent)
		 * 	{
		 * 		void OnNodeCallback(GenApi::INode* pNode)
		 * 		{
		 * 			GenApi::CIntegerPtr pTimestamp = node;
		 * 			// ...
		 * 		}
		 * 		
		 * 		GenApi::INode* pEventTestTimestamp = pNodeMap->GetNode("EventTestTimestamp");
		 * 		GenApi::CCommandPtr pTestEventGenerate = pNodeMap->GetNode("TestEventGenerate");
		 * 		
		 * 		pDevice->InitializeEvents();
		 * 		GenApi::CallbackHandleType hCallback = GenApi::Register(pEventTestTimestamp, OnNodeCallback);
		 * 		
		 * 		for (int i = 0; i < 10; i++)
		 * 		{
		 * 			pTestEventGenerate->Execute();
		 * 			pDevice->WaitOnEvent(100);
		 * 		}
		 * 		
		 * 		GenApi::Deregister(hCallback);
		 * 		pDevice->DeinitializeEvents();
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - Event infrastructure must be deinitialized
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice::WaitOnEvent
		 *  - Arena::IDevice::DeinitializeEvents
		 */
		virtual void InitializeEvents() = 0;

		/**
		 * @fn virtual void DeinitializeEvents()
		 *
		 * @return 
		 *  - none
		 *
		 * <B> DeinitializeEvents </B> stops the underlying events engine from
		 * listening for messages, shutting it down and cleaning it up. It should
		 * be called only after the events infrastructure has been initialized
		 * (Arena::IDevice::InitializeEvents) and after all events have been
		 * processed (Arena::IDevice::WaitOnEvent).
		 *
		 * Roughly speaking, <B> DeinitializeEvents </B> takes all necessary
		 * steps to undoing and cleaning up the event infrastructure's
		 * initialization
		 * (Arena::IDevice::InitializeEvents). This includes: 
		 *  - stopping the worker thread set up to listen for events
		 *  - closes the message channel socket
		 *  - unregisters all event buffers and deallocates their memory
		 *
		 * @warning 
		 *  - Event infrastructure must be deinitialized
		 *  - Stops events processing
		 *  - Deallocates event data that has not yet been processed
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * Arena::IDevice::DeinitializeEvents will perform these actions:
		 *  - Stop the listening thread
		 *  - shut down the message channel socket
		 *  - unregister all buffers and clean up memory that was allocated for
		 *    them
		 *
		 * @see 
		 *  - Arena::IDevice::InitializeEvents
		 *  - Arena::IDevice::WaitOnEvent
		 *  - Arena::IDevice::DeinitializeEvents
		 */
		virtual void DeinitializeEvents() = 0;

		/**
		 * @fn virtual void WaitOnEvent(uint64_t timeout)
		 *
		 * @param timeout
		 *  - Type: uint64_t
		 *  - Unit: milliseconds
		 *  - Maximum time to wait for an event
		 *
		 * @return 
		 *  - none
		 *
		 * <B> WaitOnEvent </B> waits for an event to occur in order to process
		 * its data. It must be called after the event infrastructure has been
		 * initialized (Arena::IDevice::InitializeEvents) and before it is
		 * deinitialized (Arena::IDevice::DeinitializeEvents).
		 *
		 * Event processing has been designed to largely abstract away its
		 * complexities. When an event occurs, the data is stored in an event
		 * buffer and placed on the output queue. <B> WaitOnEvent </B> causes the
		 * data to be processed, updating all relevant nodes appropriately. This
		 * is why <B> WaitOnEvent </B> does not return any event data; when the
		 * data is processed, nodes are updated, which can then be queried for
		 * information through the node map. This is also why callbacks
		 * (GenApi::Callback) work so well with the events infrastructure; they
		 * provide a method of accessing nodes of interest as they change.
		 *
		 * \code{.cpp}
		 * 	// initializing events infrastructure, registering and firing a test event, and deinitializing
		 * 	// (same code snippet as Arena::IDevice::InitializeEvents)
		 * 	{
		 * 		void OnNodeCallback(GenApi::INode* pNode)
		 * 		{
		 * 			GenApi::CIntegerPtr pTimestamp = node;
		 * 			// ...
		 * 		}
		 * 		
		 * 		GenApi::INode* pEventTestTimestamp = pNodeMap->GetNode("EventTestTimestamp");
		 * 		GenApi::CCommandPtr pTestEventGenerate = pNodeMap->GetNode("TestEventGenerate");
		 * 		
		 * 		pDevice->InitializeEvents();
		 * 		GenApi::CallbackHandleType hCallback = GenApi::Register(pEventTestTimestamp, OnNodeCallback);
		 * 		
		 * 		for (int i = 0; i < 10; i++)
		 * 		{
		 * 			pTestEventGenerate->Execute();
		 * 			pDevice->WaitOnEvent(100);
		 * 		}
		 * 		
		 * 		GenApi::Deregister(hCallback);
		 * 		pDevice->DeinitializeEvents();
		 * 	}
		 * \endcode
		 *
		 * When called, <B> WaitOnEvent </B> checks the output queue for event
		 * data to process, grabbing the first buffer from the queue. If nothing
		 * is in the output queue, the call will wait until an event arrives. If
		 * nothing arrives before expiration of the timeout, a
		 * GenICam::TimeoutException is thrown.
		 *
		 * <B> WaitOnEvent </B> is a blocking call. If it is called with a
		 * timeout of 20 seconds and nothing arrives in the output queue, then
		 * its thread will be blocked for the full 20 seconds. However, as the
		 * timeout is a maximum, when an event arrives in the output queue, the
		 * event will process, not waiting for the full timeout. A timeout value
		 * of 0 ensures the call will not block, throwing instead of waiting if
		 * nothing is in the output queue.
		 *
		 * @warning 
		 *  - Event data processed internally
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice::InitializeEvents
		 *  - Arena::IDevice::DeinitializeEvents
		 */
		virtual void WaitOnEvent(uint64_t timeout) = 0;

		/**
		 * @fn virtual GenApi::INodeMap* GetNodeMap()
		 *
		 * @return 
		 *  - Type: GenApi::INodeMap*
		 *  - Pointer to the main node map for the device
		 *
		 * <B> GetNodeMap </B> retrieves the already initialized main node map
		 * (GenApi::INodeMap), used to access a device's complete feature set of
		 * nodes (GenApi::INode).
		 *
		 * As a simple getter, <B> GetNodeMap </B> retrieves this node map
		 * without doing anything to initialize, manage, or maintain it. This
		 * node map is initialized when the device is created
		 * (Arena::ISystem::CreateDevice) and deinitialized when the device is
		 * destroyed
		 * (Arena::ISystem::DestroyDevice). Because node maps are cleaned up
		 * internally, retrieving multiple pointers to the same node map is
		 * permitted.
		 *
		 * The node map is built from XMLs stored on the device itself. The XML
		 * is downloaded and parsed before constructing and initializing the node
		 * map. This node map describes and provides access to all device
		 * features, and may vary from device to device. Lucid products conform
		 * to the SFNC 2.3 specification. Note that both chunk data and event
		 * data are updated on this node map.
		 *
		 * Arena provides access to five different node maps. This one comes from
		 * the device and describes all its features. Please check device
		 * documentation for more information regarding these features.
		 *  - device (Arena::IDevice::GetNodeMap)
		 *
		 * The other four node maps describe and provide access to information
		 * and settings through the software rather than the device.
		 *  - system GenTL (Arena::ISystem::GetNodeMap)
		 *  - stream GenTL (Arena::IDevice::GetTLStreamNodeMap)
		 *  - device GenTL (Arena::IDevice::GetTLDeviceNodeMap)
		 *  - interface GenTL (Arena::IDevice::GetTLInterfaceNodeMap)
		 *
		 * The most noticable difference between the two device node maps is that
		 * the GenTL device node map has only a small set of features compared to
		 * the main node map. There are a few features that overlap. For example,
		 * the difference between retrieving the serial number
		 * ('DeviceSerialNumber') is that using the main node map queries the
		 * camera directly whereas the GenTL node map queries a set of
		 * information cached at device creation. The result, however, should be
		 * the same. Basically, the GenTL node map queries the software for
		 * information whereas the main node map queries the device.
		 *
		 * @warning 
		 *  - Provides access to main node map, not to be confused with the
		 *    GenTL device node map
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::ISystem::CreateDevice
		 *  - Arena::ISystem::DestroyDevice
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::ISystem::GetNodeMap
		 *  - Arena::IDevice::GetTLStreamNodeMap
		 *  - Arena::IDevice::GetTLDeviceNodeMap
		 *  - Arena::IDevice::GetTLInterfaceNodeMap
		 */
		virtual GenApi::INodeMap* GetNodeMap() = 0;

		/**
		 * @fn virtual GenApi::INodeMap* GetTLDeviceNodeMap()
		 *
		 * @return 
		 *  - Type: GenApi::INodeMap*
		 *  - Pointer to the GenTL node map for the device
		 *
		 * <B> GetTLDeviceNodeMap </B> retrieves the already initialized GenTL
		 * device node map (GenApi::INodeMap), used to access a subset of cached
		 * device related nodes (GenApi::INode).
		 *
		 * As a simple getter, <B> GetTLDeviceNodeMap </B> retrieves this node
		 * map without doing anything to initialize, manage, or maintain it. This
		 * node map is initialized when the device is created
		 * (Arena::ISystem::CreateDevice) and deinitialized when the device is
		 * destroyed (Arena::ISystem::DestroyDevice). Because node maps are
		 * cleaned up internally, retrieving multiple pointers to the same node
		 * map is permitted.
		 *
		 * All available nodes can be viewed in the XML,
		 * SFNC_GenTLDevice_Reference_Version_1_0_0_Schema_1_1.xml, found in
		 * Arena/<version>/<platform>/xml. Nodes in this node map include nodes
		 * related to:
		 *  - device discovery information
		 *  - GigE Vision IP configuration information
		 *  - the ability to select streams
		 *
		 * Arena provides access to five different node maps. This one comes from
		 * the device and describes all its features. Please check device
		 * documentation for more information regarding these features.
		 *  - device (Arena::IDevice::GetNodeMap)
		 *
		 * The other four, including this one, node maps describe and provide
		 * access to information and settings through the software rather than
		 * the device.
		 *  - system GenTL (Arena::ISystem::GetNodeMap)
		 *  - stream GenTL (Arena::IDevice::GetTLStreamNodeMap)
		 *  - device GenTL (Arena::IDevice::GetTLDeviceNodeMap)
		 *  - interface GenTL (Arena::IDevice::GetTLInterfaceNodeMap)
		 *
		 * The most noticable difference between the two device node maps is that
		 * the GenTL device node map has only a small set of features compared to
		 * the main node map. There are a few features that overlap. For example,
		 * the difference between retrieving the serial number
		 * ('DeviceSerialNumber') is that using the main node map queries the
		 * camera directly whereas the GenTL node map queries a set of
		 * information cached at device creation. The result, however, should be
		 * the same. Basically, the GenTL node map queries the software for
		 * information whereas the main node map queries the device.
		 *
		 * @warning 
		 *  - Provides access to the GenTL device node map, not to be confused
		 *    with the main device node map
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::ISystem::CreateDevice
		 *  - Arena::ISystem::DestroyDevice
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::ISystem::GetNodeMap
		 *  - Arena::IDevice::GetTLStreamNodeMap
		 *  - Arena::IDevice::GetTLDeviceNodeMap
		 *  - Arena::IDevice::GetTLInterfaceNodeMap
		 */
		virtual GenApi::INodeMap* GetTLDeviceNodeMap() = 0;

		/**
		 * @fn virtual GenApi::INodeMap* GetTLStreamNodeMap()
		 *
		 * @return 
		 *  - Type: GenApi::INodeMap*
		 *  - Pointer to the GenTL node map for the stream
		 *
		 * <B> GetTLStreamNodeMap </B> retrieves the already initialized GenTL
		 * stream node map (GenApi::INodeMap), used to access stream related
		 * nodes
		 * (GenApi::INode).
		 *
		 * As a simple getter, <B> GetTLStreamNodeMap </B> retrieves this node
		 * map without doing anything to initialize, manage, or maintain it. This
		 * node map is initialized when the device is created
		 * (Arena::ISystem::CreateDevice) and deinitialized when the device is
		 * destroyed (Arena::ISystem::DestroyDevice). Because node maps are
		 * cleaned up internally, retrieving multiple pointers to the same node
		 * map is permitted.
		 *
		 * All available nodes can be viewed in the XML,
		 * SFNC_GenTLDataStream_Reference_Version_1_0_0_Schema_1_1.xml, found in
		 * Arena/<version>/<platform>/xml. Nodes in this node map include nodes
		 * related to:
		 *  - stream ID and type
		 *  - buffer handling mode
		 *  - stream information such as the payload size or whether the device
		 *    is currently streaming
		 *  - stream statistics such as lost frames, announced buffers, or
		 *    missed packets
		 *
		 * Arena provides access to five different node maps. This one comes from
		 * the device and describes all its features. Please check device
		 * documentation for more information regarding these features.
		 *  - device (Arena::IDevice::GetNodeMap)
		 *
		 * The other four, including this one, node maps describe and provide
		 * access to information and settings through the software rather than
		 * the device.
		 *  - system GenTL (Arena::ISystem::GetNodeMap)
		 *  - stream GenTL (Arena::IDevice::GetTLStreamNodeMap)
		 *  - device GenTL (Arena::IDevice::GetTLDeviceNodeMap)
		 *  - interface GenTL (Arena::IDevice::GetTLInterfaceNodeMap)
		 *
		 * A common use case requiring this node map is to configure the buffer
		 * handling mode prior to starting the stream.
		 *
		 * \code{.cpp}
		 * 	// setting the buffer handling mode to 'NewestOnly' before starting the stream
		 * 	{
		 * 		GenApi::INodeMap* pStreamNodeMap = pDevice->GetTLStreamNodeMap();
		 * 		
		 * 		GenApi::CEnumerationPtr pStreamBufferHandlingMode = pStreamNodeMap->GetNode("StreamBufferHandlingMode");
		 * 		GenApi::CEnumEntry pNewestOnly = pStreamBufferHandlingMode->GetEntryByName("NewestOnly");
		 * 		pStreamBufferHandlingMode->SetIntValue(pNewestOnly->GetValue());
		 * 		
		 * 		pDevice->StartStream();
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::ISystem::CreateDevice
		 *  - Arena::ISystem::DestroyDevice
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::ISystem::GetNodeMap
		 *  - Arena::IDevice::GetTLStreamNodeMap
		 *  - Arena::IDevice::GetTLDeviceNodeMap
		 *  - Arena::IDevice::GetTLInterfaceNodeMap
		 */
		virtual GenApi::INodeMap* GetTLStreamNodeMap() = 0;

		/**
		 * @fn virtual GenApi::INodeMap* GetTLInterfaceNodeMap()
		 *
		 * @return 
		 *  - Type: GenApi::INodeMap*
		 *  - Pointer to the GenTL node map for the interface
		 *
		 * <B> GetTLInterfaceNodeMap </B> retrieves the already initialized GenTL
		 * interface node map (GenApi::INodeMap), used to access interface
		 * related nodes (GenApi::INode).
		 *
		 * As a simple getter, <B> GetTLInterfaceNodeMap </B> retrieves this node
		 * map without doing anything to initialize, manage, or maintain it. This
		 * node map is initialized when the device is created
		 * (Arena::ISystem::CreateDevice) and deinitialized when the device is
		 * destroyed (Arena::ISystem::DestroyDevice). Because node maps are
		 * cleaned up internally, retrieving multiple pointers to the same node
		 * map is permitted.
		 *
		 * All available nodes can be viewed in the XML,
		 * SFNC_GenTLInterface_Reference_Version_1_0_0_Schema_1_1.xml, found in
		 * Arena/<version>/<platform>/xml. Nodes in this node map include nodes
		 * related to:
		 *  - interface discovery information
		 *  - interface IP configuration information
		 *  - ability to update and select devices
		 *  - device discovery and IP configuration information
		 *
		 * Arena provides access to five different node maps. This one comes from
		 * the device and describes all its features. Please check device
		 * documentation for more information regarding these features.
		 *  - device (Arena::IDevice::GetNodeMap)
		 *
		 * The other four, including this one, node maps describe and provide
		 * access to information and settings through the software rather than
		 * the device.
		 *  - system GenTL (Arena::ISystem::GetNodeMap)
		 *  - stream GenTL (Arena::IDevice::GetTLStreamNodeMap)
		 *  - device GenTL (Arena::IDevice::GetTLDeviceNodeMap)
		 *  - interface GenTL (Arena::IDevice::GetTLInterfaceNodeMap)
		 *
		 * @warning 
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::ISystem::CreateDevice
		 *  - Arena::ISystem::DestroyDevice
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::ISystem::GetNodeMap
		 *  - Arena::IDevice::GetTLStreamNodeMap
		 *  - Arena::IDevice::GetTLDeviceNodeMap
		 *  - Arena::IDevice::GetTLInterfaceNodeMap
		 */
		virtual GenApi::INodeMap* GetTLInterfaceNodeMap() = 0;

		        virtual void SendActionCommand(uint32_t deviceKey, uint32_t groupKey, uint32_t groupMask, uint64_t actionTime) = 0;

		/**
		* @fn virtual void RegisterImageCallback(IImageCallback* callback, void* pParam = NULL)
		*
		* @param callback
		*  - Type: IImageCallback*
		*  - A pointer to an IImageCallback object which implements the OnImage() function
		*
		* @return
		*  - none		
		*
		* <B> RegisterImageCallback </B> registers a callback object derived from Arena::IImageCallback, 
		* implementing the OnImage() function. When an image arrives, the user-implemented OnImage() 
		* will be called. The user-implemented OnImage() will receive a pointer to the image 
		* data (Arena::IImage* pImage).
		* 
		* Callbacks can be registered whether or not the device is currently streaming.
		* Multiple callbacks can be registered for a device and each will be called sequentially in
		* the order that they have been registered in.
		*
		* RegisterImageCallback will throw : 
		*  - InvalidArgumentException if the callback argument is NULL
		*  - LogicalErrorException if the callback has already been registered
		* 
		* 
		* \code{.cpp}
		*   // Register callback object
		*   {
		*     std::string camSerial = deviceInfos[0].SerialNumber();
		*     ImageCallback* pCallbackHandler = new ImageCallback(camSerial);
		*
		*     pDevice->RegisterImageCallback(pCallbackHandler);
		*
		*     pDevice->StartStream();
		*     Sleep(5000);
		*     pDevice->StopStream();
		*
		*     pDevice->DeregisterImageCallback(pCallbackHandler);
		*
		*     delete pCallbackHandler;
		*   }
		* \endcode
		*
		* @warning
		*  - Device::GetImage() should not be called while callbacks are registered for a device.
		*  - The user-implemented OnImage() should complete in a timely manner to ensure that 
		*    image reception is not impeded as the internal grab thread is blocked while the callbacks
		*    are running. If the required work cannot be performed quickly it is recommended that 
		*    the image be copied in OnImage() so that control can be returned to the library and the image
		*    can be processed in a separate thread.
		*
		* @see
		*  - Device:DeregisterImageCallback
		*  - Device:DeregisterAllImageCallback
		*/
		virtual void RegisterImageCallback(IImageCallback* callback) = 0;

		/**
		* @fn virtual bool DeregisterImageCallback(IImageCallback* callback)
		*
		* @param callback
		*  - Type: IImageCallback*
		*  - A pointer to an IImageCallback object which implements the OnImage() function
		*
		* @return
		*  - Type: bool
		*  - True if the specfied callback was in the list of registered callbacks. Otherwise false.
		*
		* <B> DeregisterImageCallback </B> unregisters a previously registered callback object.
		* Callbacks can be unregistered whether or not the device is currently streaming.
		* To deregister all callbacks at once use DeregisterAllImageCallbacks.
		*
		* RegisterImageCallback will throw :
		*  - InvalidArgumentException if the callback argument is NULL
		*
		* \code{.cpp}
		*   // Register callback object
		*   {
		*     std::string camSerial = deviceInfos[0].SerialNumber();
		*     ImageCallback* pCallbackHandler = new ImageCallback(camSerial);
		*
		*     pDevice->RegisterImageCallback(pCallbackHandler);
		*
		*     pDevice->StartStream();
		*     Sleep(5000);
		*     pDevice->StopStream();
		*
		*     pDevice->DeregisterImageCallback(pCallbackHandler);
		*
		*     delete pCallbackHandler;
		*   }
		* \endcode
		*
		* @see
		*  - Device:RegisterImageCallback
		*  - Device:DeregisterAllImageCallbacks
		*/
		virtual bool DeregisterImageCallback(IImageCallback* callback) = 0;

		/**
		* @fn virtual bool DeregisterAllImageCallbacks()
		*
		* @return
		*  - Type: bool
		*  - True if there were any callbacks registered before removing them all. Otherwise false.
		*
		* <B> DeregisterAllImageCallbacks </B> deregisters all previously registered callback objects.
		* Callbacks can be unregistered whether or not the device is currently streaming.
		* To deregister an individual callback, DeregisterImageCallback.
		*
		*
		* \code{.cpp}
		*   // Register callback object
		*   {
		*     std::string camSerial = deviceInfos[0].SerialNumber();
		*     ImageCallback* pCallbackHandler = new ImageCallback(camSerial);
		*
		*     pDevice->RegisterImageCallback(pCallbackHandler);
		*
		*     pDevice->StartStream();
		*     Sleep(5000);
		*     pDevice->StopStream();
		*
		*     pDevice->DeregisterAllImageCallbacks();
		*
		*     delete pCallbackHandler;
		*   }
		* \endcode
		*
		* @see
		*  - Device:RegisterImageCallback
		*  - Device:DeregisterImageCallback
		*/
		virtual bool DeregisterAllImageCallbacks() = 0;

		/**
		 * @fn virtual ~IDevice()
		 *
		 * A destructor
		 */
		virtual ~IDevice(){};

	protected:

		IDevice(){};
	};
} // namespace Arena
