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

#include "SaveDefs.h"
#include <cstdint>

namespace Save
{
	class ImageParams;

	/**
	 * @class ImageWriter
	 *
	 * The image writer facilitates the saving of image data to files.
	 */
	class SAVE_API ImageWriter
	{
	public:
		/**
		 * @fn ImageWriter()
		 *
		 * An empty constructor.
		 *
		 * @warning 
		 *  - Must set image parameters before saving
		 */
		ImageWriter();

		/**
		 * @fn ImageWriter(ImageParams imageParams, const char* pFileNamePattern = "savedimages/image<count>.jpg")
		 *
		 * @param imageParams
		 *  - Type: Save::ImageParams
		 *  - The parameters of the image(s) to save
		 *
		 * @param pFileNamePattern
		 *  - Type: const char*
		 *  - File name pattern to use for file names
		 *
		 * A constructor.
		 */
		ImageWriter(ImageParams imageParams, const char* pFileNamePattern = "savedimages/image<count>.jpg");
#ifdef _WIN32
		ImageWriter(ImageParams imageParams, const wchar_t* pFileNamePattern);
#endif

		/**
		 * @fn ImageWriter(const ImageWriter& imageWriter)
		 *
		 * @param imageWriter
		 *  - Type: const Save::ImageWriter&
		 *  - An image writer to copy
		 *
		 * A copy constructor.
		 */
		ImageWriter(const ImageWriter& imageWriter);

		/**
		 * @fn const ImageWriter& operator=(ImageWriter imageWriter)
		 *
		 * @param imageWriter
		 *  - Type: Save::ImageWriter
		 *  - An image writer to copy
		 *
		 * @return 
		 *  - Type: const Save::ImageWriter
		 *  - A copy of an image writer
		 *
		 * A copy assignment operator.
		 */
		const ImageWriter& operator=(ImageWriter imageWriter);

		/**
		 * @fn virtual ~ImageWriter()
		 *
		 * A destructor.
		 */
		virtual ~ImageWriter();

		/**
		 * @fn virtual void SetJpeg(const char* pSetExtension = ".jpg", size_t quality = 75, bool progressive = false, EJpegSubsampling subsampling = Subsampling420, bool optimize = false)
		 *
		 * @param pSetExtension
		 *  - Type: const char*
		 *  - Default: ".jpg"
		 *  - Extension to use for the files
		 *
		 * @param quality
		 *	- Type: size_t
		 *	- Default: 75
		 *	- Range: 1-100
		 *	- Image quality (1 lowest, 100 highest)
		 *
		 * @param progressive
		 *	- Type: bool
		 *	- Default: false
		 *	- If true, saves progressive
		 *	- Otherwise, saves baseline
		 *
		 * @param subsampling
		 *	- Type: EJpegSubsampling
		 *	- Default: Subsampling420
		 *	- The chroma subsampling to apply
		 *
		 * @param optimize
		 *	- Type: bool
		 *	- Default: false
		 *	- If true, calculates optimal Huffman coding tables
		 *	- Otherwise, does not
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetJpeg </B> sets the output file format of the image writer to
		 * JPEG.
		 *
		 * An image may be set to progressive, which encodes and decodes the
		 * image gradually so the entire image becomes sharper as the bytes
		 * arrive. Progressive images tend to have a smaller file size but take
		 * more CPU time and memory to encode and decode them. Otherwise, <B>
		 * SetJpeg </B> loads baseline by default. This encodes and decodes the
		 * image as the data is made available, top to bottom and left to right.
		 * Baseline tends to have a larger file size but takes less time and
		 * memory.
		 *
		 * Subsampling compresses the chroma (color information) of an image
		 * while retaining luminance (brightness). This reduces the bandwidth
		 * used to load an image. For example, setting <B> SetJpeg </B>
		 * subsampling to 4:2:0 (default Subsampling420) reduces the bandwidth by
		 * half with little visual difference compared to 4:4:4 (NoSubsampling).
		 *
		 * \code{.cpp}
		 *  // setting jpeg image options
		 *  {
		 * 		static const std::string sk_pimg = "imageoptions";
		 *
		 *		void SetImageOptions(PluginController& ctrl, std::string extension, Save::ImageWriter* pImgWrt)
		 *		{
		 *			if (extension == ".jpg")
		 *			{
		 *				Save::EJpegSubsampling subsampling = Save::EJpegSubsampling::NoSubsampling;
		 *
		 *				pImgWrt->SetJpeg(extension.c_str(), std::stoi(ctrl.GetValue(sk_pimg, "img_jpeg_quality")), 
		 *				ctrl.GetValue(sk_pimg, "img_jpeg_progressive") == "1", subsampling, ctrl.GetValue(sk_pimg, "img_jpeg_optimize") == "1");
		 *			}
		 *		}
		 *  }
		 * \endcode
		 */
		virtual void SetJpeg(const char* pSetExtension = ".jpg", size_t quality = 75, bool progressive = false, EJpegSubsampling subsampling = Subsampling420, bool optimize = false);
#ifdef _WIN32
		virtual void SetJpeg(const wchar_t* pSetExtension, size_t quality = 75, bool progressive = false, EJpegSubsampling subsampling = Subsampling420, bool optimize = false);
#endif
		/**
		 * @fn virtual void SetBmp(const char* pSetExtension = ".bmp")
		 *
		 * @param pSetExtension
		 *  - Type: const char*
		 *  - Default: ".bmp"
		 *  - Extension to use for saved files
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetBmp </B> sets the output file format of the image writer to
		 * BMP.
		 */
		virtual void SetBmp(const char* pSetExtension = ".bmp");
#ifdef _WIN32
		virtual void SetBmp(const wchar_t* pSetExtension);
#endif

