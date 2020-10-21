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

#include "ArenaDefs.h"

namespace Arena
{
	/**
	 * @class ImageFactory
	 *
	 * <B> ImageFactory </B> is a static class responsible for the creation,
	 * copying, conversion, and destruction of images (Arena::IImage).
	 *
	 * The image factory allocates and deallocates memory for its images. Memory
	 * is allocated when an image is created (Arena::ImageFactory::Create) or
	 * converted (Arena::ImageFactory::Convert). To clean up memory, all images
	 * created by the image factory must be destroyed
	 * (Arena::ImageFactory::Destroy).
	 *
	 * Images from the image factory are treated noticeably different from those
	 * from a device (Arena::IDevice). Retrieving an image from a device grabs a
	 * buffer that had its memory preallocated when the device started streaming;
	 * retrieving and requeuing does not allocate or deallocate memory, but
	 * simply moves buffers around the acquisition engine. Creating, copying, and
	 * converting an image with the image factory allocates and deallocates
	 * memory as needed. This is why images from a device must be requeued
	 * (Arena::IDevice::RequeueBuffer) while images from the image factory must be
	 * destroyed (Arena::ImageFactory::Destroy).
	 *
	 * @warning 
	 *  - Images from the image factory must be destroyed
	 *  - Images from a device must be requeued
	 *
	 * @see 
	 *  - Arena::IImage
	 *  - Arena::IDevice
	 */
	class ARENA_API ImageFactory
	{
	public:

		/**
		 * @fn static IImage* Create(const uint8_t* pData, size_t dataSize, size_t width, size_t height, uint64_t pixelFormat)
		 *
		 * @param pData
		 *  - Type: const uint8_t
		 *  - Pointer to the beginning of the payload data
		 *
		 * @param dataSize
		 *  - Type: size_t
		 *  - Size of the data
		 *
		 * @param width
		 *  - Type: size_t
		 *  - Unit: pixels
		 *  - Width of the image to create
		 *
		 * @param height
		 *  - Type: size_t
		 *  - Unit: pixels
		 *  - Height of the image to create
		 *
		 * @param pixelFormat
		 *  - Type: uint64_t
		 *  - Represents: enum PfncFormat
		 *  - Pixel format of the image to create
		 *
		 * @return 
		 *  - Type: Arena::IImage*
		 *  - Pointer to the image created from the parameters
		 *
		 * <B> Create </B> creates an image (Arena::IImage) from a minimal set of
		 * parameters. Images created with the image factory must be destroyed
		 * (Arena::ImageFactory::Destroy) when no longer needed.
		 *
		 * <B> Create </B> can create images from any raw image data. It has been
		 * designed to be generic in order to integrate image data from a variety
		 * of sources.
		 *
		 * When creating an image, the image factory allocates memory for the new
		 * image. As such, images created by the image factory must be destroyed;
		 * otherwise, memory will leak.
		 *
		 * \code{.cpp}
		 * 	// creating and destroying an image
		 * 	{
		 * 		IImage* pCreate = Arena::ImageFactory::Create(pData, width, height, pixelFormat);
		 * 		// ...
		 * 		Arena::ImageFactory::Destroy(pCreate);
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - Images from the image factory must be destroyed
		 *  - Images from a device must be requeued
		 *  - Incorrect data size may result in application crash
		 *
		 * @see 
		 *  - Arena::IImage
		 *  - Arena::ImageFactory::Destroy
		 *  - Arena::ImageFactory::Create
		 */
		static IImage* Create(const uint8_t* pData, size_t dataSize, size_t width, size_t height, uint64_t pixelFormat);

		/**
		 * @fn static IImage* Copy(IImage* pBuffer)
		 *
		 * @param pBuffer
		 *  - Type: Arena::IImage*
		 *  - Pointer to the image to copy
		 *
		 * @return 
		 *  - Type: Arena::IImage*
		 *  - Pointer to a deep copy of the image
		 *
		 * Creates a deep copy of an image
		 *
		 * <B> Copy </B> creates a deep copy of an image (Arena::IImage) from
		 * another image. Images created with the image factory must be destroyed
		 * (Arena::ImageFactory::Destroy) when no longer needed.
		 *
		 * When copying an image, the image factory allocates memory for the new
		 * image. As such, images created by copying an image with the image
		 * factory must be destroyed; otherwise, memory will leak.
		 *
		 * \code{.cpp}
		 * 	// creating and destroying an image
		 * 	{
		 * 		IImage* pCopy = Arena::ImageFactory::Copy(pImage);
		 * 		// ...
		 * 		Arena::ImageFactory::Destroy(pCopy);
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - Images from the image factory must be destroyed
		 *  - Images from a device should be requeued
		 *  - Instantiates all lazy properties of the original image
		 *
		 * @see 
		 *  - Arena::IImage
		 *  - Arena::ImageFactory::Destroy
		 *  - Arena::ImageFactory::Copy
		 */
		static IImage* Copy(IImage* pBuffer);

