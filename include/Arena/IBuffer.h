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

namespace Arena
{
	class IImage;
	class IChunkData;

	/**
	 * @fn int64_t CalculateCRC32(const uint8_t* pData, size_t nBytes)
	 *
	 * @param pData
	 *  - Type: const uint8_t
	 *  - A pointer to the data to use to calculate the CRC
	 *
	 * @param nBytes
	 *  - Type: size_t
	 *  - The size of the data
	 *
	 * @return 
	 *  - Type: int64_t
	 *  - The calculated CRC value
	 *
	 * <B> CalculateCRC32 </B> calculates a CRC value (cyclical redundancy check)
	 * on a dataset. This is used to check whether the dataset has been sent in
	 * its entirety.
	 *
	 * A CRC is performed by running a set of calculations on a dataset both
	 * before and after a transmission. The two calculated values are then
	 * compared for equality. If the values are the same, then the transmission
	 * is deemed successful; if different, then something in the transmission
	 * went wrong.
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
	 * The CRC value can then be retrieved from a device.
	 *
	 * \code{.cpp}
	 * 	// Retrieving CRC from chunk data, calculating it on the data, and comparing the two
	 * 	{
	 * 		Arena::IChunkData* pChunkData = pImage->AsChunkData();
	 * 		GenApi::CIntegerPtr pChunkCRC = pChunkData->GetChunk("ChunkCRC");
	 * 		int32_t crcFromDevice = pChunkCRC->GetValue();
	 * 		
	 * 		int32_t crcCalculated = CalculateCRC32(pImage->GetData(), pImage->SizeFilled());
	 * 		if (crcFromDevice == crcCalculated)
	 * 		{
	 * 			// Image data is complete
	 * 		}
	 * 	}
	 * \endcode
	 *
	 * @warning 
	 *  - May throw GenICam::GenericException or other derived exception
	 *
	 * @see 
	 *  - Arena::IChunkData
	 */
	ARENA_API int64_t CalculateCRC32(const uint8_t* pData, size_t nBytes);

	/**
	 * @class IBuffer
	 *
	 * An interface to a buffer object
	 *
	 * Buffers are the most generic form of acquisition engine data retrieved
	 * from a device. They are acquired and requeued via devices
	 * (Arena::IDevice).
	 *
	 * \code{.cpp}
	 * 	// retrieving a buffer after starting the stream
	 * 	// requeuing it before stopping the stream
	 * 	{
	 * 		pDevice->StartStream();
	 * 		Arena::IBuffer* pBuffer = pDevice->GetBuffer(2000);
	 * 		// ...
	 * 		pDevice->RequeueBuffer(pBuffer);
	 * 		pDevice->StopStream();
	 * 	}
	 * \endcode
	 *
	 * Buffers are the base class of images (Arena::IImage) and chunk data
	 * (Arena::IChunkData). Information shared between both can be accessed in the
	 * buffer class:
	 *  - buffer and payload information like payload
	 *    (Arena::IBuffer::GetData), payload and buffer size
	 *    (Arena::IBuffer::GetSizeFilled, Arena::IBuffer::GetSizeOfBuffer), and
	 *    frame ID
	 *    (Arena::IBuffer::GetFrameID) 
	 *  - type information like payload type (Arena::IBuffer::GetPayloadType)
	 *    and whether the payload has image and/or chunk data
	 *    (Arena::IBuffer::HasImageData, Arena::IBuffer::HasChunkData) 
	 *  - casting helpers (Arena::IBuffer::AsImage,
	 *    Arena::IBuffer::AsChunkData)
	 *  - error information (Arena::IBuffer::IsIncomplete,
	 *  - Arena::IBuffer::DataLargerThanBuffer)
	 *
	 * Retrieving data as a buffer can be preferable to an image or chunk data,
	 * as it can allow data to be received and treated generically before further
	 * processing.
	 *
	 * \code{.cpp}
	 * 	// processing image and chunk data only if they exist
	 * 	{
	 * 		Arena::IBuffer* pBuffer = pDevice->GetBuffer(2000);
	 * 		if (pBuffer->HasImageData())
	 * 		{
	 * 			Arena::IImage* pImage = pBuffer->AsImage();
	 * 			// ...
	 * 		}
	 * 		if (pBuffer->HasChunkData())
	 * 		{
	 * 			Arena::IChunkData* pChunkData = pBuffer->AsChunkData();
	 * 			// ...
	 * 		}
	 * 		pDevice->RequeueBuffer(pBuffer);
	 * 	}
	 * \endcode
	 *
	 * @warning 
	 *  - Should be requeued; otherwise, acquisition engine may starve
	 *  - Properties are lazily instantiated from acquisition engine
	 *
	 * @see 
	 *  - Arena::IDevice
	 *  - Arena::IBuffer
	 *  - Arena::IImage
	 *  - Arena::IChunkData
	 */
	class ARENA_API IBuffer
	{
	public:

