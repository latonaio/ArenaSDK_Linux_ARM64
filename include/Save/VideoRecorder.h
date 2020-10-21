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

namespace Save
{
	class VideoParams;

	/**
	 * @class VideoRecorder
	 *
	 * The video recorder facilitates the grouping of image data into video
	 * files.
	 */
	class SAVE_API VideoRecorder
	{
	public:
		/**
		 * @fn VideoRecorder()
		 *
		 * An empty constructor.
		 *
		 * The video recorder defaults to an empty video parameters object and
		 * 'savedvideos/video<count>.avi' for the file name pattern.
		 */
		VideoRecorder();

		/**
		 * @fn VideoRecorder(VideoParams params, const char* pFileNamePattern = "savedvideos/video<count>.avi")
		 *
		 * @param params
		 *  - Type: VideoParams
		 *  - Parameters of the video
		 *
		 * @param pFileNamePattern
		 *  - Type: const char*
		 *  - Default: "savedvideos/video<count>.avi"
		 *  - File name pattern to use with this recorder
		 *
		 * A constructor.
		 *
		 * The video recorder defaults to different settings depending on the
		 * extension of the file name pattern:
		 *  - '.avi' defaults to raw codec, BGR8 pixel format
		 *  - '.mov' defaults to raw codec, RGB8 pixel format
		 *  - '.mp4' defaults to H264 codec, BGR8 pixel format
		 */
		VideoRecorder(VideoParams params, const char* pFileNamePattern = "savedvideos/video<count>.avi");
#ifdef _WIN32
		VideoRecorder(VideoParams params, const wchar_t* pFileNamePattern);
#endif

		/**
		 * @fn virtual ~VideoRecorder()
		 *
		 * A destructor.
		 */
		virtual ~VideoRecorder();

		/**
		 * @fn virtual void SetRaw(uint64_t pixelFormat)
		 *
		 * @param pixelFormat
		 *  - Type: uint64_t
		 *  - Pixel format to use
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetRaw </B> sets the recorder to write raw frames with no
		 * container or metadata. Frame data can be written for any pixel format,
		 * but data cannot be played back. It will also set the file extension to
		 * '.raw'.
		 *
		 * @warning 
		 *  - changes file extension
		 *  - cannot be called while recording
		 */
		virtual void SetRaw(uint64_t pixelFormat);

		/**
		 * @fn virtual void SetRawAviBGR8()
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetRawAviBGR8 </B> sets the codec to raw, the container to AVI,
		 * and the expected pixel format to BGR8. It will also set the file
		 * extension to '.avi'.
		 *
		 * @warning 
		 *  - pushing non-BGR8 data will result in unexpected behavior
		 *  - changes file extension
		 *  - cannot be called while recording
		 */
		virtual void SetRawAviBGR8();

		/**
		 * @fn virtual void SetRawMovRGB8()
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetRawMovRGB8 </B> sets the codec to raw, the container to MOV,
		 * and the expected pixel format to RGB8. It will also set the file
		 * extension to '.mov'.
		 *
		 * @warning 
		 *  - pushing non-RGB8 data will result in unexpected behavior
		 *  - changes file extension
		 *  - cannot be called while recording
		 */
		virtual void SetRawMovRGB8();

		/**
		 * @fn virtual void SetH264MovRGB8(int64_t bitrate)
		 *
		 * @param bitrate
		 *	- Type: int64_t
		 *	- Default: 0
		 *	- Bitrate (bit/s)
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetH264MovRGB8 </B> sets the codec to H264, the container to MOV,
		 * and the expected pixel format to RGB8. It will also set the file
		 * extension to '.mov'.
		 *
		 * @warning 
		 *  - pushing non-RGB8 data will result in unexpected behavior
		 *  - changes file extension
		 *  - cannot be called while recording
		 */
		virtual void SetH264MovRGB8(int64_t bitrate = 0);

		/**
		 * @fn virtual void SetH264MovBGR8(int64_t bitrate)
		 *
		 * @param bitrate
		 *	- Type: int64_t
		 *	- Default: 0
		 *	- Bitrate (bit/s)
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetH264MovBGR8 </B> sets the codec to H264, the container to MOV,
		 * and the expected pixel format to BGR8. It will also set the file
		 * extension to '.mov'.
		 *
		 * @warning 
		 *  - pushing non-BGR8 data will result in unexpected behavior
		 *  - changes file extension
		 *  - cannot be called while recording
		 */
		virtual void SetH264MovBGR8(int64_t bitrate = 0);