		/**
		 * @fn virtual void SetRaw(const char* pSetExtension = ".raw")
		 *
		 * @param pSetExtension
		 *  - Type: const char*
		 *  - Default: ".raw"
		 *  - Extension to use for the saved files
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetRaw </B> sets the output file format of the image writer to
		 * raw.
		 */
		virtual void SetRaw(const char* pSetExtension = ".raw");
#ifdef _WIN32
		virtual void SetRaw(const wchar_t* pSetExtension);
#endif

		/**
		 * @fn virtual void SetPly(const char* pSetExtension = ".ply", bool filterPoints = true, bool isSigned = false, float scale = 0.25f, float offsetA = 0.0f, float offsetB = 0.0f, float offsetC = 0.0f)
		 *
		 * @param pSetExtension
		 *  - Type: const char*
		 *  - Default: ".ply"
		 *  - Extension to use for the saved files
		 *
		 * @param filterPoints
		 *  - Type: bool
		 *  - Default: true
		 *  - Filter NaN points (A = B = C = -32,678)
		 *
		 * @param isSigned
		 *  - Type: bool
		 *  - Default: false
		 *  - If true, interpret data as signed pixel format
		 *	- Otherwise, interpret as unsigned pixel format
		 *
		 * @param scale
		 *  - Type: float
		 *  - Default: 0.25f
		 *	- Data scaling
		 *
		 * @param offsetA
		 *  - Type: float
		 *  - Default: 0.0f
		 *  - X-axis (A) offset
		 *
		 * @param offsetB
		 *  - Type: float
		 *  - Default: 0.0f
		 *  - Y-axis (B) offset
		 *
		 * @param offsetC
		 *  - Type: float
		 *  - Default: 0.0f
		 *  - Z-axis (C) offset
		 *
		 * @return
		 *  - none
		 *
		 * <B> SetPly </B> sets the output file format of the image writer to
		 * ply.
		 */
		virtual void SetPly(const char* pSetExtension = ".ply", bool filterPoints = true, bool isSigned = false, float scale = 0.25f, float offsetA = 0.0f, float offsetB = 0.0f, float offsetC = 0.0f);
#ifdef _WIN32
		virtual void SetPly(const wchar_t* pSetExtension, bool filterPoints = true, bool isSigned = false, float scale = 0.25f, float offsetA = 0.0f, float offsetB = 0.0f, float offsetC = 0.0f);
#endif