		/**
		 * @fn virtual const uint8_t* GetData()
		 *
		 * @return 
		 *  - Type: const uint8_t*
		 *  - Pointer to the beginning of the payload data
		 *
		 * <B> GetData </B> retrieves a pointer to the buffer's payload data.
		 * This data may include image data, chunk data, or both.
		 *
		 * To check the type of data returned, image
		 * (Arena::IBuffer::HasImageData) and chunk data
		 * (Arena::IBuffer::HasChunkData) can be checked for specifically, or the
		 * payload type (Arena::EBufferPayloadType) can be retrieved.
		 *
		 * The returned data only includes payload data, not transport layer
		 * protocol leaders, which is handled internally. The pointer can be used
		 * in conjunction with size getters (Arena::IBuffer::GetSizeFilled) to
		 * read, process, and pass the data around.
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer::HasImageData
		 *  - Arena::IBuffer::HasChunkData
		 *  - Arena::IBuffer::GetSizeFilled
		 *  - Arena::EBufferPayloadType
		 */
		virtual const uint8_t* GetData() = 0;

		/**
		 * @fn virtual size_t GetSizeFilled()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: bytes
		 *  - Size of the received payload
		 *
		 * <B> GetSizeFilled </B> retrieves the size of the payload data,
		 * excluding transport layer protocol leaders. The payload data may
		 * include image data, chunk data, or both.
		 *
		 * The size filled is often same as the size of the buffer
		 * (Arena::IBuffer::GetSizeOfBuffer), but not because they are one and the
		 * same. <B> GetSizeFilled </B> returns the number of bytes received
		 * whereas <B> GetSizeOfBuffer </B> returns the size of the buffer, which
		 * can either be allocated by the user or calculated by Arena
		 * (Arena::IDevice::GetNodeMap, 'PayloadSize').
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
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
		 * <B> GetSizeOfBuffer </B> retrieves the size of a buffer.
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
		 *  - Causes undefined behavior if buffer requeued
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
		 *  - Frame ID
		 *
		 * <B> GetFrameId </B> gets the frame ID, a sequential identifier for
		 * buffers.
		 *
		 * Frame IDs start at '1' and continue until either 65535 (16-bit) or
		 * 2^64-1
		 * (64-bit), at which point they roll over back to '1'. The frame ID should
		 * never be '0'. In order to use 64-bit frame IDs, the device must
		 * support GigE Vision 2.0. Simply enable the extended ID mode feature
		 * (Arena::IDevice::GetNodeMap, 'GevGVSPExtendedIDMode').
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice::GetNodeMap
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
		 * (Arena::EBufferPayloadType), as defined by the GigE Vision specification.
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
		 *  - as chunk data, which may or may not include image data as a chunk
		 *    (Arena::EBufferPayloadType::BufferPayloadTypeChunkData)
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
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
		 *  - False if the payload has image data packaged as chunk data
		 *  - Otherwise, false
		 *
		 * <B> HasImageData </B> returns whether or not a buffer's payload has
		 * data that may be interpreted as image data.
		 *
		 * <B> HasImageData </B> returns true if the payload type is:
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImage
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImageExtendedChunk
		 *
		 * It returns false if the payload type is:
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeChunkData
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *  - Returns false if image data is packaged as chunk data
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
		 * <B> HasChunkData </B> returns whether or not a buffer's payload that
		 * may be interpreted as chunk data.
		 *
		 * <B> HasChunkData </B> returns true if the payload type is:
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeChunkData
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImageExtendedChunk
		 *
		 * It returns false if the payload type is:
		 *  - Arena::EBufferPayloadType::BufferPayloadTypeImage
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::EBufferPayloadType
		 */
		virtual bool HasChunkData() = 0;

