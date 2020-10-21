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

#include "IBuffer.h"

namespace Arena
{
	class IChunkData;

	/**
	 * @class IImage
	 *
	 * An interface to an image object
	 *
	 * Images are the most common form of data retrieved from the stream engine.
	 * They are retrieved and requeued via devices (Arena::IDevice) as well as
	 * created, copied, and converted via the image factory
	 * (Arena::ImageFactory).
	 *
	 * \code{.cpp}
	 * 	// retrieving an image after starting the stream
	 * 	// requeuing it before stopping the stream
	 * 	{
	 * 		pDevice->StartStream();
	 * 		Arena::IImage* pImage = pDevice->GetImage(100);
	 * 		// ...
	 * 		pDevice->RequeueBuffer(pImage);
	 * 		pDevice->StopStream();
	 * 	}
	 * \endcode
	 *
	 * Along with the functionality of its parent class (Arena::IBuffer), images
	 * provide access to additional information particular to images. This
	 * includes:
	 *  - size information (Arena::IImage::GetWidth, Arena::IImage::GetHeight
	 *  - offsets (Arena::IImage::GetOffsetX, Arena::IImage::GetOffsetY)
	 *  - padding (Arena::IImage::GetPaddingX, Arena::IImage::GetPaddingY)
	 *  - pixel information (Arena::IImage::GetPixelFormat,
	 *    Arena::IImage::GetPixelEndianness)
	 *  - timestamps (Arena::IImage::GetTimestamp,
	 *    Arena::IImage::GetTimestampNs)
	 *
	 * It is important to note that images retrieved from the camera must be
	 * requeued (Arena::IDevice::RequeueBuffer) whereas images created using the
	 * image factory must be destroyed (Arena::ImageFactory::Destroy).
	 *
	 * \code{.cpp}
	 * 	// retrieving an image, copying it, requeuing its buffer, and destroying the copy
	 * 	{
	 * 		Arena::IImage* pRetrievedImage = pDevice->GetImage(timeout);
	 * 		Arena::IImage* pCopiedImage = Arena::ImageFactory::Create(pImage->GetData(), pImage->GetHeight(), pImage->GetWidth(), pImage->GetBitsPerPixel(), pImage->GetImageLength(), pImage->GetPixelFormat());
	 * 		pDevice->RequeueBuffer(pRetrievedImage);
	 * 		// ...
	 * 		Arena::ImageFactory::Destroy(pCopiedImage);
	 * 	}
	 * \endcode
	 *
	 * @warning 
	 *  - Should be requeued if retrieved from the device
	 *  - Must be destroyed if created by the image factory
	 *  - Properties of images from the stream engine are lazily instantiated
	 *  - Properties of images from the image factory may be unavailable
	 *
	 * @see 
	 *  - Arena::IDevice
	 *  - Arena::ImageFactory
	 *  - Arena::IImage
	 *  - Arena::IBuffer
	 */
	class ARENA_API IImage : public IBuffer
	{
	public:
		/**
		 * @fn virtual size_t GetWidth()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: pixels
		 *  - Width of the image
		 *
		 * <B> GetWidth </B> gets the width of the image in pixels. Images are
		 * self-describing, so the device does not need to be queried to get this
		 * information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the width is populated by the acquisition engine
		 * payload leader. The device itself is not queried as this data is
		 * present in the image data. If the image was created by the image
		 * factory, the width is populated by the arguments.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 */
		virtual size_t GetWidth() = 0;

		/**
		 * @fn virtual size_t GetHeight()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: pixels
		 *  - Height of the image
		 *
		 * <B> GetHeight </B> gets the height of the image. Images are
		 * self-describing, so the device does not need to be queried to get this
		 * information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the height is populated by the acquisition engine
		 * payload leader. The device itself is not queried as this data is
		 * present in the image data. If the image was created by the image
		 * factory, the height is populated by the arguments.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 */
		virtual size_t GetHeight() = 0;