		/**
		 * @fn virtual void SetTiff(const char* pSetExtension = ".tiff")
		 *
		 * @param pSetExtension
		 *  - Type: const char*
		 *  - Default: ".tiff"
		 *  - Extension to use for the files
		 *
		 * @param compression
		 *	- Type: ETiffCompression
		 *	- Default: NoCompression
		 *	- Compression algorithm
		 *
		 * @param cmykTags
		 *	- Type: bool
		 *	- Default: false
		 *	- If true, stores tags for separated CMYK
		 *	- Otherwise, does not
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetTiff </B> sets the output file format of the image writer to
		 * TIFF.
		 */
		virtual void SetTiff(const char* pSetExtension = ".tiff", ETiffCompression compression = NoCompression, bool cmykTags = false);
#ifdef _WIN32
		virtual void SetTiff(const wchar_t* pSetExtension, ETiffCompression compression = NoCompression, bool cmykTags = false);
#endif

		/**
		 * @fn virtual void SetPng(const char* pSetExtension = ".png", size_t compression = 0, bool interlaced = false) 
		 *
		 * @param pSetExtension
		 *  - Type: const char*
		 *  - Default: ".png"
		 *  - Extension to use for the files
		 *
		 * @param compression
		 *	- Type: size_t
		 *	- Default: 0
		 *	- Range: 0-9
		 *	- Compression level
		 *
		 * @param interlaced
		 *	- Type: bool
		 *	- Default: false
		 *	- If true, uses Adam7 interlacing
		 *	- Otherwise, does not
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetPng </B> sets the output file format of the image writer to
		 * PNG.
		 */
		virtual void SetPng(const char* pSetExtension = ".png", size_t compression = 0, bool interlaced = false);
#ifdef _WIN32
		virtual void SetPng(const wchar_t* pSetExtension, size_t compression = 0, bool interlaced = false);
#endif

		/**
		 * @fn virtual void SetExtension(const char* pExt)
		 *
		 * @param pExt
		 *  - Type: const char*
		 *  - Extension to set
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetExtension </B> sets a new extension on the file name.
		 */
		virtual void SetExtension(const char* pExt);
#ifdef _WIN32
		virtual void SetExtension(const wchar_t* pExt);
#endif

		/**
		 * @fn virtual void SetParams(ImageParams params)
		 *
		 * @param params
		 *  - Type: Save::ImageParams
		 *  - Image parameters of the image data to save
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetParams </B> sets the image parameters of the images to save.
		 */
		virtual void SetParams(ImageParams params);

		/**
		 * @fn virtual void SetFileNamePattern(const char* pFileNamePattern)
		 *
		 * @param pFileNamePattern
		 *  - Type: const char*
		 *  - File name pattern to use for file names
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetFileNamePattern </B> sets a new file name or pattern for
		 * upcoming image(s) to save.
		 *
		 * <B> SetFileNamePattern </B> uses special tags to specify the file name
		 * to save. <datetime> uses the system clock to set the file name pattern
		 * to include the timestamp of the created image. <datetime> accepts a
		 * format string, and defaults to <datetime:yyMMdd_hhmmss_fff> (fff is
		 * milliseconds) <count> can be specified in three ways: <count:local> is
		 * the defaul value and counts according to a specific image writer or
		 * video recorder object. <count:path> counts the same file name pattern
		 * across all image writers and video recorders objects. This means that
		 * multiple objects that share a file name pattern will share their
		 * count. <count:global> shares a count across all image writers and
		 * video recorders.
		 *
		 * \code{.cpp}
		 * 	// setting file name pattern and saving image
		 * 	void SaveFiles(Arena::IDevice* pDevice, std::string serial, Save::ImageWriter* pImgWrt)
		 * 	{
		 *		pImgWrt->UpdateTag("<serial>", serial.c_str());
		 *
		 *		for (size_t i = 0; i < numImages; i++)
		 * 		{
		 * 			m_pImgWrt->SetFileNamePattern("Images/<serial>_image<count>-<datetime:yyMMdd_hhmmss_fff>.bmp");
		 *
		 * 			Arena::IImage* pImage = pDevice->GetImage(TIMEOUT);
		 * 			m_pImgWrt->Save(pImage->GetData());
		 * 		}
		 * 	}
		 * \endcode
		 */
		virtual void SetFileNamePattern(const char* pFileNamePattern);
#ifdef _WIN32
		virtual void SetFileNamePattern(const wchar_t* pFileNamePattern);
#endif