		/**
		 * @fn virtual void SetH264Mp4RGB8(int64_t bitrate)
		 *
		 * @param bitrate
		 *	- Type: int64_t
		 *	- Default: 0
		 *	- Bitrate (bit/s)
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetH264Mp4RGB8 </B> sets the codec to H264, the container to MP4,
		 * and the expected pixel format to RGB8. It will also set the file
		 * extension to '.mp4'.
		 *
		 * @warning 
		 *  - pushing non-RGB8 data will result in unexpected behavior
		 *  - changes file extension
		 *  - cannot be called while recording
		 */
		virtual void SetH264Mp4RGB8(int64_t bitrate = 0);

		/**
		 * @fn virtual void SetH264Mp4BGR8(int64_t bitrate)
		 *
		 * @param bitrate
		 *	- Type: int64_t
		 *	- Default: 0
		 *	- Bitrate (bit/s)
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetH264Mp4BGR8 </B> sets the codec to H264, the container to MP4,
		 * and the expected pixel format to BGR8. It will also set the file
		 * extension to '.mp4'.
		 *
		 * \code{.cpp}
		 *  // prepare and open video recorder to prepare to create recorded video
		 *  {
		 * 		void RecordVideo(const std::vector<Arena::IImage*>& images)
		 * 		{
		 * 			Save::VideoParams params(images[0]->GetWidth(), images[0]->GetHeight(), 25);
		 * 			Save::VideoRecorder videoRecorder(params, Cpp_Record/video.mp4);
		 *
		 * 			videoRecorder.SetH264Mp4BGR8();
		 *
		 * 			videoRecorder.Open();
		 * 		}
		 *  }
		 * \endcode
		 *
		 * @warning 
		 *  - pushing non-BGR8 data will result in unexpected behavior
		 *  - changes file extension
		 *  - cannot be called while recording
		 */
		virtual void SetH264Mp4BGR8(int64_t bitrate = 0);

		/**
		 * @fn virtual void SetParams(VideoParams params)
		 *
		 * @param params
		 *  - Type: VideoParams
		 *  - Parameters to set
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetParams </B> sets the width and height of the input frame data
		 * and the frames per second of the video output.
		 *
		 * @warning 
		 *  - cannot be called while recording
		 */
		virtual void SetParams(VideoParams params);

		/**
		 * @fn virtual void SetFileNamePattern(const char* pFileNamePattern)
		 *
		 * @param pFileNamePattern
		 *  - Type: const char*
		 *  - File name pattern to set
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetFileNamePattern </B> sets a file name pattern. Changing the
		 * extension via this call will change the output container.
		 *
		 * The video recorder defaults to different settings depending on the
		 * extension of the file name pattern:
		 *  - '.avi' defaults to raw codec, BGR8 pixel format
		 *  - '.mov' defaults to raw codec, RGB8 pixel format
		 *  - '.mp4' defaults to H264 codec, BGR8 pixel
		 *
		 * @warning 
		 *  - changes to the file extension will change the container
		 *    accordingly
		 *  - set file name pattern before setting other settings
		 *  - cannot be called while recording
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
		 * <B> UpdateTag </B> updates the value to replace a given tag when a
		 * video is saved.
		 */
		virtual void UpdateTag(const char* pTag, const char* pValue);
#ifdef _WIN32
		virtual void UpdateTag(const wchar_t* pTag, const wchar_t* pValue);
#endif

		/**
		 * @fn virtual void SetCount(uint64_t count, ECountScope scope = Local)
		 *
		 * @param count
		 *  - Type: uint64_t
		 *  - Value of the internal counter
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
		 * @warning 
		 *  - cannot be called while recording
		 */
		virtual void SetCount(uint64_t count, ECountScope scope = Local);

		/**
		 * @fn virtual void SetTimestamp(uint64_t timestamp)
		 *
		 * @param timestamp
		 *  - Type: uint64_t
		 *  - Timestamp to set
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetTimestamp </B> sets the timestamp. The timestamp replaces any
		 * instance of '<timestamp>' in the file name pattern.
		 *
		 * @warning 
		 *  - cannot be called while recording
		 *
		 * @deprecated 
		 *  - Deprecated in favor of the more generic tag and value components
		 */
		virtual void SetTimestamp(uint64_t timestamp);

		/**
		 * @fn virtual VideoParams GetParams()
		 *
		 * @return 
		 *  - Type: VideoParams
		 *  - Video parameters
		 *
		 * <B> GetParams </B> retrieves the video parameters object, which houses
		 * the width, height, and frames per second of the video.
		 */
		virtual VideoParams GetParams();