		/**
		 * @fn virtual size_t GetOffsetX()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: pixels
		 *  - Offset X of the image
		 *
		 * <B> GetOffsetX </B> gets the offset of the image along the X-axis.
		 * Images are self-describing, so the device does not need to be queried
		 * to get this information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the offset X is populated by the acquisition engine
		 * payload leader. The device itself is not queried as this data is
		 * present in the image data. If the image was created by the image
		 * factory, the offset X is populated by the arguments.
		 *
		 * The image factory can create an image from another image
		 * (Arena::IImage) or from a minimal set of parameters (data, width,
		 * height, pixel format). If the image is created from parameters, the
		 * offset X will be set to 0, no matter its original value.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 *  - Arena::IImage
		 */
		virtual size_t GetOffsetX() = 0;

		/**
		 * @fn virtual size_t GetOffsetY()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: pixels
		 *  - Offset Y of the image
		 *
		 * <B> GetOffsetY </B> gets the offset of the image along the Y-axis.
		 * Images are self-describing, so the device does not need to be queried
		 * to get this information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the offset Y is populated by the acquisition engine
		 * payload leader. The device itself is not queried as this data is
		 * present in the image data. If the image was created by the image
		 * factory, the offset Y is populated by the arguments.
		 *
		 * The image factory can create an image from another image
		 * (Arena::IImage) or from a minimal set of parameters (data, width,
		 * height, pixel format). If the created from parameters, the offset Y
		 * will be set to 0, no matter its original value.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 *  - Arena::IImage
		 */
		virtual size_t GetOffsetY() = 0;

		/**
		 * @fn virtual size_t GetPaddingX()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: pixels
		 *  - Padding X of the image
		 *
		 * <B> GetPaddingX </B> gets the padding of the image along the X-axis.
		 * Images are self-describing, so the device does not need to be queried
		 * to get this information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the padding X is populated by the acquisition engine
		 * payload leader. The device itself is not queried as this data is
		 * present in the image data. If the image was created by the image
		 * factory, the padding X is populated by the arguments.
		 *
		 * The image factory can create an image from another image
		 * (Arena::IImage) or from a minimal set of parameters (data, width,
		 * height, pixel format). If the image was created from parameters, the
		 * padding X will be set to 0, no matter its original value.
		 *
		 * Padding X specifically refers to the number of bytes padding the end
		 * of each line. This number will affect the pitch/stride/step of an
		 * image.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 *  - Arena::IImage
		 */
		virtual size_t GetPaddingX() = 0;

		/**
		 * @fn virtual size_t GetPaddingY()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: pixels
		 *  - Padding Y of the image
		 *
		 * <B> GetPaddingY </B> gets the padding of the image along the Y-axis.
		 * Images are self-describing, so the device does not need to be queried
		 * to get this information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the padding Y is populated by the acquisition engine
		 * payload leader. The device itself is not queried as this data is
		 * present in the image data. If the image was created by the image
		 * factory, the padding Y is populated by the arguments.
		 *
		 * The image factory can create an image from another image
		 * (Arena::IImage) or from a minimal set of parameters (data, width,
		 * height, pixel format). If the image was created from parameters, the
		 * padding Y will be set to 0, no matter its original value.
		 *
		 * Padding Y specifically refers to the number of bytes padding the end
		 * of an image. This number will not affect the pitch/stride/step of an
		 * image.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 *  - Arena::IImage
		 */
		virtual size_t GetPaddingY() = 0;

		/**
		 * @fn virtual uint64_t GetPixelFormat()
		 *
		 * @return 
		 *  - Type: uint64_t
		 *  - Represents: enum PfncFormat
		 *  - Pixel format of the image
		 *
		 * <B> GetPixelFormat </B> gets the pixel format (PfncFormat) of the
		 * image, as defined by the PFNC (Pixel Format Naming Convention). Images
		 * are self-describing, so the device does not need to be queried to get
		 * this information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the pixel format is populated by the acquisition engine
		 * payload leader. The device itself is not queried as this data is
		 * present in the image data. If the image was created by the image
		 * factory, the pixel format is populated by the arguments.
		 *
		 * Pixel format value are determined by the PFNC (Pixel Format Naming
		 * Convention) specification. The PFNC assigns a name and number to each
		 * pixel format helping to standardize pixel formats. The number of bits
		 * per pixel can be found in each integer at bytes 5 and 6 (mask
		 * 0x00FF0000). The pixel format can be determined by the integer using
		 * the GetPixelFormatName function provided by the PFNC.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 */
		virtual uint64_t GetPixelFormat() = 0;

