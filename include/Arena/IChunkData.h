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
	class IImage;

	/**
	 * @class IChunkData
	 *
	 * An interface to a chunk data object
	 *
	 * The chunk data interface helps read and interpret chunk data. It inherits
	 * from the buffer interface (Arena::IBuffer). If chunk data is present,
	 * buffers (Arena::IBuffer) and images (Arena::IImage) may be cast to chunk
	 * data.
	 *
	 * \code{.cpp}
	 * 	// retrieving a buffer, checking for and casting to chunk data
	 * 	{
	 * 		Arena::IBuffer* pBuffer = pDevice->GetBuffer(100);
	 * 		
	 * 		if (pBuffer->HasChunkData())
	 * 		{
	 * 			Arena::IChunkData* pChunkData = pBuffer->AsChunkData();
	 * 			// ...
	 * 		}
	 * 	}
	 * \endcode
	 *
	 * The concept of chunk data is a method of adding extra data (such as CRC,
	 * width, height, etc.) to an image. A nuance of this concept is whether the
	 * additional information is appended to the back of the image or the image
	 * is treated as part of the chunk data. This is important for parsing the
	 * data. Lucid devices create chunk data by appending it to the payload.
	 *
	 * In order to receive chunk data with an image, chunk data must be enabled
	 * and configured on node map (GenApi::INodeMap, Arena::IDevice::GetNodeMap).
	 * Chunk data must first be activated ('ChunkModeActive'). Each specific
	 * chunk must then be selected and enabled ('ChunkSelector', 'ChunkEnable').
	 *
	 * \code{.cpp}
	 * 	// enabling exposure time chunk data
	 * 	{
	 * 		CBooleanPtr pChunkModeActive = pNodeMap->GetNode("ChunkModeActive");
	 * 		pChunkModeActive->SetValue(true);
	 * 		
	 * 		CEnumerationPtr pChunkSelector = pNodeMap->GetNode("ChunkSelector");
	 * 		CEnumEntryPtr pPixelFormat = pChunkSelector->GetEntryByName("ExposureTime");
	 * 		pChunkSelector->SetIntValue(pPixelFormat->GetValue());
	 * 		
	 * 		CBooleanPtr pChunkEnable = pNodeMap->GetNode("ChunkEnable");
	 * 		pChunkEnable->SetValue(true);
	 * 	}
	 * \endcode
	 *
	 * Along with the functionality of its parent class (Arena::IBuffer), chunk
	 * data objects provide the ability to get chunks
	 * (Arena::IChunkData::GetChunk).
	 *
	 * @warning 
	 *  - Should be requeued; same as other buffers
	 *  - Properties are lazily instantiated from the acquisition engine
	 *
	 * @see 
	 *  - Arena::IBuffer
	 *  - Arena::IImage
	 *  - Arena::IChunkData
	 */
	class ARENA_API IChunkData : public IBuffer
	{
	public:
		/**
		 * @fn virtual GenApi::INode* GetChunk(GenICam::gcstring name)
		 *
		 * @param name
		 *  - Type: GenICam::gcstring
		 *  - Name of the chunk
		 *  - Prefixed with 'Chunk'
		 *
		 * @return 
		 *  - Type: GenApi::INode*
		 *  - Requested node
		 *  - Null on failure
		 *
		 * <B> GetChunk </B> gets a specified chunk, returning it as a node in
		 * order to preserve metadata related to the chunk.
		 *
		 * Internally, chunk data objects have an internal node map and a chunk
		 * adapter. These allow chunk information to be processed and read as
		 * nodes
		 * (GenApi::INode) using GenICam and GenApi.
		 *
		 * There is a chance that incomplete images have garbage data in place of
		 * expected chunk data. If this is the case, it is still possible to
		 * attempt chunk retrieval. Invalid chunks return as null.
		 *
		 * \code{.cpp}
		 * 	// printing the exposure time from chunk
		 * 	{
		 * 		Arena::IChunkData* pChunkData = pImage->AsChunkData();
		 * 		if (!pChunkData->IsIncomplete())
		 * 		{
		 * 			GenApi::CFloatPtr pChunkExposureTime = pChunkData->GetChunk("ChunkExposureTime");
		 * 			std::cout << "Exposure time: " << pChunkExposureTime->GetValue() << std::endl;
		 * 		}
		 * 	}
		 * \endcode
		 *
		 * Chunk data must meet three criteria to provide relevant data. Chunk
		 * mode must be activated ('ChunkModeActive'), the chunk must be enabled
		 * ('ChunkSelector', 'ChunkEnable'), and the node must exist: 
		 *  - if chunk mode is inactive, the buffer will not contain chunk data
		 *  - if chunk does not exist, returns null
		 *  - if chunk is not enabled, returned node will be unavailable
		 *  - otherwise, returns node successfully
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - Returns null on corrupted or missing data
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IChunkData
		 */
		virtual GenApi::INode* GetChunk(GenICam::gcstring name) = 0;

		/**
		 * @fn virtual size_t GetSizeFilled()
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Unit: bytes
		 *  - Size of the chunk portion of the payload
		 *
		 * A getter for the size of the data
		 *
		 * <B> GetSizeFilled </B> retrieves the size of the data of a buffer,
		 * excluding transport layer protocol leaders. It takes no inputs and
		 * returns the size of the data as output.
		 *
		 * The return value of <B> GetSizeFilled </B> should always be the same
		 * as the return value of <B> GetSizeOfBuffer </B>
		 * (Arena::IBuffer::GetSizeOfBuffer), but not because they are one and the
		 * same. <B> GetSizeFilled </B> returns the size of the data whereas <B>
		 * GetSizeFilled </B> returns the size of the buffer, which is set
		 * according to the expected payload size ('PayloadSize').
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *  - Properties lazily instantiated if buffer retrieved from device
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IBuffer::GetSizeOfBuffer
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
		 *  - Frame ID
		 *
		 * <B> GetFrameId </B> returns the frame ID, a sequential identifier for
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
		 *  - False if the payload has image data packaged as chunk
		 *  - Otherwise, false
		 *
		 * <B> HasImageData </B> returns whether or not a buffer's payload may be
		 * interpreted as image data.
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
		 *  - True if the payload has chunk data
		 *  - Otherwise, false
		 *
		 * <B> HasChunkData </B> returns whether or not a buffer's payload may be
		 * interpreted as chunk data. Calling <B> HasChunkData </B> from chunk
		 * data returns true.
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
		 * @fn virtual IImage* AsImage()
		 *
		 * @return 
		 *  - Type: Arena::IImage*
		 *  - Pointer to the original object cast to an image
		 *  - Null on failure
		 *
		 * <B> AsImage </B> casts the buffer to an image (Arena::IImage). This is
		 * only possible if the payload contains image data not packaged as a
		 * chunk.
		 *
		 * @warning 
		 *  - Causes undefined behaviour if buffer requeued
		 *
		 * @see 
		 *  - Arena::IImage
		 */
		virtual IImage* AsImage() = 0;

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
		 * <B> VerifyCRC </B> calculates the CRC of chunk data and verifies it
		 * against the CRC value sent from the device. This helps verify that no
		 * data has been changed or missed during transmission. This function
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
		 * @fn virtual ~IChunkData()
		 *
		 * A destructor
		 */
		virtual ~IChunkData(){};

	protected:
		IChunkData(){};
		virtual const uint8_t* GetData() = 0;
		virtual IChunkData* AsChunkData() = 0;
	};
} // namespace Arena