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

/**
 * @file ArenaDefs.h
 * This file defines global Arena enums.
 */

#pragma once

// define infinite macro
#define ARENA_INFINITE  0xFFFFFFFFFFFFFFFFULL  /* Infinite value to be used in various function calls. Equals GENTL_INFINITE */

namespace Arena
{
	/**
	 * @typedef ENumBufferFlags
	 *
	 * The <B> ENumBufferFlags </B> predefined number of buffers options for the
	 * stream.
	 *
	 * The enum values and their descriptions:
	 *  - NumBuffersAuto
	 *    - Value: 0xFFFFFFFF
	 *    - Description: Auto calculate numBuffers based on max throughput
	 */
	typedef enum _ENumBufferFlags
	{
		NumBuffersAuto = 0xFFFFFFFF /*!< Auto calculate numBuffers based on max throughput */
	} ENumBufferFlags;

	/**
	 * @typedef EBufferPayloadType
	 *
	 * The <B> EBufferPayloadType </B> enum represents the different types of
	 * GVSP data that can be acquired by the acquisition engine. This enum is
	 * returned from:
	 *  - buffers (Arena::IBuffer::GetPayloadType)
	 *  - images (Arena::IImage::GetPayloadType)
	 *  - chunk data (Arena::IChunkData::GetPayloadType)
	 *
	 * The enum values and their descriptions:
	 *  - BufferPayloadTypeImage
	 *    - Value: 0x0001
	 *    - Description: Image data only
	 *  - BufferPayloadTypeImageExtendedChunk
	 *    - Value: 0x4001
	 *    - Description: Image data extended with chunk data
	 *  - BufferPayloadTypeChunkData
	 *    - Value: 0x0004
	 *    - Description: Chunk data only; image data may be present as chunk
	 *
	 * @see 
	 *  - Arena::IBuffer::GetPayloadType
	 *  - Arena::IImage::GetPayloadType
	 *  - Arena::IChunkData::GetPayloadType
	 */
	typedef enum _EBufferPayloadType
	{
		BufferPayloadTypeImage = 0x0001,			  /*!< Image data only */
		BufferPayloadTypeImageExtendedChunk = 0x4001, /*!< Image data extended with chunk data */
		BufferPayloadTypeChunkData = 0x0004			  /*!< Chunk data only; image data may be present as chunk */
	} EBufferPayloadType;

	/**
	 * @typedef EPixelEndianness
	 *
	 * The <B> EPixelEndianness </B> enum represents the endianness of an image's
	 * multi-byte pixels. This enum is returned from images
	 * (Arena::IImage::GetPixelEndianness).
	 *
	 * The enum values and their descriptions:
	 *  - PixelEndiannessUnknown
	 *    - Value: 0
	 *    - Description: Unknown pixel endianness
	 *  - PixelEndiannessLittle
	 *    - Value: 1
	 *    - Description: Little endian
	 *  - PixelEndiannessBig
	 *    - Value: 2
	 *    - Description: Big endian
	 *
	 * @see 
	 *  - Arena::IImage::GetPixelEndianness
	 */
	typedef enum _EPixelEndianness
	{
		PixelEndiannessUnknown = 0, /*!< Unknown pixel endianness */
		PixelEndiannessLittle = 1,	/*!< Little endian */
		PixelEndiannessBig = 2,		/*!< Big endian */
	} EPixelEndianness;

	/**
	 * @typedef EBayerAlgorithm
	 *
	 * The <B> EBayerAlgorithm </B> enum represents different algorithms for
	 * interpreting bayer patterns. Provide this enum when converting an image
	 * from any bayer pattern (Arena::ImageFactory::Create).
	 *
	 * The enum values and their descriptions
	 *  - DirectionalInterpolation
	 *    - Description: Algorithm that averages nearest neighbours (faster)
	 *  - AdaptiveHomogeneityDirected
	 *    - Description: Adaptive algorithm that uses directional data (slower,
	 *      more accurate coloring)
	 *  - _UndefinedAlgorithm
	 *    - Description: Object not yet initialized
	 *
	 * @see 
	 *  - Arena::ImageFactory::Create
	 */
	typedef enum _EBayerAlgorithm
	{
		DirectionalInterpolation,	 /*!< Algorithm that averages nearest neighbours (faster) */
		AdaptiveHomogeneityDirected, /*!< Adaptive algorithm that uses directional data (slower, more accurate coloring) */
		_UndefinedAlgorithm			 /*!< Undefined algorithm */
	} EBayerAlgorithm;
} // namespace Arena