		/**
		 * @fn virtual size_t GetBitsPerPixel()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: pixels
		 *  - Bits per pixel of the image
		 *
		 * <B> GetBitsPerPixel </B> gets the number of bits per pixel of the
		 * image from the integer value of the pixel format (PfncFormat).
		 * Internally, a public helper funciton is called
		 * (Arena::GetBitsPerPixel).
		 *
		 * Pixel format value are determined by the PFNC (Pixel Format Naming
		 * Convention) specification. The PFNC assigns a name and number to each
		 * pixel format helping to standardize pixel formats. The number of bits
		 * per pixel can be found in each integer at bytes 5 and 6 (mask
		 * 0x00FF0000). The pixel format can be determined by the integer using
		 * the GetPixelFormatName function provided by the PFNC.
		 *
		 * @see 
		 *  - Arena::GetBitsPerPixel
		 */
		virtual size_t GetBitsPerPixel() = 0;

		/**
		 * @fn virtual int32_t GetPixelEndianness()
		 *
		 * @return 
		 *  - Type: int32_t
		 *  - Represents: enum Arena::EPixelEndianness
		 *  - Endianness of the pixels of the image
		 *
		 * <B> GetPixelEndianness </B> gets the pixel endianness
		 * (Arena::EPixelEndianness) of the image. Images are self-describing, so the
		 * device does not need to be queried to get this information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the pixel endianness is populated by the acquisition
		 * engine payload leader. The device itself is not queried as this data
		 * is present in the image data. If the image was created by the image
		 * factory, the pixel endianness is populated by the arguments.
		 *
		 * The image factory can create an image from another image
		 * (Arena::IImage) or from a minimal set of parameters (data, width,
		 * height, pixel format). If the image was created from parameters, the
		 * pixel endianness will be set to 0
		 * (EPixelEndianness::PixelEndiannessUnknown), no matter its original
		 * value.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 *  - Arena::IImage
		 *  - Arena::EPixelEndianness
		 */
		virtual int32_t GetPixelEndianness() = 0;

		/**
		 * @fn virtual uint64_t GetTimestamp()
		 *
		 * @return 
		 *  - Type: uint64_t
		 *  - Unit: nanoseconds
		 *  - Timestamp of the image in nanoseconds
		 *
		 * <B> GetTimestamp </B> gets the timestamp of the image in nanoseconds.
		 * Images are self-describing, so the device does not need to be queried
		 * to get this information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the timestamp is populated by the acquisition engine
		 * payload leader. The device itself is not queried as this data is
		 * present in the image data. If the image was created by the image
		 * factory, the timestamp is populated by the arguments.
		 *
		 * The image factory can create an image from another image
		 * (Arena::IImage) or from a minimal set of parameters (data, width,
		 * height, pixel format). If the image was created from parameters, the
		 * timestamp will be set to 0, no matter its original value.
		 *
		 * This is the same as the nanosecond timestamp call
		 * (Arena::IImage::GetTimestampNs).
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 *  - Arena::IImage
		 *  - Arena::IImage::GetTimestampNs
		 */
		virtual uint64_t GetTimestamp() = 0;