		/**
		 * @fn virtual IImage* AsImage()
		 *
		 * @return 
		 *  - Type: Arena::IImage*
		 *  - Pointer to the original object cast to an image
		 *  - Null on failure
		 *
		 * <B> AsImage </B> casts the buffer to an image (Arena::IImage). This is
		 * only possible if the payload contains image data.
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
		 *
		 * @see 
		 *  - Arena::IImage
		 */
		virtual IImage* AsImage() = 0;

		/**
		 * @fn virtual IChunkData* AsChunkData()
		 *
		 * @return 
		 *  - Type: Arena::IChunkData*
		 *  - Pointer to the original object cast to chunk data
		 *  - Null on failure
		 *
		 * <B> AsChunkData </B> casts the buffer to a chunk data
		 * (Arena::IChunkData). This is only possible if the payload contains
		 * chunk data.
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
		 *
		 * @see 
		 *  - Arena::IChunkData
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
		 * <B> IsIncomplete </B> returns whether or not the payload is complete.
		 *
		 * Error handling may be required in the case that the data is
		 * incomplete. An incomplete image signifies that the data size
		 * (Arena::IBuffer::GetSizeFilled) does not match the expected data size
		 * (Arena::IDevice::GetTLStreamNodeMap, 'PayloadSize'). This is either due to
		 * missed packets or a small buffer.
		 *
		 * The number of missed packets may be discovered through the stream node
		 * map
		 * (Arena::IDevice::GetTLStreamNodeMap). The missed packet count feature
		 * (Arena::IDevice::GetTLStreamNodeMap, 'StreamMissedPacketCount') is a
		 * cumulative count of all missed packets, and does not necessarily
		 * reflect the number of missed packets for any given buffer.
		 *
		 * A buffer may be missing data if the buffer to hold the data is too
		 * small. This happens when the size of the buffer
		 * (Arena::IBuffer::GetSizeOfBuffer) does not match the expected data
		 * size (Arena::IDevice::GetTLStreamNodeMap, 'PayloadSize'). This
		 * function will also return true when checking whether the data is
		 * larger than the buffer (Arena::IBuffer::DataLargerThanBuffer).
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
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
		 * size (Arena::IDevice::GetTLStreamNodeMap, 'PayloadSize'). This
		 * function will also return true when checking whether the data is
		 * larger than the buffer.
		 *
		 * @warning 
		 *  - Causes undefined behavior if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer::GetSizeOfBuffer
		 *  - Arena::IDevice::GetTLStreamNodeMap
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
		 * <B> VerifyCRC </B> calculates the CRC of a buffer's data and verifies
		 * it against the CRC value sent from the device. This helps verify that
		 * no data has been changed or missed during a transmission. This
		 * function calls a global helper function to calculate the CRC
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
		 * 		Arena::IBuffer* pBuffer = pDevice->GetBuffer(timeout);
		 * 		if (!pBuffer->VerifyCRC())
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
		 *  - Arena::IBuffer
		 */
		virtual bool VerifyCRC() = 0;

		/**
		 * @fn virtual ~IBuffer()
		 *
		 * A destructor
		 */
		virtual ~IBuffer(){};

	protected:

		IBuffer(){};
	};
}