		/**
		 * @fn static IImage* Convert(IImage* pImage, uint64_t pixelFormat, EBayerAlgorithm bayerAlgorithm = DirectionalInterpolation)
		 *
		 * @param pImage
		 *  - Type: Arena::IImage
		 *  - Pointer to the image to convert
		 *
		 * @param pixelFormat
		 *  - Type: uint64_t
		 *  - Represents: enum PfncFormat
		 *  - Pixel format to convert to
		 *
		 * @param bayerAlgorithm
		 *  - Type: Arena::EBayerAlgorithm
		 *  - Bayer conversion algorithm to use
		 *  - Only applicable when converting from bayer
		 *
		 * @return 
		 *  - Type: Arena::IImage*
		 *  - Pointer to the converted image
		 *
		 * <B> Convert </B> converts an image (Arena::IImage) to a select pixel
		 * format. In doing so, it creates a completely new image, similar to a
		 * deep copy but with a different pixel format. Images created with the
		 * image factory must be destroyed (Arena::ImageFactory::Destroy) when no
		 * longer needed; otherwise, memory will leak.
		 *
		 * \code{.cpp}
		 * 	// creating and destroying an image
		 * 	{
		 * 		Arena::IImage* pConvert = Arena::ImageFactory::Convert(pImage, BGRa8);
		 * 		// ...
		 * 		Arena::ImageFactory::Destroy(pConvert);
		 * 	}
		 * \endcode
		 *
		 * The list of supported pixel formats can be found in the software node
		 * map
		 * (Arena::ISystem::GetNodeMap, 'SupportedPixelFormats'). The list of
		 * supported conversion pixel formats is difference from a device's pixel
		 * formats (Arena::IDevice::GetNodeMap, 'PixelFormat'). In order for
		 * conversion to succeed, both the source and destination pixel formats
		 * must be supported. Bayer formats are supported as source formats only.
		 *
		 * @warning 
		 *  - Images from the image factory must be destroyed
		 *  - Images from a device should be requeued
		 *  - Cannot convert to bayer formats
		 *  - Bayer conversion algorithm only necessary when converting from
		 *    bayer formats
		 *
		 * @see 
		 *  - Arena::IImage
		 *  - Arena::ImageFactory::Destroy
		 *  - Arena::ImageFactory::Convert
		 *  - Arena::ISystem::GetNodeMap
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::EBayerAlgorithm
		 */
		static IImage* Convert(IImage* pImage, uint64_t pixelFormat, EBayerAlgorithm bayerAlgorithm = DirectionalInterpolation);

		/**
		 * @fn static void Destroy(IImage* pImage)
		 *
		 * @param pImage
		 *  - Type: Arena::IImage*
		 *  - Pointer to the image to destroy
		 *  - Image must be from image factory
		 *
		 * @return 
		 *  - none
		 *
		 * <B> Destroy </B> cleans up an image (Arena::IImage) and deallocates
		 * its memory. It must be called on any image created by the image
		 * factory
		 * (Arena::ImageFactory::Create, Arena::ImageFactory::Copy,
		 * Arena::ImageFactory::Convert).
		 *
		 * All images from the image factory, whether created
		 * (Arena::ImageFactory::Create), copied (Arena::ImageFactory::Copy), or
		 * converted (Arena::ImageFactory::Convert), must be destroyed to
		 * deallocate their memory; otherwise, memory will leak. It is important
		 * that <B> Destroy </B> only be called on images from the image factory,
		 * and not on those retrieved from a device (Arena::IDevice).
		 *
		 * @warning 
		 *  - Images from the image factory must be destroyed
		 *  - Images from a device should be requeued
		 *
		 * @see 
		 *  - Arena::IImage
		 *  - Arena::IDevice
		 *  - Arena::ImageFactory::Create
		 *  - Arena::ImageFactory::Copy
		 *  - Arena::ImageFactory::Convert
		 */
		static void Destroy(IImage* pImage);

	private:

		// static class
		// functions inaccessible
		ImageFactory();
	};
}