		/**
		 * @fn virtual uint64_t GetTimestampNs()
		 *
		 * @return 
		 *  - Type: uint64_t
		 *  - Unit: nanoseconds
		 *  - Timestamp of the image in nanoseconds
		 *
		 * <B> GetTimestampNs </B> gets the timestamp of the image in
		 * nanoseconds. Images are self-describing, so the device does not need
		 * to be queried to get this information.
		 *
		 * Images are either retrieved from a device (Arena::IDevice) or created
		 * by the image factory (Arena::ImageFactory). If the image was retrieved
		 * from a device, the timestamp is populated by the acquisition engine
		 * payload leader. The device itself is not queried as this data is
		 * present in the image data. If the image was created by the image
		 * factory, the timestamp is populated by the arguments.
		 *
		 * The image factory can create an image from another image
		 * (Arena::IImage) or from a minimal set of parameters (data, width,
		 * height, pixel format). If the image was created from parameters, the
		 * timestamp will be set to 0, no matter its original value.
		 *
		 * This is the same as the general timestamp call
		 * (Arena::IImage::GetTimestamp).
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory
		 *  - Arena::IImage
		 *  - Arena::IImage::GetTimestamp
		 */
		virtual uint64_t GetTimestampNs() = 0;

		/**
		 * @fn virtual const uint8_t* GetData()
		 *
		 * @return 
		 *  - Type: const uint8_t*
		 *  - Pointer to the payload data
		 *
		 * <B> GetData </B> returns a pointer to the beginning of the image's
		 * payload data. The payload may include chunk data.
		 *
		 * The returned data only includes payload data, not transport layer
		 * protocol leaders, which is handled internally. The pointer can be used
		 * in conjunction with size getters (Arena::IBuffer::GetSizeFilled) to
		 * read, process, and pass the data around. The data may include image
		 * data
		 * (Arena::IBuffer::HasImageData), chunk data (Arena::IBuffer::HasChunkData),
		 * or both.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer::GetSizeFilled
		 *  - Arena::IBuffer::HasImageData
		 *  - Arena::IBuffer::HasChunkData
		 */
		virtual const uint8_t* GetData() = 0;

		/**
		 * @fn virtual size_t GetSizeFilled()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: bytes
		 *  - Size of the payload data
		 *
		 * <B> GetSizeFilled </B> retrieves the size of the data of a buffer,
		 * excluding transport layer protocol leaders.
		 *
		 * The size filled is often same as the size of the buffer
		 * (Arena::IBuffer::GetSizeOfBuffer), but not because they are one and the
		 * same. <B> GetSizeFilled </B> returns the number of bytes received
		 * whereas <B> GetSizeOfBuffer </B> returns the size of the buffer, which
		 * can either be allocated by the user or calculated by Arena
		 * (Arena::IDevice::GetNodeMap, 'PayloadSize').
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer::GetSizeOfBuffer
		 *  - Arena::IDevice::GetNodeMap
		 */
		virtual size_t GetSizeFilled() = 0;

		/**
		 * @fn virtual size_t GetPayloadSize()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: bytes
		 *  - Size of the intended payload
		 *
		 * <B> GetPayloadSize </B> retrieves the intended size of the payload.
		 * This is similar to the retrieved payload size
		 * (Arena::IBuffer::GetSizeFilled), but different in that missed data is
		 * included. This returns the same as the SFNC feature by the same name
		 * ('PayloadSize').
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
		 *
		 * @see 
		 *  - Arena::IBuffer::GetSizeFilled
		 */
		virtual size_t GetPayloadSize() = 0;

		/**
		 * @fn virtual size_t GetSizeOfBuffer()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: bytes
		 *  - Size of the buffer
		 *
		 * <B> GetSizeOfBuffer </B> retrieves the size of the buffer.
		 *
		 * The size filled is often same as the size of the buffer
		 * (Arena::IBuffer::GetSizeOfBuffer), but not because they are one and the
		 * same. <B> GetSizeFilled </B> returns the number of bytes received
		 * whereas <B> GetSizeOfBuffer </B> returns the size of the buffer, which
		 * can either be allocated by the user or calculated by Arena
		 * (Arena::IDevice::GetNodeMap, 'PayloadSize').
		 *
		 * The payload size is calculated at the beginning of the stream and
		 * cannot be recalculated until the stream has stopped. Because of this,
		 * features that can affect payload size ('Width', 'Height',
		 * 'PixelFormat') become unwritable when the stream has started.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer::GetSizeOfBuffer
		 *  - Arena::IDevice::GetNodeMap
		 */
		virtual size_t GetSizeOfBuffer() = 0;

