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

#include <cstdint>
#include "SaveDefs.h"

namespace Save
{
	/**
	 * @class ImageReader
	 *
	 * The image reader facilitates the loading of image data from files.
	 */
	class SAVE_API ImageReader
	{
	public:
		/**
		 * @fn ImageReader(const char* pFileName, bool topToBottom = true)
		 *
		 * @param pFileName
		 *  - Type: const char*
		 *  - Name of the file to load
		 *
		 * @param topToBottom
		 *  - Type: Type: bool
		 *  - Default: true
		 *  - If true, read the file from top to bottom
		 *  - Otherwise, from bottom to top
		 *
		 * A constructor. Internally, it loads an image
		 * (Save::ImageReader::Load).
		 */
		ImageReader(const char* pFileName, bool topToBottom = true);
#ifdef _WIN32
		ImageReader(const wchar_t* pFileName, bool topToBottom = true);
#endif

		/**
		 * @fn ImageReader(const ImageReader& imageReader)
		 *
		 * @param imageReader
		 *  - Type: const Save::ImageReader&
		 *  - Image reader to copy
		 *
		 * A copy constructor.
		 */
		ImageReader(const ImageReader& imageReader);

		/**
		 * @fn ImageReader& operator=(ImageReader imageReader)
		 *
		 * @param imageReader
		 *  - Type: ImageReader
		 *  - Image reader to copy
		 *
		 * @return 
		 *  - Type: ImageReader&
		 *  - Copied image reader
		 *
		 * A copy assignment operator.
		 */
		ImageReader& operator=(ImageReader imageReader);

		/**
		 * @fn virtual ~ImageReader()
		 *
		 * A destructor.
		 */
		virtual ~ImageReader();

		/**
		 * @fn virtual void Load(const char* pFileName, bool topToBottom = true)
		 *
		 * @param pFileName
		 *  - Type: const char*
		 *  - Name of the file to load
		 *
		 * @return 
		 *  - none
		 *
		 * <B> Load </B> loads image data from a file to memory. After loading an
		 * image, the image parameters can be retrieved
		 * (Save::ImageReader::GetParams) or the image data
		 * (Save::ImageReader::GetData).
		 */
		virtual void Load(const char* pFileName, bool topToBottom = true);
#ifdef _WIN32
		virtual void Load(const wchar_t* pFileName, bool topToBottom = true);
#endif

		/**
		 * @fn virtual ImageParams GetParams() const
		 *
		 * @return 
		 *  - Type: Save::ImageParams
		 *  - Parameters of a loaded image
		 *
		 * <B> GetParams </B> retrieves the parameters of a loaded image.
		 */
		virtual ImageParams GetParams() const;

		/**
		 * @fn virtual const uint8_t* GetData() const
		 *
		 * @return 
		 *  - Type: const uint8_t*
		 *  - Data of a loaded image
		 *
		 * <B> GetData </B> retrieves the data of a loaded image.
		 */
		virtual const uint8_t* GetData() const;

		/**
		 * @fn static void LoadRawData(const char* pFileName, uint8_t* pImageData, const size_t size)
		 *
		 * @param pFileName
		 *  - Type: const char*
		 *  - Name of the file to load
		 *
		 * @param pImageData
		 *  - Type: uint8_t*
		 *  - Out parameter
		 *  - Requires a preallocated buffer
		 *  - Returns data from the loaded file
		 *
		 * @param size
		 *  - Type: const size_t
		 *  - Size of the file to read
		 *
		 * <B> LoadRawData </B> is a static function that loads the raw data of a
		 * file.
		 *
		 * @warning 
		 *  - Reads files top to bottom
		 */
		static void LoadRawData(const char* pFileName, uint8_t* pImageData, const size_t size);
#ifdef _WIN32
		static void LoadRawData(const wchar_t* pFileName, uint8_t* pImageData, const size_t size);
#endif

	private:
		ImageReader();
		void* m_pInternal;
	};
}