		/**
		 * @fn virtual void UpdateTag(const char* pTag, const char* pValue)
		 *
		 * @param pTag
		 *  - Type: const char*
		 *  - Tag to replace
		 *
		 * @param pValue
		 *  - Type: const char*
		 *  - Value to set
		 *
		 * @return 
		 *  - none
		 *
		 * <B> UpdateTag </B> updates the value to replace a given tag when an
		 * image is saved.
		 *
		 * \code{.cpp}
		 * 	// updating serial, width, height, and bits per pixel tags and setting as filename pattern to save images
		 * 	void UpdateTags(Arena::IDevice* pDevice, std::string serial, size_t w, size_t h, size_t bpp, Save::ImageWriter* pImgWrt, PluginController& ctrl)
		 * 	{
		 * 		pImgWrt->UpdateTag("<serial>", serial.c_str());
		 *		pImgWrt->UpdateTag("<width>", std::to_string(w).c_str());
		 *		pImgWrt->UpdateTag("<height>", std::to_string(h).c_str());
		 *		pImgWrt->UpdateTag("<bitsperpixel>", std::to_string(bpp).c_str());
		 *
		 *		GenICam::gcstring pixelFormat = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat");
		 *
		 *		pImgWrt->UpdateTag("<pixelformat>", pixelFormat);
		 *
		 *		m_pImgWrt->SetFileNamePattern(Images/<serial>_image<count>-<width>x<height>-<bitsperpixel>bpp_format<pixelformat>-<datetime>.bmp);
		 * 	}
		 * \endcode
		 */
		virtual void UpdateTag(const char* pTag, const char* pValue);
#ifdef _WIN32
		virtual void UpdateTag(const wchar_t* pTag, const wchar_t* pValue);
#endif

		/**
		 * @fn virtual void SetCount(unsigned long long count, ECountScope scope = Local)
		 *
		 * @param count
		 *  - Type: unsigned long long
		 *  - Value to set
		 *
		 * @param scope
		 *  - Type: ECountScope
		 *  - Default: Local
		 *  - Counter to set
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetCount </B> sets the value of one of the available counters.
		 *
		 * \code{.cpp}
		 * 	// adding the counter value to saved images
		 * 	{
		 * 		Save::ImageWriter writer(params, "savedimages/image<count>.jpg");
		 * 		
		 * 		for (size_t i = 0; i < numImages; i++)
		 * 		{
		 * 			// ...
		 * 			writer.Save(pImageData);
		 * 		}
		 * 	}
		 * \endcode
		 */
		virtual void SetCount(unsigned long long count, ECountScope scope = Local);

		/**
		 * @fn virtual void SetTimestamp(unsigned long long timestamp)
		 *
		 * @param timestamp
		 *  - Type: unsigned long long
		 *  - Timestamp to set
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetTimestamp </B> updates the timestamp to use in any file name
		 * patterns.
		 *
		 * \code{.cpp}
		 * 	// using the timestamp to differentiate images
		 * 	{
		 * 		Save::ImageWriter writer(params, "savedimages/image<timestamp>.jpg");
		 * 		
		 * 		for (size_t i = 0; i < numImages; i++)
		 * 		{
		 * 			// ...
		 * 			writer.SetTimestamp(timestamp);
		 * 			writer.Save(pImageData);
		 * 		}
		 * 	}
		 * \endcode
		 *
		 * @deprecated 
		 *  - Deprecated in favor of the more generic tag and value components
		 */
		virtual void SetTimestamp(unsigned long long timestamp);

		/**
		 * @fn virtual ImageParams GetParams()
		 *
		 * @return 
		 *  - Type: Save::ImageParams
		 *  - Image parameters
		 *
		 * <B> GetParams </B> retrieves the image parameters used to save images.
		 */
		virtual ImageParams GetParams();

		/**
		 * @fn virtual std::string GetFileNamePattern(bool withPath = false, bool withExt = true)
		 *
		 * @param withPath
		 *  - Type: bool
		 *  - Default: false
		 *  - If true, directories included in returned file name pattern
		 *  - Otherwise, not
		 *
		 * @param withExt
		 *  - Type: bool
		 *  - Default: true
		 *  - If true, extension included in returned file name pattern
		 *  - Otherwise, not
		 *
		 * @return 
		 *  - Type: std::string
		 *  - File name pattern
		 *
		 * <B> GetFileNamePattern </B> gets the file name pattern currently being
		 * used to save images.
		 */
		virtual std::string GetFileNamePattern(bool withPath = false, bool withExt = true);
#ifdef _WIN32
		virtual std::wstring GetFileNamePatternU(bool withPath = false, bool withExt = true);
#endif