		/**
		 * @fn virtual uint64_t GetFrameId()
		 *
		 * @return 
		 *  - Type: uint64_t
		 *  - Frame ID of the image
		 *
		 * <B> GetFrameId </B> gets the frame ID, a sequential identifier for
		 * buffers.
		 *
		 * Frame IDs start at '1' and continue until either 65535 (16-bit) or
		 * 2^64-1
		 * (64-bit), at which point they roll over back to '1'. The frame ID should
		 * never be '0'. In order to use 64-bit frame IDs, the device must
		 * support GigE Vision 2.0. Simply enable the extended ID mode feature
		 * ('GevGVSPExtendedIDMode').
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 */
		virtual uint64_t GetFrameId() = 0;

		/**
		 * @fn virtual size_t GetPayloadType()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Represents: enum Arena::EBufferPayloadType
		 *  - Type of payload data
		 *
		 * <B> GetPayloadType </B> returns a buffer's payload type
		 * (Arena::EBufferPayloadType), as defined in the GigE Vision specification.
		 *
		 * The payload type indicates how to interpret the data stored in the
		 * buffer
		 * (Arena::IBuffer::GetData). Lucid devices may provide three ways to
		 * interpret the data:
		 *  - as an image (Arena::EBufferPayloadType::BufferPayloadTypeImage)
		 *  - as an image with chunk data appended to the end
		 *    (Arena::EBufferPayloadType::BufferPayloadTypeImageExtended) 
		 *  - as chunk data, which may or may not include image data as a chunk
		 *  - (Arena::EBufferPayloadType::BufferPayloadTypeChunkData)
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer::GetData
		 *  - Arena::EBufferPayloadType
		 */
		virtual size_t GetPayloadType() = 0;

		/**
		 * @fn virtual bool HasImageData()
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if the payload has image data
		 *  - False if the payload has image packaged as chunk data
		 *  - Otherwise, false
		 *
		 * <B> HasImageData </B> returns whether or not a payload can be
		 * interpreted as image data. Calling <B> HasImageData </B> from an image
		 * returns true.
		 *
		 * <B> HasImageData </B> returns true if the payload type is:
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImage
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImageExtendedChunk
		 *
		 * It returns false if the payload type is:
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeChunkData
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::EBufferPayloadType
		 */
		virtual bool HasImageData() = 0;

		/**
		 * @fn virtual bool HasChunkData()
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if the payload has chunk data
		 *  - Otherwise, false
		 *
		 * <B> HasChunkData </B> returns whether or not a payload can be
		 * interpreted as chunk data.
		 *
		 * <B> HasChunkData </B> returns true if the payload type is:
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeChunkData
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImageExtendedChunk
		 *
		 * It returns false if the payload type is:
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImage
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::EBufferPayloadType
		 */
		virtual bool HasChunkData() = 0;

		/**
		 * virtual IChunkData* AsChunkData()
		 *
		 * @return 
		 * - Type: Arena::IChunkData 
		 * - Pointer to the original object cast to chunk data
		 * - Null on failure
		 *
		 * <B> AsChunkData </B> casts the image to chunk data
		 * (Arena::IChunkData). This is only possible if the payload contains
		 * chunk data.
		 *
		 * @warning 
		 * - Causes undefined behaviour if buffer requeued 
		 *
		 * @see 
		 * - Arena::IChunkData 
		 * - Arena::IBuffer::HasChunkData
		 */
		virtual IChunkData* AsChunkData() = 0;