		/**
		 * @fn virtual std::string GetFileNamePattern(bool withPath = false, bool withExt = true)
		 *
		 * @param withPath
		 *  - Type: bool
		 *  - Default: false
		 *  - Whether or not to include the path
		 *
		 * @param withExt
		 *  - Type: bool
		 *  - Default: true
		 *  - Whether or not to include the extension
		 *
		 * @return 
		 *  - Type: std::string
		 *  - The file name pattern
		 *
		 * <B> GetFileNamePattern </B> retrieves the file name pattern of the
		 * recorder.
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
		 *  - The path
		 *
		 * <B> GetPath </B> retrieves the path that the video recorder is
		 * currently outputting videos to.
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
		 *  - The extension
		 *
		 * <B> GetExtension </B> The extension of the file name pattern.
		 */
		virtual std::string GetExtension();
#ifdef _WIN32
		virtual std::wstring GetExtensionU();
#endif

		/**
		 * @fn virtual std::string GetCodec()
		 *
		 * @return 
		 *  - Type: std::string
		 *  - Video codec
		 *
		 * <B> GetCodec </B> retrieves the compression codec the recorder is
		 * currently set to use for video encoding.
		 */
		virtual std::string GetCodec();
#ifdef _WIN32
		virtual std::wstring GetCodecU();
#endif

		/**
		 * @fn virtual uint64_t GetPixelFormat()
		 *
		 * @return 
		 *  - Type: uint64_t
		 *  - Pixel format, as defined in the PFNC (Pixel Format Naming
		 *    Convention)
		 *
		 * <B> GetPixelFormat </B> retrieves the integer value representation of
		 * the pixel format that the recorder is currently expecting.
		 *
		 */
		virtual uint64_t GetPixelFormat();

		/**
		 * @fn virtual std::string PeekFileName(bool withPath = false, bool withExt = true)
		 *
		 * @param withPath
		 *  - Type: bool
		 *  - Default: false
		 *  - Whether or not to include the path
		 *
		 * @param withExt
		 *  - Type: bool
		 *  - Default: true
		 *  - Whether or not to include the extension
		 *
		 * @return 
		 *  - Type: std::string
		 *  - File name of the next video
		 *
		 * <B> PeekFileName </B> retrieves a peek at the file name of the next
		 * video to be recorded, barring any changes to the file name pattern.
		 */
		virtual std::string PeekFileName(bool withPath = false, bool withExt = true);
#ifdef _WIN32
		virtual std::wstring PeekFileNameU(bool withPath = false, bool withExt = true);
#endif

		/**
		 * @fn virtual uint64_t PeekCount(ECountScope scope = Local)
		 *
		 * @param scope
		 *  - Type: ECountScope
		 *  - Default: Local
		 *  - Counter to peek
		 *
		 * @return 
		 *  - Type: uint64_t
		 *  - Value of the internal counter
		 *
		 * <B> PeekCount </B> retrieves a peek at the value of one of the
		 * available counters.
		 */
		virtual uint64_t PeekCount(ECountScope scope = Local);

		/**
		 * @fn virtual std::string GetLastFileName(bool withPath = false, bool withExt = true)
		 *
		 * @param withPath
		 *  - Type: bool
		 *  - Default: false
		 *  - Whether or not to include the path
		 *
		 * @param withExt
		 *  - Type: bool
		 *  - Default: true
		 *  - Whether or not to include the extension
		 *
		 * @return 
		 *  - Type: std::string
		 *  - The last file name
		 *
		 * <B> GetLastFileName </B> retrieves the last file name used for a
		 * video.
		 */
		virtual std::string GetLastFileName(bool withPath = false, bool withExt = true);
#ifdef _WIN32
		virtual std::wstring GetLastFileNameU(bool withPath = false, bool withExt = true);
#endif

		/**
		 * @fn virtual void Open()
		 *
		 * @return 
		 *  - none
		 *
		 * <B> Open </B> opens the output file, sets up internal buffers, frames,
		 * contexts, and settings, and allocates memory to prepare to append
		 * input frames.
		 *
		 * @warning 
		 *  - settings cannot be changed until the recorder is closed
		 */
		virtual void Open();

		/**
		 * @fn virtual void AppendImage(const uint8_t* pData)
		 *
		 * @param pData
		 *  - Type: const uint8_t*
		 *  - Data of the next frame to append
		 *
		 * @return 
		 *  - none
		 *
		 * <B> AppendImage </B> accepts the next frame's raw image data, encodes
		 * it, and appends it to the video file.
		 */
		virtual void AppendImage(const uint8_t* pData);

		/**
		 * @fn virtual void Close()
		 *
		 * @return 
		 *  - none
		 *
		 * <B> Close </B> cleans up memory and settings of the video, concluding
		 * the currently recorded video.
		 *
		 * @warning 
		 *  - additional frames cannot be appended once the file has been
		 *    closed
		 */
		virtual void Close();

	private:
		VideoRecorder(const VideoRecorder& videoRecorder);
		const VideoRecorder& operator=(VideoRecorder videoRecorder);
		void* m_pInternal;
	};
} // namespace Save