		/**
		 * @fn virtual std::string GetPath()
		 *
		 * @return 
		 *  - Type: std::string
		 *  - Path
		 *
		 * <B> GetPath </B> retrieves the path portion of the file name pattern.
		 * Tags '<count>' and '<timestamp>' are not replaced.
		 */
		virtual std::string GetPath();
#ifdef _WIN32
		virtual std::wstring GetPathU();
#endif

		/**
		 * @fn virtual std::string GetExtension()
		 *
		 * @return 
		 *  - Type: std::string
		 *  - Extension to save
		 *
		 * <B> GetExtension </B> retrieves the extension portion of the file name
		 * pattern.
		 */
		virtual std::string GetExtension();
#ifdef _WIN32
		virtual std::wstring GetExtensionU();
#endif

		/**
		 * @fn virtual std::string PeekFileName(bool withPath = false, bool withExt = true)
		 *
		 * @param withPath
		 *  - Type: bool
		 *  - Default: false
		 *  - If true, directories included in returned file name pattern
		 *  - Otherwise, not
		 *
		 * @param withExt
		 *  - Type: bool
		 *  - Default: true
		 *  - If true, extension included in returned file name pattern
		 *  - Otherwise, not
		 *
		 * @return 
		 *  - Type: std::string
		 *  - Next file name to save
		 *
		 * <B> PeekFileName </B> provides a look at the file name that the next
		 * image will be saved as. Tags '<count>' and '<timestamp>' are replaced
		 * with their respective values.
		 */
		virtual std::string PeekFileName(bool withPath = false, bool withExt = true);
#ifdef _WIN32
		virtual std::wstring PeekFileNameU(bool withPath = false, bool withExt = true);
#endif

		/**
		 * @fn virtual unsigned long long PeekCount(ECountScope scope = Local)
		 *
		 * @param scope
		 *  - Type: ECountScope
		 *  - Default: Local
		 *  - Counter to peek
		 *
		 * @return 
		 *  - Type: unsigned long long
		 *  - Current count value
		 *
		 * <B> PeekCount </B> retrieves a peek at the value of one of the
		 * available counters.
		 */
		virtual unsigned long long PeekCount(ECountScope scope = Local);

		/**
		 * @fn virtual std::string GetLastFileName(bool withPath = false, bool withExt = true)
		 *
		 * @param withPath
		 *  - Type: bool
		 *  - Default: false
		 *  - If true, directories included in returned file name pattern
		 *  - Otherwise, not
		 *
		 * @param withExt
		 *  - Type: bool
		 *  - Default: true
		 *  - If true, extension included in returned file name pattern
		 *  - Otherwise, not
		 *
		 * @return 
		 *  - Type: std::string
		 *  - Last file name
		 *
		 * <B> GetLastFileName </B> provides a look at the last file name that an
		 * image was saved under. Tags '<count>' and '<timestamp>' are replaced
		 * with their respective values.
		 *
		 * \code{.cpp}
		 * 	// retrieving an image and saving it to the writer, then printing the file
		 * 	// name of the most recently saved image.
		 * 	{
		 *		Save::ImageWriter writer(params, "savedimages/image-<count>_<timestamp>.jpg");
		 *		Arena::IImage* pImage = pDevice->GetImage(100);
		 *		writer << pImage->GetData();
		 *
		 *		std::cout << "Save image at " << writer.GetLastFileName(true) << "\n";
		 *
		 *		pDevice->RequeueBuffer(pImage);
		 * 	}
		 * \endcode
		 *
		 */
		virtual std::string GetLastFileName(bool withPath = false, bool withExt = true);
#ifdef _WIN32
		virtual std::wstring GetLastFileNameU(bool withPath = false, bool withExt = true);
#endif

		/**
		 * @fn virtual void Save(const uint8_t* pData, bool createDirectories = true)
		 *
		 * @param pData
		 *  - Type: const uint8_t*
		 *  - Pointer to the image data to save
		 *
		 * @param createDirectories
		 *  - Type: bool createDirectories
		 *  - Default: true
		 *  - If true, attempts to create any missing directories in the path
		 *  - Otherwise, does not create any directories
		 *
		 * @return 
		 *  - none
		 *
		 * <B> Save </B> saves an image according to the file name pattern, image
		 * parameters, and image data.
		 */
		virtual void Save(const uint8_t* pData, bool createDirectories = true);