		/**
		 * @fn virtual bool IsIncomplete()
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if the data is incomplete
		 *  - Otherwise, false
		 *
		 * <B> IsIncomplete </B> returns whether or not a buffer's payload data
		 * is complete.
		 *
		 * Error handling may be required in the case that the data is
		 * incomplete. An incomplete image signifies that the data size
		 * (Arena::IBuffer::GetSizeFilled) does not match the expected data size
		 * ('PayloadSize'). This is either due to missed packets or a small buffer.
		 *
		 * The number of missed packets may be discovered through the stream node
		 * map
		 * (Arena::IDevice::GetTLStreamNodeMap). The missed packet count feature
		 * ('StreamMissedPacketCount') is a cumulative count of all missed packets,
		 * and does not necessarily reflect the number of missed packets for any
		 * given buffer.
		 *
		 * A buffer may be missing data if the buffer to hold the data is too
		 * small. This happens when the size of the buffer
		 * (Arena::IBuffer::GetSizeOfBuffer) does not match the expected data
		 * size ('PayloadSize'). This function will also return true when
		 * checking whether the data is larger than the buffer
		 * (Arena::IBuffer::DataLargerThanBuffer).
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer::GetSizeFilled
		 *  - Arena::IDevice::GetTLStreamNodeMap
		 *  - Arena::IBuffer::GetSizeOfBuffer
		 *  - Arena::IBuffer::DataLargerThanBuffer
		 */
		virtual bool IsIncomplete() = 0;

		/**
		 * @fn virtual bool DataLargerThanBuffer()
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if the payload is larger than the buffer
		 *  - Otherwise, false
		 *
		 * <B> DataLargerThanBuffer </B> returns whether or not a buffer's
		 * payload data is too larger for the buffer.
		 *
		 * A buffer may be missing data if the buffer to hold the data is too
		 * small. This happens when the size of the buffer
		 * (Arena::IBuffer::GetSizeOfBuffer) does not match the expected data
		 * size ('PayloadSize'). This function will also return true when
		 * checking whether the data is larger than the buffer.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer::GetSizeOfBuffer
		 */
		virtual bool DataLargerThanBuffer() = 0;

		/**
		 * @fn virtual bool VerifyCRC()
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if the calculated CRC value equals the one sent from the
		 *    device
		 *  - Otherwise, false
		 *
		 * <B> VerifyCRC </B> calculates the CRC of an image's data and verifies
		 * it against the CRC value sent from the device. This helps verify that
		 * no data has been changed or missed during transmission. This function
		 * calls a global helper function to calculate the CRC
		 * (Arena::CalculateCRC32).
		 *
		 * A CRC is performed by running a set of calculations on a dataset both
		 * before and after a transmission. The two calculated values are then
		 * compared for equality. If the values are the same, then the
		 * transmission is deemed successful; if different, then something in the
		 * transmission went wrong.
		 *
		 * A device can be set to send a CRC value by enabling its chunk data
		 * setting.
		 *
		 * \code{.cpp}
		 * 	// Enable chunk data and the CRC chunk
		 * 	{
		 * 		GenApi::INodeMap* pNodeMap = pDevice->GetNodeMap();
		 * 		
		 * 		GenApi::CBooleanPtr pChunkModeActive = pNodeMap->GetNode("ChunkModeActive");
		 * 		pChunkModeActive->SetValue(true);
		 * 		
		 * 		GenApi::CEnumerationPtr pChunkSelector = pNodeMap->GetNode("ChunkSelector");
		 * 		GenApi::CEnumEntryPtr pCRC = pChunkSelector->GetEntryByname("CRC");
		 * 		pChunkSelector->SetIntValue(pCRC->GetValue());
		 * 		
		 * 		GenApi::CBooleanPtr pChunkEnable = pNodeMap->GetNode("ChunkEnable");
		 * 		pChunkEnable->SetValue(true);
		 * 	}
		 * \endcode
		 *
		 * The data can then be checked by verifying the CRC.
		 *
		 * \code{.cpp}
		 * 	// Verifying a buffer's data
		 * 	{
		 * 		Arena::IImage* pImage = pDevice->GetImage(timeout);
		 * 		if (!pImage->VerifyCRC())
		 * 		{
		 * 			// data not complete
		 * 		}
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - May throw GenICam::GenericException or other derived exception
		 *  - Throws if chunk data disabled or not present, or CRC chunk
		 *    disabled
		 *
		 * @see 
		 *  - Arena::CalculateCRC
		 *  - Arena::IImage
		 */
		virtual bool VerifyCRC() = 0;

		/**
		 * @fn virtual ~IImage()
		 *
		 * A destructor
		 */
		virtual ~IImage(){};

	protected:
		virtual IImage* AsImage() = 0;
		IImage(){};
	};
}