		/**
		* @fn virtual void Save(const uint8_t* pData, const uint8_t* pColor, bool createDirectories = true)
		*
		* @param pData
		*  - Type: const uint8_t*
		*  - Pointer to the image data to save
		*
		* @param pColor
		*  - Type: const uint8_t*
		*  - Pointer to RGB data for image coloring
		*
		* @param createDirectories
		*  - Type: bool createDirectories
		*  - Default: true
		*  - If true, attempts to create any missing directories in the path
		*  - Otherwise, does not create any directories
		*
		* @return
		*  - none
		*
		* <B> Save </B> saves an image according to the file name pattern, image
		* parameters, and image data.
		*/
		virtual void Save(const uint8_t* pData, const uint8_t* pColor, bool createDirectories = true);

		/**
		 * @fn virtual ImageWriter& operator<<(const char* pInput)
		 *
		 * @param pInput
		 *  - Type: const char*
		 *  - Accepts both tags and values
		 *
		 * @return 
		 *  - Type: Save::ImageWriter&
		 *  - The object returns itself in order to cascade
		 *
		 * The cascading I/O operator allows for the setting of tags and their
		 * values on the fly.
		 *
		 * \code{.cpp}
		 * 	// using the cascading I/O operator (<<) to replace tags with values
		 * 	// using the cascading I/O operator (<<) to save an image
		 * 	{
		 * 		Save::ImageWriter writer(params, "path/img-<model>-<serial>.raw");
		 * 		
		 * 		// update file name and save
		 * 		writer << "<model>" << deviceModelName
		 * 		<< "<serial>" << deviceSerialNumber
		 * 		<< pImageData;
		 * 	}
		 * \endcode
		 */
		virtual ImageWriter& operator<<(const char* pInput);
#ifdef _WIN32
		virtual ImageWriter& operator<<(const wchar_t* pInput);
#endif

		/**
		 * @fn virtual ImageWriter& operator<<(unsigned long long timestamp)
		 *
		 * @param timestamp
		 *  - Type: unsigned long long
		 *  - Timestamp
		 *
		 * @return 
		 *  - Type: Save::ImageWriter&
		 *  - The object returns itself in order to cascade
		 *
		 * The cascading I/O operator sets the timestamp
		 * (Save::ImageWriter::SetTimestamp) when an unsigned 64-bit integer is
		 * passed in.
		 *
		 * \code{.cpp}
		 * 	// using timestamp tag to cascading I/O operator to save a set of images
		 * 	{
		 * 		Save::ImageWriter writer(params, "savedimages/image<timestamp>.jpg");
		 * 		for (size_t i = 0; i < numImages; i++)
		 * 		{
		 * 			unsigned long long timestamp;
		 * 			// update timestamp
		 * 			writer << timestamp << pImageData;
		 * 		}
		 * 	}
		 * \endcode
		 *
		 * @deprecated 
		 *  - Deprecated in favor of the more generic tag and value components
		 */
		virtual ImageWriter& operator<<(unsigned long long timestamp);

		/**
		 * @fn virtual ImageWriter& operator<<(const uint8_t* pData)
		 *
		 * @param pData
		 *  - Type: const uint8_t*
		 *  - Pointer to the image data to save
		 *
		 * @return 
		 *  - Type: Save::ImageWriter&
		 *  - The object returns itself in order to cascade
		 *
		 * The cascading I/O operator saves an image (Save::ImageWriter::Save)
		 * when image data is passed in.
		 *
		 * @warning 
		 *  - Creates any missing directories
		 */
		virtual ImageWriter& operator<<(const uint8_t* pData);

		/**
		 * @fn virtual ImageWriter& operator<<(ImageParams params)
		 *
		 * @param params
		 *  - Type: Save::ImageParams
		 *  - Parameters of the image
		 *
		 * @return 
		 *  - Type: Save::ImageWriter&
		 *  - The object returns itself in order to cascade
		 *
		 * The cascading I/O operator updates the image parameters
		 * (Save::ImageWriter::SetParams) when an image parameters object is passed
		 * in.
		 */
		virtual ImageWriter& operator<<(ImageParams params);

	private:
		void* m_pInternal;
	};
} // namespace Save
